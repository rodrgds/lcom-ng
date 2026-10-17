#ifndef TIMER_H
#define TIMER_H

#include <lcom/lcf.h>

extern int counter;

/**
 * @brief Subscribes timer's interrupts.
 * 
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes timer's interrupts.
 * 
 */
int (timer_unsubscribe_int)();

#endif
