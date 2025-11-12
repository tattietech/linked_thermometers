#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <rom/ets_sys.h>
#include "esp_log.h"


float get_temperature();
float get_humidity();
bool get_data();
