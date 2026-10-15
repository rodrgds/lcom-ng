#ifndef __PROJ_TIMER_H
#define __PROJ_TIMER_H

#include <stdint.h>

#include "controller/timer/i8254.h"

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @brief Gets the current value of the timer counter
 *
 * Retrieves the number of timer interrupts that have occurred since
 * the last reset or since the program started.
 *
 * @return The current value of the timer counter in seconds
 */
uint32_t(timer_get_counter)(void);

/**
 * @brief Resets the timer counter to zero
 *
 * Sets the global counter variable back to zero, effectively resetting the timer count.
 */
void(timer_reset)(void);

/**
 * @brief Changes the operating frequency of a timer
 *
 * Must use the read-back command so that it does not change
 * the 4 LSBs (mode and BCD/binary) of the timer's control word.
 *
 * @param timer Timer to configure. (Ranges from 0 to 2)
 * @param freq Timer operating frequency
 * @return Return 0 upon success and non-zero otherwise
 */
int(timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int(timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(timer_unsubscribe_int)(void);

/**
 * @brief Timer 0 interrupt handler
 *
 * Increments counter
 */
void(timer_int_handler)(void);

/**
 * @brief Reads the input timer configuration (status) via read-back command
 *
 * @param timer Timer whose configuration to read (Ranges from 0 to 2)
 * @param st    Address of memory position to be filled with the timer config
 * @return Return 0 upon success and non-zero otherwise
 */
int(timer_get_conf)(uint8_t timer, uint8_t *st);

/**@}*/

#endif /* __PROJ_TIMER_H */
