#include "dht22.h"

#include "sdkconfig.h"

#define WIFI_SSID CONFIG_WIFI_SSID
#define WIFI_PASS CONFIG_WIFI_PASSWORD

wifi_config_t wifi_config = {
    .sta = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
    },
};

void app_main(void)
{
    // wait 2 seconds for sensor to initialise
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    while (1)
    {
        get_data();

        float t = get_temperature();
        float h = get_humidity();
        printf("\033[2F");   // Move cursor up 2 lines
        printf("\033[2K");   // Clear line

        printf("temp - %0.2f°C\n", t);

        printf("\033[2K"); // clear line
        printf("humidity - %0.2f%%\n", h);

        fflush(stdout);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
