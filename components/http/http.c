#include "http.h"

//Settings
static const uint32_t sleep_time_ms = 5000;
#define RX_BUF_SIZE 64

//Server settings and URL to fetch
#define WEB_HOST "httpbin.org"
#define WEB_PORT "80"
#define WEB_PATH "/post"
#define CONTENT_TYPE "application/json"
#define REQUEST_BODY "{\"temperature\": 30}"
#define USER_AGENT "esp-idf/1.0 esp32"

//HTTP POST request
// static const char *REQUEST = "POST " WEB_PATH " HTTP/1.0\r\n"
//     "Host: " WEB_HOST ":" WEB_PORT "\r\n"
//     "User-Agent: esp-idf/1.0 esp32\r\n"
//     "Content-Type: " CONTENT_TYPE "\r\n"
//     "Content-Length: 19\r\n"
//     "\r\n"
//     REQUEST_BODY
;

//Set timeouts
#define CONNECTION_TIMEOUT_SEC 10 // Set delay to wait for connection (sec)
#define SOCKET_TIMEOUT_SEC 5 // Set socket tineout (sec)

//tag for debug messages0
static const char *TAG = "http_request";

int send_post_request(const char *json_body) {
    int ret; // socket return code
    struct addrinfo *dns_res; // store result of dns lookup
    int sock; // sock handle
    char recv_buf[RX_BUF_SIZE]; // receive buffer
    ssize_t recv_len; // amount of bytes we get at a time per chunk
    char request[512];
   

    // dns hints - we want to stream the data and us IPv4
    struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };

    // Socket timout
    struct timeval sock_timeout = {
        .tv_sec = SOCKET_TIMEOUT_SEC,
        .tv_usec = 0,
    };

    // Welcome message
    ESP_LOGI(TAG, "Starting HTTP POST request");

    // Perform DNS lookup
    ret = getaddrinfo(WEB_HOST, WEB_PORT, &hints, &dns_res);
    if (ret != 0 || dns_res == NULL) {
        ESP_LOGE(TAG, "Error (%d): DNS lookup failed", ret);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        return -1;
    }

    // print resolve IP addresses (we will just use the first)
    ESP_LOGI(TAG, "DNS lookup succeeded. IP addresses: ");
    for(struct addrinfo *addr = dns_res; addr != NULL; addr = addr->ai_next) {
        if(addr->ai_family == AF_INET) {
            struct in_addr *ip = &((struct sockaddr_in *)addr->ai_addr)->sin_addr;
            inet_ntop(AF_INET, ip, recv_buf, INET_ADDRSTRLEN);
            ESP_LOGI(TAG, " IPv4: %s", recv_buf);
        }
        else if(addr->ai_family == AF_INET6) {
            struct in6_addr *ip = &((struct sockaddr_in6 *)addr->ai_addr)->sin6_addr;
            inet_ntop(AF_INET6, ip, recv_buf, INET6_ADDRSTRLEN);
            ESP_LOGI(TAG, "IPv6: %s", recv_buf);
        }
    }

    // Create a socket
    sock = socket(dns_res->ai_family, dns_res->ai_socktype, dns_res->ai_protocol);
    if (sock < 0) {
        ESP_LOGE(TAG, "Error (%d): Failed to create socket: %s", errno, strerror(errno));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        return -1;
    }

    // Set socket send timeouts
    ret = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &sock_timeout, sizeof(sock_timeout));
    if (ret < 0) {
        ESP_LOGE(TAG,
                 "Error (%d): Failed to set socket send timeout: %s",
                 errno,
                 strerror(errno));
        close(sock);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        return -1;
    }

    // Set socket send timeouts
    ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &sock_timeout, sizeof(sock_timeout));
    if (ret < 0) {
        ESP_LOGE(TAG,
                 "Error (%d): Failed to set socket receive timeout: %s",
                 errno,
                 strerror(errno));
        close(sock);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        return -1;
    }

    // Conec to Server
    ret = connect(sock, dns_res->ai_addr, dns_res->ai_addrlen);
    if (ret < 0) {
        ESP_LOGE(TAG, "Error (%d): Failed to connect to server: %s", errno, strerror(errno));
        close(sock);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        return -1;
    }

    // Delete the address info
    freeaddrinfo(dns_res);

    // Build request dynamically
    int body_len = strlen(json_body);
    int req_len = snprintf(request, sizeof(request),
        "POST %s HTTP/1.0\r\n"
        "Host: %s:%s\r\n"
        "User-Agent: %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s",
        WEB_PATH, WEB_HOST, WEB_PORT, USER_AGENT,
        CONTENT_TYPE, body_len, json_body);

    if (req_len >= sizeof(request)) {
        printf("Request buffer too small\n");
        return -1;
    }

    // Send HTTP POST request
    ESP_LOGI(TAG, "Sending HTTP POST request...");
    ret = send(sock, request, req_len, 0);
    if (ret < 0) {
        ESP_LOGE(TAG, "Error (%d): Failed to send HTTP GET request: %s", errno, strerror(errno));
        close(sock);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        return -1;
    }

    // Print the HTTP response
    ESP_LOGI(TAG, "HTTP response:");
    while(1) {
        // Receive data from the socket
        recv_len = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0);

        // Check for errors
        if (recv_len < 0) {
            ESP_LOGE(TAG, "Error (%d): Failed to receive data: %s", errno, strerror(errno));
            break;
        }

        // Check for end of data
        if (recv_len == 0) {
            break;
        }

        // Null-terminate the received data and print it
        recv_buf[recv_len] = '\0';
        printf("%s", recv_buf);
    }
        // Close the socket
        close(sock);

        vTaskDelay(sleep_time_ms / portTICK_PERIOD_MS);

    return 0;
}
