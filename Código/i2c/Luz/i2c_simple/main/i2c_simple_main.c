#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
static const char *TAG = "i2c-example";
#define I2C_SLAVE_ADDR	0x10
#define VEML7700_ALS_ADDR 0x04
#define TIMEOUT_MS		1000
#define DELAY_MS		1000


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
static esp_err_t leer_luz(uint8_t *data_read){

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

void app_main() {
	uint8_t rx_data[2];

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
	config_luz();
	while (1) {
		leer_luz(rx_data);
		
		ESP_LOGI(TAG,"LVL LUZ: %.2f",convertir_Luz(rx_data[0] | (rx_data[1] << 8)) );
		vTaskDelay(DELAY_MS/portTICK_RATE_MS);
	}
}