#ifndef NINJIX_TIMER_H
#define NINJIX_TIMER_H

#include "ninjix_platform.h"

#include <lcom/i8254.h>

int timer_set_frequency(uint8_t timer, uint32_t freq);
int timer_subscribe_int(uint8_t *bit_no);
int timer_unsubscribe_int(void);
void timer_int_handler(void);
int timer_get_conf(uint8_t timer, uint8_t *st);
uint32_t timer_get_counter(void);
void timer_set_counter(uint32_t value);
int timer_calc_ticks_per_event(uint32_t timer_hz, uint32_t event_hz, uint32_t *ticks);

#endif
