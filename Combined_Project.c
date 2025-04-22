#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/cyw43_arch.h"
#include "USS.h"
#include "LCD.h"
#include "OLED.h"
#include "wifi_server.h"

#define trigger 0
#define echo 1
#define SDA 16
#define SCL 17
#define I2C_PORT i2c0

int main()
{
    stdio_init_all();
    sleep_ms(1000);

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);

    USS_init(trigger,echo);
    lcd_init();
    oled_init();
    prepare_for_data();

    start_wifi_server();
    

    while (true)
    {
        wifi_task_handler();
        sleep_ms(300);
    }
}
