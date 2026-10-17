#ifndef _TIMER_H_
#define _TIMER_H_

#include <lcom/lcf.h>

#include "utils.h"
#include "i8254.h"
#include <stdbool.h>
#include <stdint.h>

/** @defgroup Timer Timer
 * @{
 *
 * Functions for the programming of the timer
 */

/**
 * @brief Subscribes the interruptions of the timer
 * 
 * @param bit_no        IRQ line of the timer
 * @return              1 on faulure
 */
int (timer_subscribe_int)(uint8_t* bit_no);

/**
 * @brief Unsubscribes the interruptions of the timer
 * 
 * @return              1 on failure
 */
int (timer_unsubscribe_int)();

/**
 * @brief Handles the timer interruptions
 * 
 */
void (timer_int_handler)();

/**
 * @brief Retrieves the configuration of the timer and returns it in the st param 
 * 
 * @param timer         The timer to retrieve the configuration from
 * @param st            Return value of the confifuration
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays the configuration of the timer based on the field param
 * 
 * @param timer         Timer to display the configuration of
 * @param st            Configuration of the timer
 * @param field         Part of the configuration to show
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

/**
 * @brief Sets the frequency of the timer to the value freq
 * 
 * @param timer         Timer to change the frequency of
 * @param freq          Target frequency
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**@}*/

#endif
