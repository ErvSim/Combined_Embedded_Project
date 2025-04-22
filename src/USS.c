#include "USS.h"
#include <stdio.h>
#include <stdlib.h>

static volatile bool interrupt_flag = false;
static volatile absolute_time_t start_time;
static volatile absolute_time_t end_time;
static volatile uint total_time;
static volatile float total_distance;
static uint trigger_pin_global;
static uint echo_pin_global;


void USS_init(uint trigger_pin, uint echo_pin);
void callback(uint pin, uint32_t event);
float distance(void);

void USS_init(uint trigger_pin, uint echo_pin)
{
    trigger_pin_global = trigger_pin;
    echo_pin_global = echo_pin;

    gpio_init(trigger_pin);
    gpio_init(echo_pin);
    gpio_set_dir(trigger_pin, GPIO_OUT);
    gpio_set_dir(echo_pin, GPIO_IN);

    gpio_set_irq_enabled_with_callback(echo_pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, callback);
}

void callback(uint pin, uint32_t event)
{
    if (pin == echo_pin_global)
    {
        if (event & GPIO_IRQ_EDGE_RISE)
        {
            start_time = get_absolute_time();
        }
        else if (event & GPIO_IRQ_EDGE_FALL)
        {
            end_time = get_absolute_time();
            total_time = absolute_time_diff_us(start_time, end_time);
            total_distance = total_time / 148.0f;
            interrupt_flag = true;
        }
    }
}

float distance(void)
{
    gpio_put(trigger_pin_global, 1);
    sleep_us(10);
    gpio_put(trigger_pin_global, 0);
    sleep_us(20);

    while (!interrupt_flag)
    {
        tight_loop_contents(); // Let other cores or tasks run
    }
    interrupt_flag = false;
    return total_distance;
}