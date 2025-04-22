#ifndef OLED_H
#define OLED_H

#include "pico/stdlib.h"

void oled_init(void);
void prepare_for_data(void);
void print_string(const char *str, uint8_t start_row);

#endif 