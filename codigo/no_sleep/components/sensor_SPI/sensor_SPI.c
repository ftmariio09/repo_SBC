#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#include "sdkconfig.h"
#include "esp_log.h"




#define CLK_PIN     18
#define MISO_PIN    19
#define CS_PIN      5


spi_device_handle_t spi2;
uint8_t rx_data[2];

int lvl_ruido(uint16_t ruido)
{

    if( 1900 < ruido && ruido < 2100){
        return 0;
    }
    else if( ( 2100 <= ruido  && ruido < 2300 ) || ( 1700 < ruido  && ruido <= 1900) ){
        return 1;
    }
    else if( ( 2300 <= ruido  && ruido < 2500 ) || ( 1500 < ruido  && ruido <= 1700 ) ){
        return 2;
    }
    else if( ( 2500 <= ruido  && ruido < 2700 ) || ( 1300 < ruido  && ruido <= 1500 ) ){
        return 3;
    }
    else if( ( 2700 <= ruido  && ruido < 2900 ) || ( 1100 < ruido  && ruido <= 1300 ) ){
        return 4;
    }
    else if( ( 2900 <= ruido  && ruido < 3100 ) || ( 900 < ruido  && ruido <= 1100 ) ){
        return 5;
    }
    else if( ( 3100 <= ruido  && ruido < 3300 ) || ( 700 < ruido  && ruido <= 900 ) ){
        return 6;
    }
    else if( ( 3300 <= ruido  && ruido < 3500 ) || ( 500 < ruido  && ruido <= 700 ) ){
        return 7;
    }
    else if( ( 3500 <= ruido  && ruido < 3700 ) || ( 300 < ruido  && ruido <= 500 ) ){
        return 8;
    }
    else return 0;
}

void spi_init()
{
    esp_err_t ret;

    spi_bus_config_t buscfg={
        .miso_io_num = MISO_PIN,
        .mosi_io_num = -1,
        .sclk_io_num = CLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };

    ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    spi_device_interface_config_t devcfg={
        .clock_speed_hz = 1000000,  // 1 kHz
        .mode = 0,                  //SPI mode 0
        .spics_io_num = CS_PIN,     
        .queue_size = 1,
        .flags = SPI_DEVICE_HALFDUPLEX,
        .pre_cb = NULL,
        .post_cb = NULL,
    };

    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi2));
};

static void write_reg()
{

    spi_transaction_t t = {
        .tx_buffer = NULL,
        .rx_buffer = rx_data,
        .rxlength = 2*8,
    };

    ESP_ERROR_CHECK(spi_device_polling_transmit(spi2, &t));
}

float get_ruido()
{
    
    int max = 0;
    int max_2 = 0;
    int max_raw = 0;
    int actual = 0;
    int actual_2 = 0;
    int cont = 0;
    int ruido_act = 0;
    int actual_raw = 0;

    int valor_media = 0;
    int contador_media = 0;

    while(cont < (1000)){
        if (cont % 100 == 0 && cont > 1)
        {
            printf("\nRUIDO_MAX : %d", max_2);
            printf("\nRUIDO_ACTUAL : %d", actual_raw);
            valor_media += max_2;
            contador_media +=1;
            max_2 = 0;
        }
        
        write_reg();
        actual_raw = (rx_data[0] << 8  | rx_data[1]);

        if(actual_raw > max_raw){
            max_raw = actual_raw;
        } 
        
        
        ruido_act = lvl_ruido(actual_raw);


        if(ruido_act > max){
            max = ruido_act;
        } 

        if(ruido_act > max_2){
            max_2 = ruido_act;
        } 

        cont++;
        vTaskDelay(1);
    }
    ESP_LOGI("SPI", "RETURN Media: %f", (float)((float)valor_media/(float)contador_media));
    return (float)((float)valor_media/(float)contador_media);

}