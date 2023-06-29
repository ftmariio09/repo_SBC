
#define I2C_SLAVE_ADDR	0x10
#define VEML7700_ALS_ADDR 0x04
#define TIMEOUT_MS		1000
#define DELAY_MS		1000

esp_err_t leer_THR(uint8_t *data_read);

float convertir_Temp(uint16_t tempura);

float convertir_HR(uint16_t wet);

esp_err_t leer_luz(uint8_t *data_read);

float convertir_Luz(uint16_t luz);

esp_err_t configurar_i2c();

float get_luz();
float get_humedad();
float get_temperatura();
float get_carbono();