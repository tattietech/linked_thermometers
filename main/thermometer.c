#include "dht22.h"
#include "esp_err.h"
#include "esp_event.h"
#include "wifi.h"
#include "http.h"
static const char *TAG = "main";

void app_main(void)
{
    esp_err_t esp_ret; // esp return code
    // create default event loop that runs in the background (once in app)
    // must be running prior to initialising the network driver
    esp_ret = esp_event_loop_create_default();
    if (esp_ret != ESP_OK) {
        ESP_LOGE(TAG, "Error (%d): Failed to initialise network interface", esp_ret);
    }

    connect_wifi();

    // wait 2 seconds for sensor to initialise
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    char json_body[64];

    while (1)
    {
        get_data();

        float t = get_temperature();
        float h = get_humidity();
        // printf("\033[2F");   // Move cursor up 2 lines
        // printf("\033[2K");   // Clear line
        //
        // printf("temp - %0.2f°C\n", t);
        //
        // printf("\033[2K"); // clear line
        // printf("humidity - %0.2f%%\n", h);
        //
        // fflush(stdout);
        
        snprintf(json_body, sizeof(json_body), "{\"temperature\": %.2f,\"humidity\": %.2f}", t, h);
        send_post_request(json_body);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    
}
