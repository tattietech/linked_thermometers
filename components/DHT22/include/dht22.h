#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <rom/ets_sys.h>

#define DHT_GPIO GPIO_NUM_4

float get_temperature();
float get_humidity();
void get_data();
