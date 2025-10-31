#include "dht22.h"
#include "esp_err.h"
#include "esp_event.h"
#include "wifi.h"
#include "http.h"
#include "http_client.h"
#include "esp_mac.h"

static const char *TAG = "main";
void get_mac(char *macId);
char mac_addr[50];

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

    get_mac(mac_addr);

    // wait 2 seconds for sensor to initialise
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    float last_t = 0;
    float last_h = 0;

    while (1)
    {
        get_data();
        float t = get_temperature();
        float h = get_humidity();
        
        // don't need to post the data again if it hasn't changed
        if (t != last_t || h != last_h) {
            char post_data[256];
            snprintf(post_data, sizeof(post_data), "{\"deviceId\": \"%s\",\"temperature\": %.2f,\"humidity\": %.2f}", mac_addr, t, h);
            send_post(post_data);

            last_t = t;
            last_h = h;
        }


        vTaskDelay(60000 / portTICK_PERIOD_MS);
    }
    
}

void get_mac(char *macId) {
    esp_err_t ret = ESP_OK;
    uint8_t base_mac_addr[6];
    ret = esp_efuse_mac_get_default(base_mac_addr);

    if(ret != ESP_OK){
        ESP_LOGE(TAG, "Failed to get base MAC address from EFUSE BLK0. (%s)", esp_err_to_name(ret));
        ESP_LOGE(TAG, "Aborting");
        abort();
    }

    static const char *TAG = "extract_mac";

    uint8_t index = 0;

    for(uint8_t i=0; i<6; i++){
       index += sprintf(&macId[index], "%02x", base_mac_addr[i]);
            }
    ESP_LOGI(TAG, "macId = %s", macId);
}
