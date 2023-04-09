idf_component_register(SRCS "thingsboard.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver esp_event esp_wifi mqtt json)
