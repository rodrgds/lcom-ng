/**
 * @file timer.h
 * @brief Timer module
 */

#ifndef TIMER_H
#define TIMER_H

#include <lcom/lcf.h>
#include <stdint.h>

/**
 * @brief Sets the frequency of the specified timer.
 * @param timer Timer to configure (0, 1, or 2).
 * @param freq Desired frequency in Hz.
 * @return 0 on success, non-zero otherwise.
 */
int(timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes timer interrupts.
 * @param bit_no Pointer to variable to store the interrupt bit number.
 * @return 0 on success, non-zero otherwise.
 */
int(timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes timer interrupts.
 * @return 0 on success, non-zero otherwise.
 */
int(timer_unsubscribe_int)();

/**
 * @brief Timer interrupt handler. Increments the timer counter.
 */
void(timer_int_handler)();

/**
 * @brief Gets the configuration of the specified timer.
 * @param timer Timer to query (0, 1, or 2).
 * @param st Pointer to variable to store the status byte.
 * @return 0 on success, non-zero otherwise.
 */
int(timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays the configuration of the specified timer.
 * @param timer Timer to display (0, 1, or 2).
 * @param st Status byte.
 * @param field Field of the status to display.
 * @return 0 on success, non-zero otherwise.
 */
int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

#endif
