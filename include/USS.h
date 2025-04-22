#ifndef USS_H
#define USS_H


#include "pico/stdlib.h"

void USS_init(uint trigger_pin, uint echo_pin);
float distance(void);

#endif