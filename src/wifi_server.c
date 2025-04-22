#include "wifi_server.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include "lwip/ip4_addr.h"
#include "OLED.h"
#include "LCD.h"
#include "USS.h"
#include <stdio.h>
#include "pico/stdlib.h"

// Update these with your Wi-Fi credentials
#define WIFI_SSID "" //put username here
#define WIFI_PASSWORD "" //Put password here
#define ENDPOINT_GPIO_ON "/gpio/on"
#define ENDPOINT_GPIO_OFF "/gpio/off"

static volatile bool activate_flag = false;
static volatile bool deactivate_flag = false;
static bool measuring = false;
static bool oled_sleep_pending = false;
static absolute_time_t sleep_start_time;

const char *html_body =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "  <title>Pico W Device Control</title>"
    "  <style>"
    "    body { font-size: 24px; }"
    "    h2 { font-size: 32px; }"
    "    button { font-size: 18px; padding: 10px; }"
    "  </style>"
    "</head>"
    "<body>"
    "  <h2>Device Control</h2>"
    "<a href=\"" ENDPOINT_GPIO_ON "\"><button>Turn On</button></a>"
    "<a href=\"" ENDPOINT_GPIO_OFF "\"><button>Turn Off</button></a>"
    "</body>"
    "</html>";

const char *oled_on_msg = "Online and Active";
const char *oled_off_msg = "Going to Sleep, Goodnight";

static err_t http_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (!p)
    {
        tcp_close(tpcb);
        return ERR_OK;
    }

    tcp_recved(tpcb, p->tot_len);
    char *payload = (char *)p->payload;

    if (strstr(payload, ENDPOINT_GPIO_ON))
    {
        activate_flag = true;
        deactivate_flag = false; // ensure OFF is cleared
    }
    else if (strstr(payload, ENDPOINT_GPIO_OFF))
    {
        deactivate_flag = true;
        activate_flag = false; // stop measuring immediately
    }

    char response[512];
    int len = sprintf(response,
                      "HTTP/1.1 200 OK\r\n"
                      "Content-Type: text/html\r\n"
                      "Content-Length: %d\r\n"
                      "Connection: close\r\n\r\n%s",
                      (int)strlen(html_body), html_body);

    tcp_write(tpcb, response, len, TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);
    pbuf_free(p);
    tcp_close(tpcb);
    return ERR_OK;
}

static err_t http_accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    tcp_recv(newpcb, http_recv_callback);
    return ERR_OK;
}

static void start_http_server(void)
{
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb || tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK)
    {
        printf("Failed to bind HTTP server\n");
        return;
    }

    tcp_accept(tcp_listen(pcb), http_accept_callback);
    printf("HTTP server started. Listening on port 80...\n");
}

void start_wifi_server(void)
{
    if (cyw43_arch_init())
    {
        printf("Wi-Fi init failed\n");
        return;
    }

    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi-Fi...\n");

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("Failed to connect to Wi-Fi\n");
        return;
    }

    const ip4_addr_t *ip = netif_ip4_addr(netif_default);
    if (ip)
    {
        printf("Connected with IP: %s\n", ip4addr_ntoa(ip));
    }

    start_http_server();
}

void wifi_task_handler(void)
{
    if (activate_flag)
    {
        prepare_for_data();
        print_string(oled_on_msg, 0);
        measuring = true;   // Start continuous updates
        activate_flag = false;
    }

    if (deactivate_flag)
    {
        measuring = false;  // Stop measuring
        
        lcd_backlight_off();
        prepare_for_data();
        print_string(oled_off_msg, 0);
        sleep_ms(1000);
        prepare_for_data();
        deactivate_flag = false;
    }

    if (measuring)
    {
        float d = distance();
        lcd_display_distance(d);
        sleep_ms(500); // Small delay to avoid flooding
    }
}
