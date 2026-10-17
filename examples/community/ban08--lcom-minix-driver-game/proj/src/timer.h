#ifndef _TIMER_H_
#define _TIMER_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8254.h"

/**
 * @brief Sets the frequency of the specified timer.
 *
 * Configures a timer (0, 1, or 2) to generate interrupts at a given frequency.
 * It uses LSB followed by MSB mode for writing the initial value.
 *
 * @param timer Timer number (0, 1, or 2).
 * @param freq Desired frequency in Hz.
 * @return 0 on success, 1 on failure.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes to timer interrupts.
 *
 * Registers the timer interrupt handler to receive IRQ0 interrupts.
 *
 * @param bit_no Address of variable to store the interrupt bitmask.
 * @return 0 on success, 1 on failure.
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from timer interrupts.
 *
 * Removes the interrupt policy for the timer to stop receiving IRQ0 interrupts.
 *
 * @return 0 on success, 1 on failure.
 */
int (timer_unsubscribe_int)();

/**
 * @brief Timer interrupt handler.
 *
 * Increments a global tick counter (cnt) on every interrupt.
 */
void (timer_int_handler)();

/**
 * @brief Retrieves the current configuration of a given timer.
 *
 * Sends a Read-Back command to retrieve the current configuration of a timer
 * and stores it in the provided pointer.
 *
 * @param timer Timer number (0, 1, or 2).
 * @param st Pointer to store the timer's status byte.
 * @return 0 on success, 1 on failure.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays a specific field of the timer configuration.
 *
 * Parses the timer status byte and extracts the requested field, printing it
 * using the provided timer_print_config function.
 *
 * @param timer Timer number (0, 1, or 2).
 * @param st Timer status byte.
 * @param field Field to interpret and display (tsf_all, tsf_initial, tsf_mode, tsf_base).
 * @return 0 on success, 1 on failure.
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

#endif /* _TIMER_H_ */
