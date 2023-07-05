#include <stdio.h>
#include "thingsboard.h"
#include "mqtt_client.h"
#include "cJSON.h"
#include "esp_event.h"
#include "esp_log.h"

static const char *TAG = "THINGSBOARD";

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%ld", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}



int enviar_datos(int64_t time_stamps[NUM_OF_DATA], float luz[NUM_OF_DATA], float temp[NUM_OF_DATA], float HR[NUM_OF_DATA], float CO2[NUM_OF_DATA])
{
    int exito = 0;
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_THINGSBOARD_MQTT_URI, 
        //.event_handle = mqtt_event_handler,
        //.broker.address.port = 1883,
        .credentials.username = CONFIG_THINGSBOARD_MQTT_USERNAME,
        .credentials.client_id = CONFIG_THINGSBOARD_MQTT_CLIENT_ID,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);

    

    for (int i = 0; i < NUM_OF_DATA; i++)
    {
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "ts", time_stamps[i]);

        cJSON *values = cJSON_CreateObject();

        cJSON_AddItemToObject(root, "values", values);

        cJSON_AddNumberToObject(values, "Lux", luz[i]);
        cJSON_AddNumberToObject(values, "Temperatura", temp[i]);
        cJSON_AddNumberToObject(values, "Humedad_Relativa", HR[i]);
        cJSON_AddNumberToObject(values, "CO2", CO2[i]);


        char *post_data = cJSON_PrintUnformatted(root);
        printf("\n");
        printf(post_data);
        printf("\n");
        exito = esp_mqtt_client_publish(client, "v1/devices/me/telemetry", post_data, 0, 1, 0);
        printf("EXITO: %d\n", exito);
        vTaskDelay(200 / portTICK_PERIOD_MS);

        cJSON_Delete(root);
    }
    
    esp_mqtt_client_stop(client);
    

    return exito;
}
