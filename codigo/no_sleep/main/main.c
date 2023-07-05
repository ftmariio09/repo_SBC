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

static struct timeval sleep_enter_time;

float luz[NUM_OF_DATA];
float HR[NUM_OF_DATA];
float ruido[NUM_OF_DATA];
float CO2[NUM_OF_DATA];
float temperatura[NUM_OF_DATA];
int64_t time_stamps[NUM_OF_DATA];
static int num_lecturas = 0;

int ultima_hora;

void app_main(void)
{   
    ESP_LOGI(TAG, "Esta el la ultima version de deep_sleep.");

    int exito = 0;
    int exito_thingsboard = 0;

    int wifi_conectado = 0;

    int ota_res = 0;

    int dato_ruido_listo = 0;

    time_t now;
    struct tm timeinfo;
    struct timeval tv_now;

    configurar_i2c();
    spi_init();

    while (1)
    {
        time(&now);
        localtime_r(&now, &timeinfo);
        if (timeinfo.tm_year < (2016 - 1900)) {
            exito = iniciar_wifi();

            ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
            obtener_tiempo();

            parar_wifi();

            time(&now);
            char strftime_buf[64];

            setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
            tzset();

            localtime_r(&now, &timeinfo);
            strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
            ESP_LOGI(TAG, "The current date/time in Madrid is: %s", strftime_buf);
        }

        gettimeofday(&tv_now, NULL);


        ruido[num_lecturas] = get_ruido();

        luz[num_lecturas] = get_luz();
        vTaskDelay(1000);
        temperatura[num_lecturas] = get_temperatura();
        vTaskDelay(20);
        HR[num_lecturas] = get_humedad();
        vTaskDelay(1000);
        CO2[num_lecturas] = get_carbono();
        ESP_LOGI(TAG,"----------------- TimeStamp  VERISION 4  --------------------");
        ESP_LOGI(TAG,"Seconds: %lld", tv_now.tv_sec);
        ESP_LOGI(TAG,"Micro Seconds: %ld\n", tv_now.tv_usec);
        time_stamps[num_lecturas] = tv_now.tv_sec * 1000 + tv_now.tv_usec / 1000;;


        num_lecturas += 1;

        if (num_lecturas == NUM_OF_DATA)
        {
            exito = iniciar_wifi();

            printf("Wifi inicializado");
            
            exito_thingsboard = enviar_datos(time_stamps, luz, temperatura, HR, CO2, ruido);
            if (exito_thingsboard != -1)
            {
                for (int i = 0; i < NUM_OF_DATA; i++)
                {   
                    ESP_LOGI(TAG,"Timestamp: %lld", time_stamps[i]);
                    ESP_LOGI(TAG,"LUZ : %f", luz[i]);
                    ESP_LOGI(TAG,"Humedad Relativa : %f", HR[i]);
                    ESP_LOGI(TAG,"Temperatura : %f", temperatura[i]);
                    ESP_LOGI(TAG,"CO2 : %f\n", CO2[i]);
                    ESP_LOGI(TAG,"Ruido : %f\n", ruido[i]);
                }
            }
            else
            {
                ESP_LOGE(TAG, "No se pudo subir los datos a Thingsboard");
            }
            
            if (timeinfo.tm_hour != ultima_hora)
            {
                ota_res = update_firmware_OTA();
                ota_res = 1;
                if (ota_res != 1)
                {
                    ultima_hora = timeinfo.tm_hour;   
                }
            }
            
            parar_wifi();
            num_lecturas = 0;
        }

        printf("\nNumber of data measurements: %d / %d\n", num_lecturas, NUM_OF_DATA);
    }
}




