#define NUM_OF_DATA CONFIG_NUM_OF_DATA
#include "esp_event.h"


static void log_error_if_nonzero(const char *message, int error_code);


static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);



int enviar_datos(int64_t time_stamps[NUM_OF_DATA], float luz[NUM_OF_DATA], float temp[NUM_OF_DATA], float HR[NUM_OF_DATA], float CO2[NUM_OF_DATA], float ruido[NUM_OF_DATA]);
