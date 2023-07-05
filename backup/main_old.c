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
#include "freertos/semphr.h"
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

static const char *TAG = "Deep_Sleep_MAIN";

static RTC_DATA_ATTR struct timeval sleep_enter_time;

RTC_DATA_ATTR float luz[NUM_OF_DATA];
RTC_DATA_ATTR float HR[NUM_OF_DATA];
RTC_DATA_ATTR float ruido[NUM_OF_DATA];
RTC_DATA_ATTR float CO2[NUM_OF_DATA];
RTC_DATA_ATTR float temperatura[NUM_OF_DATA];
RTC_DATA_ATTR int64_t time_stamps[NUM_OF_DATA];
static RTC_DATA_ATTR int num_lecturas = 0;

RTC_DATA_ATTR int ultima_hora;

SemaphoreHandle_t semaforo;

float ultimo_ruido;

void tarea_Principal(void *parameters){
    int exito = 0;
    int exito_thingsboard = 0;

    int wifi_conectado = 0;

    int ota_res = 0;

    int dato_ruido_listo = 0;

    time_t now;
    struct tm timeinfo;
    struct timeval tv_now;

    configurar_i2c();

    exito = iniciar_wifi();

    while (1)
    {
        // if (!exito)
        // {
        //     exito = iniciar_wifi();
        // }
        
        time(&now);
        localtime_r(&now, &timeinfo);
        // Is time set? If not, tm_year will be (1970 - 1900).
        if (timeinfo.tm_year < (2016 - 1900)) {
            ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
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
        printf("Esperando que termina ruido");
        xSemaphoreTake(semaforo, portMAX_DELAY);
        gettimeofday(&tv_now, NULL);


        ruido[num_lecturas] = ultimo_ruido;


        luz[num_lecturas] = get_luz();
        vTaskDelay(1000);
        temperatura[num_lecturas] = get_temperatura();
        vTaskDelay(20);
        HR[num_lecturas] = get_humedad();
        vTaskDelay(1000);
        CO2[num_lecturas] = get_carbono();
        printf("\n\n----------------- TimeStamp --------------------");
        printf("\nSeconds: %lld", tv_now.tv_sec);
        printf("\nMicro Seconds: %ld\n", tv_now.tv_usec);
        time_stamps[num_lecturas] = tv_now.tv_sec * 1000 + tv_now.tv_usec / 1000;;


        num_lecturas += 1;

        if (num_lecturas == NUM_OF_DATA)
        {
            // exito = iniciar_wifi();

            printf("Wifi inicializado");
            
            exito_thingsboard = enviar_datos(time_stamps, luz, temperatura, HR, CO2, ruido);
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
            
            if (timeinfo.tm_hour != ultima_hora)
            {
                // ota_res = update_firmware_OTA();
                ota_res = 1;
                if (ota_res != 1)
                {
                    ultima_hora = timeinfo.tm_hour;   
                }
            }
            
            // parar_wifi();
            printf("Wifi parado");
            num_lecturas = 0;
        }

        printf("\nNumber of data measurements: %d / %d\n", num_lecturas, NUM_OF_DATA);
    }
}

void tarea_micro(void *parameters)
{
    int ruido = 0;
    spi_init();
    while (1)
    {
        ultimo_ruido = get_ruido();
        xSemaphoreGive(semaforo);
    }
}




void app_main(void)
{   
    ESP_LOGI(TAG, "Esta el la ultima version de deep_sleep.");

    semaforo = xSemaphoreCreateBinary();

    xTaskCreate(tarea_Principal, "tarea_Principal", 4096, NULL, 5, NULL);
    xTaskCreate(tarea_micro, "tarea_micro", 4096, NULL, 5, NULL);

}



