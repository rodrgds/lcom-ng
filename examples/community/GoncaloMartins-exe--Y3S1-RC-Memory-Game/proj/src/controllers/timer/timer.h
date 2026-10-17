#ifndef TIMER_H
#define TIMER_H

#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include <stdbool.h>

#include "i8254.h"

/**
 * @brief Global counter incremented on every timer interrupt.
 */
extern uint32_t counter;

/**
 * @brief Hook ID used for timer interrupt subscription.
 */
extern int hook_id;
extern uint32_t game_seconds;
extern bool timer_running;


/**
 * @brief Configures the frequency of the specified timer.
 * 
 * @param timer Timer to configure (0 to 2).
 * @param freq Desired frequency (must be between 19 and TIMER_FREQ).
 * @return 0 on success, non-zero otherwise.
 */
int (my_timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes to timer interrupts.
 * 
 * @param bit_no Pointer to a variable to store the interrupt bit mask.
 * @return 0 on success, non-zero otherwise.
 */
int (my_timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from timer interrupts.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (my_timer_unsubscribe_int)();

/**
 * @brief Timer interrupt handler. Increments the global counter.
 */
void (timer_int_handler)();

/**
 * @brief Gets the current configuration of the specified timer.
 * 
 * @param timer Timer to query (0 to 2).
 * @param st Pointer to a variable to store the status byte.
 * @return 0 on success, non-zero otherwise.
 */
int (my_timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays the configuration of the specified timer.
 * 
 * @param timer Timer to display (0 to 2).
 * @param st Status byte of the timer.
 * @param field Enum indicating which field of the status byte to display.
 * @return 0 on success, non-zero otherwise.
 */
int (my_timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

#endif
