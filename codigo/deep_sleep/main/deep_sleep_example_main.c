#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "soc/soc_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "driver/rtc_io.h"
#include "soc/rtc.h"
#include "esp_sntp.h"
#include "sensor_i2c.h"
#include "sensor_SPI.h"
#include "conexion_wifi.h"
#include "thingsboard.h"
#include "ota.h"

#define NUM_OF_DATA CONFIG_NUM_OF_DATA

static const char *TAG = "Deep_Sleep_MAIN";

static RTC_DATA_ATTR struct timeval sleep_enter_time;

RTC_DATA_ATTR float luz[NUM_OF_DATA];
RTC_DATA_ATTR float HR[NUM_OF_DATA];
RTC_DATA_ATTR float temperatura[NUM_OF_DATA];
RTC_DATA_ATTR float CO2[NUM_OF_DATA];
RTC_DATA_ATTR int64_t time_stamps[NUM_OF_DATA];
static RTC_DATA_ATTR int num_lecturas = 0;

RTC_DATA_ATTR int ultima_hora = -1;

static void obtain_time(void);
static void initialize_sntp(void);


void app_main(void)
{   
    ESP_LOGI(TAG, "Esta el la ultima version de deep_sleep.");
    int exito = 0;
    int exito_thingsboard = 0;

    int wifi_conectado = 0;

    int ota_res = 0;

    time_t now;
    struct tm timeinfo;


    configurar_i2c();

    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        obtain_time();
        // update 'now' variable with current time
        time(&now);
        char strftime_buf[64];
        // Set timezone to Madrid time
        setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
        tzset();

        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
        ESP_LOGI(TAG, "The current date/time in Madrid is: %s", strftime_buf);
    }



    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    int sleep_time_ms = (tv_now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (tv_now.tv_usec - sleep_enter_time.tv_usec) / 1000;

    switch (esp_sleep_get_wakeup_cause()) {

        case ESP_SLEEP_WAKEUP_TIMER: {
            printf("Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
            break;
        }
        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
            printf("Not a deep sleep reset\n");
    }


    luz[num_lecturas] = get_luz();
    vTaskDelay(1000);
    temperatura[num_lecturas] = get_temperatura();
    vTaskDelay(20);
    HR[num_lecturas] = get_humedad();
    vTaskDelay(1000);
    CO2[num_lecturas] = get_carbono();
    printf("\n\n----------------- TimeStamp  VERISION 3  --------------------");
    printf("\nSeconds: %lld", tv_now.tv_sec);
    printf("\nMicro Seconds: %ld\n", tv_now.tv_usec);
    time_stamps[num_lecturas] = tv_now.tv_sec * 1000 + tv_now.tv_usec / 1000;;


    num_lecturas += 1;

    

    if (num_lecturas == NUM_OF_DATA)
    {
        exito = iniciar_wifi();
        
        exito_thingsboard = enviar_datos(time_stamps, luz, temperatura, HR, CO2);
        if (exito_thingsboard != -1)
        {
            for (int i = 0; i < NUM_OF_DATA; i++)
            {   
                printf("\n\nTimestamp: %lld", time_stamps[i]);
                printf("\nLUZ : %f", luz[i]);
                printf("\nHumedad Relativa : %f", HR[i]);
                printf("\nTemperatura : %f\n", temperatura[i]);
            }
        }
        else
        {
            ESP_LOGE(TAG, "No se pudo subir los datos a Thingsboard");
        }
        
        if (ultima_hora == -1)
        {
            ultima_hora = timeinfo.tm_hour;
        }
        
        if (timeinfo.tm_hour != ultima_hora)
        {
            ota_res = update_firmware_OTA();
            if (ota_res != 1)
            {
                ultima_hora = timeinfo.tm_hour;   
            }
        }
        
        parar_wifi();
        num_lecturas = 0;
    }

    printf("\nNumber of data measurements: %d / %d\n", num_lecturas, NUM_OF_DATA);
    



    //vTaskDelay(1000 / portTICK_PERIOD_MS);

    const int wakeup_time_sec = 20;  // 300 = 5 min
    printf("Enabling timer wakeup, %ds\n", wakeup_time_sec);
    esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);

#if CONFIG_IDF_TARGET_ESP32
    // Isolate GPIO12 pin from external circuits. This is needed for modules
    // which have an external pull-up resistor on GPIO12 (such as ESP32-WROVER)
    // to minimize current consumption.
    rtc_gpio_isolate(GPIO_NUM_12);
#endif

    printf("Entering deep sleep\n");
    gettimeofday(&sleep_enter_time, NULL);

    esp_deep_sleep_start();
}



//Functions to set the time

static void obtain_time(void)
{
    /**
     * NTP server address could be aquired via DHCP,
     * see following menuconfig options:
     * 'LWIP_DHCP_GET_NTP_SRV' - enable STNP over DHCP
     * 'LWIP_SNTP_DEBUG' - enable debugging messages
     *
     * NOTE: This call should be made BEFORE esp aquires IP address from DHCP,
     * otherwise NTP option would be rejected by default.
     */
#ifdef LWIP_DHCP_GET_NTP_SRV
    sntp_servermode_dhcp(1);      // accept NTP offers from DHCP server, if any
#endif

    if (iniciar_wifi())
    {
        initialize_sntp();

        // wait for time to be set
        time_t now = 0;
        struct tm timeinfo = { 0 };
        int retry = 0;
        const int retry_count = 15;
        while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
            ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        time(&now);
        localtime_r(&now, &timeinfo);

        parar_wifi();
    }

}

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);

/*
 * If 'NTP over DHCP' is enabled, we set dynamic pool address
 * as a 'secondary' server. It will act as a fallback server in case that address
 * provided via NTP over DHCP is not accessible
 */
#if LWIP_DHCP_GET_NTP_SRV && SNTP_MAX_SERVERS > 1
    sntp_setservername(1, "pool.ntp.org");

#if LWIP_IPV6 && SNTP_MAX_SERVERS > 2          // statically assigned IPv6 address is also possible
    ip_addr_t ip6;
    if (ipaddr_aton("2a01:3f7::1", &ip6)) {    // ipv6 ntp source "ntp.netnod.se"
        sntp_setserver(2, &ip6);
    }
#endif  /* LWIP_IPV6 */

#else   /* LWIP_DHCP_GET_NTP_SRV && (SNTP_MAX_SERVERS > 1) */
    // otherwise, use DNS address from a pool
    sntp_setservername(0, CONFIG_SNTP_TIME_SERVER);

    sntp_setservername(1, "pool.ntp.org");     // set the secondary NTP server (will be used only if SNTP_MAX_SERVERS > 1)
#endif

#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
#endif
    sntp_init();
}
