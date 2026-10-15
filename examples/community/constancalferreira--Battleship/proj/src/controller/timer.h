/**
 * @file timer.h
 * @brief Timer device driver interface
 *
 * This file provides functions for interacting with the i8254 Programmable Interval Timer (PIT).
 * It includes functionality to subscribe to timer interrupts, configure timer operation,
 * and set timer frequencies for game timing and animation purposes.
 */

#ifndef TIMER_H
#define TIMER_H

#include <lcom/lcf.h>
#include <lcom/timer.h> 
#include <stdint.h>
#include "i8254.h"


/**
 * @brief Subscribes to timer interrupts
 * 
 * Sets up the timer interrupt service routine and enables timer interrupts.
 * 
 * @param[out] bit_no Pointer to variable that will be filled with the bit position for timer interrupts
 * @return 0 upon success, non-zero otherwise
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes timer interrupts
 * 
 * Disables timer interrupts and removes the interrupt handler.
 * 
 * @return 0 upon success, non-zero otherwise
 */
int (timer_unsubscribe_int)();

/**
 * @brief Reads the configuration from a timer
 * 
 * Uses the read-back command to retrieve the current configuration
 * of the specified timer.
 * 
 * @param timer Timer to read the configuration from (0, 1, or 2)
 * @param[out] st Pointer to variable where the configuration will be stored
 * @return 0 upon success, non-zero otherwise
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Sets the frequency of a timer
 * 
 * Programs the specified timer to generate interrupts at the given frequency.
 * Preserves the operating mode of the timer.
 * 
 * @param timer Timer to configure (0, 1, or 2)
 * @param freq Frequency in Hz (must be between 19 and TIMER_FREQ)
 * @return 0 upon success, non-zero otherwise
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

#endif
