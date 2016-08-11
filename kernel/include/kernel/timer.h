#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
#include <stddef.h>

void timer_init(uint32_t freq);
uint32_t timer_get_tick();

#endif