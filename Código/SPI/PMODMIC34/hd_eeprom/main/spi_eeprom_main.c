#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"

#define CLK_PIN     18
#define MISO_PIN    19
#define CS_PIN      5


spi_device_handle_t spi2;
uint8_t rx_data[2];

int lvl_ruido(uint16_t pepino){

     if( pepino < 1500){ 
        return 1;
     }
     else{
        if(pepino < 1900){
            return 2;
        }
        else if(pepino < 2200){
            return 3;
        }
        else if(pepino < 2800){
            return 4;
        }
        else{
            return 5;
        }
     }
}

static void spi_init() {
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

static void write_reg() {

    spi_transaction_t t = {
        .tx_buffer = NULL,
        .rx_buffer = rx_data,
        .rxlength = 2 * 8
    };

    ESP_ERROR_CHECK(spi_device_polling_transmit(spi2, &t));
}


void app_main(void)
{
    spi_init();
    while(1){
          write_reg();
            printf("%d\n",lvl_ruido(rx_data[1] | rx_data[0] << 8 ) );

            vTaskDelay(1000/portTICK_PERIOD_MS);
    }
        
    }

