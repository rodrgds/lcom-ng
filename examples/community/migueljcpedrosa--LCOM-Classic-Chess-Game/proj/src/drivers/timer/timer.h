/**
 * @file timer
 * @brief This file has the implementation of the functions related to the timer.
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <lcom/lcf.h>

/**
 * @brief Reads a timer's configuration.
 * 
 * @param timer Timer to read the configuration of (0, 1, or 2).
 * @param status Pointer to store the timer's configuration.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Sets the operating frequency of a timer.
 * 
 * @param timer Timer to set the frequency for (0, 1, or 2).
 * @param freq Desired frequency.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Timer interrupt handler.
 * 
 * Increments a counter variable whenever a timer interrupt occurs.
 * 
 * @return void.
 */
void (timer_ih)(void);

/**
 * @brief Subscribes to timer interrupts.
 * 
 * @param bit_no Pointer to store the IRQ line bitmask of the timer.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from timer interrupts.
 * 
 * @return Returns 0 upon success and non-zero otherwise.
 */
int (timer_unsubscribe_int)();


#endif
