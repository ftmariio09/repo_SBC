#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
static const char *TAG = "i2c-example";


static esp_err_t leer_THR(uint8_t *data_read){


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

void app_main() {
	uint8_t rx_data[6];
	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = 21,
		.scl_io_num = 22,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = 100000,
	};

	i2c_param_config(I2C_NUM_0, &conf);

	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
	
	while (1) {
		leer_THR(rx_data);
		ESP_LOGI(TAG," \n Temp: %.2f - Hum: %.2f \n crcT: %u crcH: %u \n RawT: %u RawH: %u",convertir_Temp(rx_data[0] << 8  | (rx_data[1] )),convertir_HR(rx_data[3] << 8| (rx_data[4]) ),(rx_data[2]),(rx_data[5]),(rx_data[0] << 8 | (rx_data[1])),(rx_data[3] << 8 | (rx_data[4])) );
		vTaskDelay(1000);
	}
}