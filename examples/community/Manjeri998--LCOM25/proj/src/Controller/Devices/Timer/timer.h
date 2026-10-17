#ifndef _TIMER_H_
#define _TIMER_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "../macros.h"
#include "../func_aux.h"

int (timer_subscribe_int)(uint8_t *bit_no);
int (timer_unsubscribe_int)();
int (timer_set_frequency)(uint8_t timer, uint32_t freq);
int (timer_get_conf)(uint8_t timer, uint8_t *st);
void (timer_int_handler)();

#endif
