idf_component_register(SRCS "ota.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver esp_event esp_wifi nvs_flash app_update esp_http_client esp_https_ota
                    EMBED_TXTFILES ${project_dir}/components/ota/server_certs/ca_cert.pem)
