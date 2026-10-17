#ifndef _LCOM_PROJ_TIMER_H_
#define _LCOM_PROJ_TIMER_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/*!
 * @brief Subscribes timer interrupts.
 *
 * Sets up the policy to receive timer interrupts, with interruption BIT 0.
 *
 * @param[out] bit_no Pointer to variable to store the interrupt bit number.
 * @return 0 on success, non-zero otherwise.
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/*!
 * @brief Unsubscribes timer interrupts.
 *
 * Removes the policy for timer interrupts.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (timer_unsubscribe_int)();

/*!
 * @brief Sets the frequency of the specified timer.
 *
 * Configures the timer to generate interrupts at the specified frequency.
 *
 * @param[in] timer Timer number (0, 1, or 2).
 * @param[in] freq Desired frequency in Hz.
 * @return 0 on success, non-zero otherwise.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/*!
 * @brief Timer interrupt handler.
 *
 * Should be called on every timer interrupt. Increments the internal timer counter.
 */
void (timer_int_handler)();

/*!
 * @brief Gets the configuration of the specified timer.
 *
 * Reads the current configuration of the specified timer.
 *
 * @param[in] timer Timer number (0, 1, or 2).
 * @param[out] st Pointer to variable to store the status byte.
 * @return 0 on success, non-zero otherwise.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/*!
 * @brief Displays the configuration of the specified timer.
 *
 * Prints the configuration of the timer in a human-readable format.
 *
 * @param[in] timer Timer number (0, 1, or 2).
 * @param[in] st Status byte of the timer.
 * @param[in] field Field of the status to display.
 * @return 0 on success, non-zero otherwise.
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

#endif
