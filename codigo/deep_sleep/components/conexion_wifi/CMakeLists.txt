idf_component_register(SRCS "conexion_wifi.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver esp_event esp_wifi nvs_flash)
