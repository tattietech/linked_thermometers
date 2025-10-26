#include "esp_err.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "portmacro.h"
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

int send_post_request(const char *json_body);
