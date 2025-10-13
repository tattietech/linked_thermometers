idf_component_register(
    SRCS "dht22.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES esp_driver_gpio
)

#idf_component_register(
#   SRCS "DHT22.c"
#   INCLUDE_DIRS "include"
#   PRIV_REQUIRES esp_driver_gpio
#)
