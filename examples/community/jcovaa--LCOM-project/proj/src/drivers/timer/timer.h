/**
 * @file timer.h
 * @brief This file contains the function prototypes for the timer driver. It includes functions to manage timer interrupts, configure timers, and display timer configurations.
 */

#ifndef __TIMER_H
#define __TIMER_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief subscribes to timer interrupts.
 * 
 * @param bit_no Pointer to a variable where the bit number will be stored.
 * @return 0 on success, non-zero otherwise.
 */
int(timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief unsubscribes from timer interrupts.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int(timer_unsubscribe_int)();

/**
 * @brief Timer interrupt handler. Increments the timer counter.
 */
void(timer_int_handler)();

/**
 * @brief Gets the configuration of a timer.
 * 
 * @param timer Timer to get configuration from (0, 1, or 2).
 * @param st Pointer to variable to store the status byte.
 * @return 0 on success, non-zero otherwise.
 */
int(timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays the configuration of a timer.
 * 
 * @param timer Timer to display configuration for (0, 1, or 2).
 * @param st Status byte to interrupt.
 * @param field Field of the status byte to display.
 * @return 0 on success, non-zero otherwise.
 */

#endif /* __TIMER_H */
