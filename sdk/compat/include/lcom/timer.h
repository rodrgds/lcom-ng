#ifndef LCOM_NG_FORWARD_LCOM_TIMER
#define LCOM_NG_FORWARD_LCOM_TIMER
/* The course SDK uses this guard. Local copies of that same header must not
 * redeclare its public enum and union types after <lcom/lcf.h>. */
#ifndef __TIMER_H
#define __TIMER_H
#endif
#include <lcom/compat.h>
int timer_set_frequency(uint8_t timer, uint32_t freq);
int timer_subscribe_int(uint8_t *bit_no);
int timer_unsubscribe_int(void);
void timer_int_handler(void);
int timer_get_conf(uint8_t timer, uint8_t *status);
int timer_display_conf(uint8_t timer, uint8_t status, enum timer_status_field field);
#endif
