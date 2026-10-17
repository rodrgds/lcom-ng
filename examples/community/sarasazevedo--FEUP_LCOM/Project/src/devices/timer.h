/**
 * @file timer.h
 * @brief Data structures and functions for timer functions.
 */



#ifndef _TIMER_H_
#define _TIMER_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "utils.c"

/** @defgroup timer timer
 * @{
 *
 * @brief timer related functions
 */

/**
 * @brief Sets the frequency of the specified timer.
 * 
 * @param timer Timer to configure (0, 1, or 2).
 * @param freq Frequency to set for the timer.
 * @return 0 on success, 1 on failure.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes to timer interrupts.
 * 
 * @param bit_no Address of memory to be initialized with the bit mask.
 * @return 0 on success, 1 on failure.
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from timer interrupt
 * 
 * @return 0 on success, 1 on failure.
 */
int (timer_unsubscribe_int)();

/**
 * @brief Timer interrupt handler. Increments the timer interrupt counter.
 */
void (timer_int_handler)();

/**
 * @brief Gets the configuration of the specified timer.
 * 
 * @param timer Timer to get the configuration from (0, 1, or 2).
 * @param st Address of memory to be initialized with the timer configuration.
 * @return 0 on success, 1 on failure.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays the configuration of the specified timer.
 * 
 * @param timer Timer whose configuration to display (0, 1, or 2).
 * @param st Timer configuration.
 * @param field Field of the timer configuration to display.
 * @return 0 on success, 1 on failure.
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

/**@} end of timer*/

#endif /* _TIMER_H_ */
