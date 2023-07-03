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
#include "tiempo.h"

#define NUM_OF_DATA CONFIG_NUM_OF_DATA

static const char *TAG = "Deep_Sleep_V4";

static RTC_DATA_ATTR struct timeval sleep_enter_time;

RTC_DATA_ATTR float luz[NUM_OF_DATA];
RTC_DATA_ATTR float HR[NUM_OF_DATA];
RTC_DATA_ATTR float temperatura[NUM_OF_DATA];
RTC_DATA_ATTR float CO2[NUM_OF_DATA];
RTC_DATA_ATTR int64_t time_stamps[NUM_OF_DATA];
static RTC_DATA_ATTR int num_lecturas = 0;

RTC_DATA_ATTR int ultima_hora = -1;


void app_main(void)
{   
    // ESP_LOGI(TAG, "Esta el la ultima version de deep_sleep.");
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
        ESP_LOGW(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        obtener_tiempo();
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
            ESP_LOGI(TAG,"Wake up from timer. Time spent in deep sleep: %dms\n", sleep_time_ms);
            break;
        }
        case ESP_SLEEP_WAKEUP_UNDEFINED:
        default:
            ESP_LOGI(TAG,"Not a deep sleep reset\n");
    }


    luz[num_lecturas] = get_luz();
    vTaskDelay(100);
    temperatura[num_lecturas] = get_temperatura();
    vTaskDelay(100);
    HR[num_lecturas] = get_humedad();
    vTaskDelay(100);
    CO2[num_lecturas] = get_carbono();
    ESP_LOGI(TAG,"----------------- TimeStamp  VERISION 4  --------------------");
    ESP_LOGI(TAG,"Seconds: %lld", tv_now.tv_sec);
    ESP_LOGI(TAG,"Micro Seconds: %ld\n", tv_now.tv_usec);
    time_stamps[num_lecturas] = tv_now.tv_sec * 1000 + tv_now.tv_usec / 1000;;


    num_lecturas += 1;

    

    if (num_lecturas == 1)
    {
        exito = iniciar_wifi();
        
        exito_thingsboard = enviar_datos(time_stamps, luz, temperatura, HR, CO2);
        if (exito_thingsboard != -1)
        {
            for (int i = 0; i < NUM_OF_DATA; i++)
            {   
                ESP_LOGI(TAG,"Timestamp: %lld", time_stamps[i]);
                ESP_LOGI(TAG,"LUZ : %f", luz[i]);
                ESP_LOGI(TAG,"Humedad Relativa : %f", HR[i]);
                ESP_LOGI(TAG,"Temperatura : %f", temperatura[i]);
                ESP_LOGI(TAG,"CO2 : %f\n", CO2[i]);
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
        
        // if (timeinfo.tm_hour != ultima_hora)
        if (1)
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

    ESP_LOGI(TAG,"\nNumber of data measurements: %d / %d\n", num_lecturas, NUM_OF_DATA);
    



    //vTaskDelay(1000 / portTICK_PERIOD_MS);

    const int wakeup_time_sec = 20;  // 300 = 5 min
    ESP_LOGI(TAG,"Enabling timer wakeup, %ds\n", wakeup_time_sec);
    esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);

#if CONFIG_IDF_TARGET_ESP32
    // Isolate GPIO12 pin from external circuits. This is needed for modules
    // which have an external pull-up resistor on GPIO12 (such as ESP32-WROVER)
    // to minimize current consumption.
    rtc_gpio_isolate(GPIO_NUM_12);
#endif

    ESP_LOGI(TAG,"Entering deep sleep\n");
    gettimeofday(&sleep_enter_time, NULL);

    esp_deep_sleep_start();
}