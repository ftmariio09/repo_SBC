idf_component_register(SRCS "deep_sleep_example_main.c"
                    INCLUDE_DIRS "."
                    REQUIRES sensor_i2c driver conexion_wifi thingsboard sensor_SPI ota esp_event esp_wifi nvs_flash lwip)
