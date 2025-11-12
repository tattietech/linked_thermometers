#include "dht22.h"

#define DHT_GPIO GPIO_NUM_4

float temperature = 0;
float humidity = 0;
int get_signal(int usTimeout, bool state);


bool get_data()
{
    int us_timeout = 0;
    uint8_t data[5] = {0};

    gpio_set_direction(DHT_GPIO, GPIO_MODE_OUTPUT);

    // start signal, pull low for 3 ms
    gpio_set_level(DHT_GPIO, 0);
    ets_delay_us(3000);

    // pull up for 25 us to request data
    gpio_set_level(DHT_GPIO, 1);
    ets_delay_us(25);

    // switch to input to read sensor response
    gpio_set_direction(DHT_GPIO, GPIO_MODE_INPUT);

    // --- DHT sensor will keep the line low for 80us then high for 80us --- //

    // low
    us_timeout = get_signal(85, 0);
    if (us_timeout < 0) return false;

    // high
    us_timeout = get_signal(85, 1);
    if (us_timeout < 0) return false;


    // read 40 bits
    for (int i = 0; i < 40; i++)
    {
        // wait for >50us low signal to start transmission
        us_timeout = get_signal(56, 0);
        if (us_timeout < 0) return false;
        
        // measure high pulse in us
        us_timeout = get_signal(75, 1);
        if (us_timeout < 0) return false;

         uint8_t bit = (us_timeout > 40) ? 1 : 0;

        // store the bit in the correct byte
        data[i / 8] <<= 1;  // shift current byte left
        data[i / 8] |= bit; // OR in the new bit
    }
    
    // check sum to make sure we return no bad readings
    int checksum = (data[0] + data[1] + data[2] + data[3]) & 0xFF;

    if (checksum == data[4]) {
        humidity = (data[0] << 8) + data[1];
        temperature = (data[2] << 8) + data[3];

        humidity /= 10;
        temperature /= 10;

        return true;
    }
    
    ESP_LOGE("DHT", "Checksum failed, data: %d, %d, %d, %d  checksum: %d",
             data[0], data[1], data[2], data[3], data[4]);
    return false;
}

// check if sensor is sending high or low signal with a timeout
int get_signal(int usTimeout, bool state)
{
    int uSec = 0;

    while (gpio_get_level(DHT_GPIO) == state)
    {
        if (uSec > usTimeout) return false;

        uSec++;
        ets_delay_us(1);
    }

    return uSec;
}

float get_temperature()
{
    return temperature;
}

float get_humidity()
{
    return humidity;
}


