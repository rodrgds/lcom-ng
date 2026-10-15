#ifndef _TIMER_H_
#define _TIMER_H_


#include <lcom/lcf.h>
#include <lcom/timer.h> // Includes standard timer function declarations from LCF
#include <stdint.h>

#include "i8254.h"     // Includes i8254 timer constants

/**
 * @file timer.h
 * @brief Interface for programming the i8254 Programmable Interval Timer (PIT).
 *
 * This file declares functions for configuring timer frequency,
 * subscribing and unsubscribing timer interrupts, handling timer interrupts,
 * and reading/displaying timer configurations.
 */

/** @defgroup timer Timer
 * @{
 *
 * Functions for interacting with the i8254 Programmable Interval Timer.
 */

/**
 * @brief Configures a specified timer to generate interrupts at a desired frequency.
 *
 * Changes the operating frequency of the selected i8254 timer channel.
 * The timer must be between 0 and 2. The frequency must be at least 19 Hz
 * (approximately, as min value for divisor is 2 and max is 65535 for a 16-bit counter,
 * and TIMER_FREQ / 65535 is ~18.2).
 *
 * @param timer The timer channel to configure (0, 1, or 2).
 * @param freq The desired frequency in Hz.
 * @return int 0 upon success, non-zero otherwise (e.g., invalid timer, invalid frequency).
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes to timer 0 interrupts.
 *
 * Sets up the system to receive interrupts from timer 0. The IRQ line used by timer 0
 * will be enabled, and a hook ID will be associated with it.
 *
 * @param bit_no_out Pointer to a variable where the bit number (hook_id) for timer 0 interrupts will be stored.
 *                   This bit number should be used to check for timer interrupts in the main loop.
 * @return int 0 upon success, non-zero otherwise.
 */
int (timer_subscribe_int)(uint8_t *bit_no_out);

/**
 * @brief Unsubscribes from timer 0 interrupts.
 *
 * Disables timer 0 interrupts and removes the associated hook ID.
 *
 * @return int 0 upon success, non-zero otherwise.
 */
int (timer_unsubscribe_int)();

/**
 * @brief Timer interrupt handler.
 *
 * This function should be called when a timer 0 interrupt occurs.
 * It typically increments a global counter or performs other periodic tasks.
 */
void (timer_int_handler)();

/**
 * @brief Reads the configuration (status byte) of a specified timer.
 *
 * Uses the Read-Back command to retrieve the status byte of the selected timer.
 * The status byte contains information about the timer's current settings,
 * such as operating mode, BCD/binary counting, and output state.
 *
 * @param timer The timer channel whose configuration is to be read (0, 1, or 2).
 * @param st    Pointer to a variable where the status byte will be stored.
 * @return int 0 upon success, non-zero otherwise (e.g., invalid timer).
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays the configuration of a specified timer.
 *
 * Parses and prints the meaning of the timer's status byte (`st`) or specific fields within it.
 * The `field` parameter determines what part of the configuration is displayed.
 *
 * @param timer The timer channel whose configuration is being displayed (0, 1, or 2).
 * @param st    The status byte of the timer (previously read by `timer_get_conf`).
 * @param field An enum `timer_status_field` specifying which part of the status to display
 *              (e.g., TSF_ALL, TSF_INITIAL, TSF_MODE, TSF_BASE).
 * @return int 0 upon success, non-zero otherwise (e.g., invalid timer, invalid field).
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

/**@}*/

#endif /* _TIMER_H_ */
