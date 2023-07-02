#include "driver/i2c.h"
static const char *TAG = "i2c-example";
#define I2C_SLAVE_ADDR	0x10
#define VEML7700_ALS_ADDR 0x04
#define TIMEOUT_MS		1000
#define DELAY_MS		1000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "sdkconfig.h"
#include "esp_log.h"

#define CLK_PIN     18
#define MISO_PIN    19
#define CS_PIN      5

uint8_t Lux_data[2];
uint8_t THR_data[6];
uint8_t CO2_data[9];

static const char *TAG_i2c = "sensores_i2c";


i2c_config_t conf = {
	.mode = I2C_MODE_MASTER,
	.sda_io_num = 21,
	.scl_io_num = 22,
	.sda_pullup_en = GPIO_PULLUP_ENABLE,
	.scl_pullup_en = GPIO_PULLUP_ENABLE,
	.master.clk_speed = 100000,
};



static esp_err_t config_luz(){
	esp_err_t espRc;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, 0x20, I2C_MASTER_ACK);
	i2c_master_write_byte(cmd,0x00,I2C_MASTER_ACK);

	i2c_master_write_byte(cmd,0x00,I2C_MASTER_ACK);
	i2c_master_write_byte(cmd,0x00,I2C_MASTER_ACK);

	i2c_master_stop(cmd);

	espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000);

	i2c_cmd_link_delete(cmd);

	return espRc;

}

esp_err_t configurar_i2c()
{
	i2c_param_config(I2C_NUM_0, &conf);

	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));

	return config_luz();
}



static esp_err_t leer_carbono(uint8_t *data_read){

	esp_err_t espRc1;
	i2c_cmd_handle_t cmd1 = i2c_cmd_link_create();
    
	i2c_master_start(cmd1);
	i2c_master_write_byte(cmd1, 0xB5, I2C_MASTER_ACK);

	i2c_master_read_byte(cmd1, &data_read[0],  I2C_MASTER_ACK);
    i2c_master_read_byte(cmd1, &data_read[1],  I2C_MASTER_ACK);
    i2c_master_read_byte(cmd1, &data_read[2],  I2C_MASTER_ACK);
    i2c_master_read_byte(cmd1, &data_read[3],  I2C_MASTER_ACK);
    i2c_master_read_byte(cmd1, &data_read[4],  I2C_MASTER_ACK);
    i2c_master_read_byte(cmd1, &data_read[5],  I2C_MASTER_ACK);
    i2c_master_read_byte(cmd1, &data_read[6],  I2C_MASTER_ACK);
    i2c_master_read_byte(cmd1, &data_read[7],  I2C_MASTER_ACK);
	i2c_master_read_byte(cmd1, &data_read[8],  I2C_MASTER_NACK);
	i2c_master_stop(cmd1);

	espRc1 = i2c_master_cmd_begin(I2C_NUM_0, cmd1, 1000);

	i2c_cmd_link_delete(cmd1);

	return espRc1;
}

float convertir_co2(uint8_t dioxido, uint8_t carbono){
	float result = (float)dioxido * 256.0 + (float)carbono;
	return result;
}


esp_err_t leer_THR(uint8_t *data_read){


    esp_err_t espRc2;
	i2c_cmd_handle_t cmd2 = i2c_cmd_link_create();

    i2c_master_start(cmd2);
	i2c_master_write_byte(cmd2, 0x88, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd2, 0x24, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd2, 0x0B, I2C_MASTER_ACK);


    i2c_master_start(cmd2);
    i2c_master_write_byte(cmd2, 0x89, I2C_MASTER_ACK);
    //temperatura
    i2c_master_read_byte(cmd2, &data_read[0],  I2C_MASTER_ACK);
	i2c_master_read_byte(cmd2, &data_read[1],  I2C_MASTER_ACK);
    
    i2c_master_read_byte(cmd2, &data_read[2],  I2C_MASTER_ACK);
    //humedad
    i2c_master_read_byte(cmd2, &data_read[3],  I2C_MASTER_ACK);
	i2c_master_read_byte(cmd2, &data_read[4],  I2C_MASTER_ACK);

    i2c_master_read_byte(cmd2, &data_read[5],  I2C_MASTER_NACK);

    i2c_master_stop(cmd2);

    espRc2 = i2c_master_cmd_begin(I2C_NUM_0, cmd2, 1000);

	i2c_cmd_link_delete(cmd2);

    return espRc2;

}

float convertir_Temp(uint16_t tempura){
    float aux = tempura;
    return ((0-45.0) + 175.0 * (aux/65535));
}

float convertir_HR(uint16_t wet){
    float aux = wet;
    return (100.0 * (aux/65535));
}

esp_err_t leer_luz(uint8_t *data_read)
{

	esp_err_t espRc;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, 0x20, I2C_MASTER_ACK);
	i2c_master_write_byte(cmd, VEML7700_ALS_ADDR, I2C_MASTER_ACK);

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, 0x21, I2C_MASTER_ACK);

	i2c_master_read_byte(cmd, &data_read[0],  I2C_MASTER_ACK);
	i2c_master_read_byte(cmd, &data_read[1],  I2C_MASTER_NACK);
	i2c_master_stop(cmd);

	espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000);

	i2c_cmd_link_delete(cmd);

	return espRc;
}

float convertir_Luz(uint16_t luz)
{

	float aux = luz;

	return aux*0.0576;
}

float get_luz()
{
	ESP_LOGI(TAG_i2c, "Dentro get_luz()");
    return 112.0;
}

float get_temperatura()
{
	ESP_LOGI(TAG_i2c, "Dentro get_temperatura()");
    return 28.0;
}

float get_humedad()
{
	ESP_LOGI(TAG_i2c, "Dentro get_humedad()");
    return 45.0;

}

float get_carbono()
{
	ESP_LOGI(TAG_i2c, "Dentro get_carbono()");
    return 545.0;

}
