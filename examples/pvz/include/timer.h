#ifndef PVZ_TIMER_H
#define PVZ_TIMER_H

#include "pvz_platform.h"

#include <lcom/i8254.h>

int timer_set_frequency(uint8_t timer, uint32_t freq);
int timer_subscribe_int(uint8_t *bit_no);
int timer_unsubscribe_int(void);
void timer_int_handler(void);
uint32_t get_timer_counter(void);
void reset_timer_counter(void);

#endif
