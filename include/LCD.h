#ifndef LCD_H
#define LCD_H

#include "pico/stdlib.h"

void lcd_init(void);
void lcd_display_distance(float distance_value);
void lcd_backlight_off(void);

#endif