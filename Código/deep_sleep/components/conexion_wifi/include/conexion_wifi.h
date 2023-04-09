#include "esp_event.h"

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

int wifi_init_sta(void);

int iniciar_wifi(void);

void parar_wifi(void);

    
