#ifndef TIMER_H
#define TIMER_H

#include <lcom/lcf.h>
#include <stdint.h>

extern int timer_counter;

/**
 * @brief Sets the operating frequency of a timer.
 *
 * This function sets the operating frequency of the specified timer.
 *
 * @param timer The timer to be configured (0, 1, or 2).
 * @param freq The desired operating frequency.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Timer interrupt handler.
 *
 * This function is the interrupt handler for timer interrupts. It processes the 
 * timer interrupt, executing any necessary actions.
 */
void (timer_int_handler)();

/**
 * @brief Subscribes to timer interrupts.
 *
 * This function subscribes to timer interrupts and enables notifications.
 *
 * @param bit_no Pointer to a variable where the interrupt bit number will be stored.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from timer interrupts.
 *
 * This function unsubscribes from timer interrupts and disables notifications.
 *
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (timer_unsubscribe_int)();

/**
 * @brief Retrieves the current configuration of a timer.
 *
 * This function retrieves the current configuration of the specified timer.
 *
 * @param timer The timer to query (0, 1, or 2).
 * @param st Pointer to a variable where the configuration will be stored.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays the current configuration of a timer.
 *
 * This function displays the current configuration of the specified timer.
 *
 * @param timer The timer to query (0, 1, or 2).
 * @param st The configuration of the timer.
 * @param field The field of the configuration to display.
 * @return Returns 0 upon success, or a non-zero error code otherwise.
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);


#endif // TIMER_H
