#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>

#define TIMER_FREQ 50 // Timer frequency in Hz

void init_timer(uint32_t freq);

uint32_t get_ticks(void);

#endif