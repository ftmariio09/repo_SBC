idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES sensor_i2c driver conexion_wifi thingsboard sensor_SPI ota tiempo esp_event esp_wifi nvs_flash lwip)
