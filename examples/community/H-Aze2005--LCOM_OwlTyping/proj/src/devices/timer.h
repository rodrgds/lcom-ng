/**
 * @file timer.h
 * @brief Timer device interface and interrupt handling.
 *
 * Declares functions and variables for subscribing to, unsubscribing from,
 * and handling timer interrupts, as well as managing the timer counter.
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8254.h"

/**
 * @brief Global counter incremented on each timer interrupt.
 */
int timer_counter;

/**
 * @brief Subscribes timer interrupts.
 * @param bit_no Address of variable to store the hook id bit number.
 * @return 0 on success, non-zero otherwise.
 */
int (subscribe_timer)(uint8_t *bit_no);

/**
 * @brief Unsubscribes timer interrupts.
 * @return 0 on success, non-zero otherwise.
 */
int (unsubscribe_timer)();

/**
 * @brief Timer interrupt handler.
 */
void (timer_ih)();

#endif
