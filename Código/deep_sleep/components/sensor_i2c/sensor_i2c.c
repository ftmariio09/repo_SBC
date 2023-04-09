#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sensor_i2c.h"
static const char *TAG = "i2c-example";
#define I2C_SLAVE_ADDR	0x10
#define VEML7700_ALS_ADDR 0x04
#define TIMEOUT_MS		1000
#define DELAY_MS		1000


esp_err_t leer_THR(uint8_t *data_read){

    esp_err_t espRc;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
	i2c_master_write_byte(cmd, 0x88, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, 0x24, I2C_MASTER_ACK);
    i2c_master_write_byte(cmd, 0x0B, I2C_MASTER_ACK);


    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x89, I2C_MASTER_ACK);
    //temperatura
    i2c_master_read_byte(cmd, &data_read[0],  I2C_MASTER_ACK);
	i2c_master_read_byte(cmd, &data_read[1],  I2C_MASTER_ACK);
    
    i2c_master_read_byte(cmd, &data_read[2],  I2C_MASTER_ACK);
    //humedad
    i2c_master_read_byte(cmd, &data_read[3],  I2C_MASTER_ACK);
	i2c_master_read_byte(cmd, &data_read[4],  I2C_MASTER_ACK);

    i2c_master_read_byte(cmd, &data_read[5],  I2C_MASTER_NACK);

    i2c_master_stop(cmd);

    espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000);

	i2c_cmd_link_delete(cmd);

    return espRc;
}

float convertir_Temp(uint16_t tempura){
    float aux = tempura;
    return ((0-45.0) + 175.0 * (aux/65535));
}

float convertir_HR(uint16_t wet){
    float aux = wet;
    return (100.0 * (aux/65535));
}

esp_err_t config_luz(){
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
esp_err_t leer_luz(uint8_t *data_read){

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

float convertir_Luz(uint16_t luz){

	float aux = luz;

	return aux*0.0576;
}

esp_err_t configurar_i2c(){
    i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = 21,
		.scl_io_num = 22,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = 100000,
	};

    esp_err_t error;

    i2c_param_config(I2C_NUM_0, &conf);

	error = i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    ESP_ERROR_CHECK(error);

	config_luz();

    return error;
}

float get_luz()
{
    //uint8_t Lux_data[2];

    //leer_luz(Lux_data);

    //return convertir_Luz(Lux_data[0] | (Lux_data[1] << 8));
	return 200;
}

float get_HR()
{
	/*
    uint8_t THR_data[6];

    leer_THR(THR_data);

    return convertir_HR(THR_data[3] << 8| (THR_data[4]) );
	*/
	return 50;
}

float get_temp()
{
	/*
    uint8_t THR_data[6];

    leer_THR(THR_data);

    return convertir_Temp(THR_data[0] << 8  | (THR_data[1] ));
	*/
	return 22;
}