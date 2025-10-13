#include "dht22.h"

float temperature = 0;
float humidity = 0;

float get_temperature()
{
    return temperature;
}

float get_humidity()
{
    return humidity;
}

void get_data()
{
    gpio_set_direction(DHT_GPIO, GPIO_MODE_OUTPUT);
    // start signal, pull low for 2ms, it only needs one
    // but we'll do 2 to be safe

    gpio_set_level(DHT_GPIO, 0);
    ets_delay_us(3000);

    gpio_set_level(DHT_GPIO, 1);
    ets_delay_us(25);

    // switch to input to read sensor response
    gpio_set_direction(DHT_GPIO, GPIO_MODE_INPUT);

    // wait for sensor response (low then high)
    uint32_t timeout = 10000;
    while (gpio_get_level(DHT_GPIO) == 1 && timeout--) ets_delay_us(1);
    timeout = 10000;
    while (gpio_get_level(DHT_GPIO) == 0 && timeout--) ets_delay_us(1);
    timeout = 10000;
    while (gpio_get_level(DHT_GPIO) == 1 && timeout--) ets_delay_us(1);

    uint8_t data[5] = {0};

    // read 40 bits
    for (int i = 0; i < 40; i++)
    {
        // wait for low
        timeout = 10000;
        while (gpio_get_level(DHT_GPIO) == 0 && timeout--) ets_delay_us(1);
        
        // measure length of high pulse
        uint32_t high_ticks = 0;
        
        while (gpio_get_level(DHT_GPIO) == 1 && high_ticks < 200)
        {
            ets_delay_us(1);
            high_ticks++;
        }

         uint8_t bit = (high_ticks > 40) ? 1 : 0;

        // store the bit in the correct byte
        data[i / 8] <<= 1;  // shift current byte left
        data[i / 8] |= bit; // OR in the new bit

        //printf("%lu ", high_ticks);
       //printf("%d", (high_ticks > 40) ? 1 : 0);
    }
    
    humidity = data[0] * 256 + data[1];
    temperature = data[2] * 256 + data[3];

    humidity /= 10;
    temperature /= 10;
}


