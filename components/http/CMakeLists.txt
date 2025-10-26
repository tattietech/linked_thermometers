idf_component_register(
    SRCS "http.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES esp_http_client nvs_flash esp_netif esp-tls
)
