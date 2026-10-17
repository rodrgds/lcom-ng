/**
 * @file timer
 * 
 * @brief  This file contains functions and constants related to timer handling and configuration. It does synchronization and interruptions of info
 * as asked.
 * 
 * @author João Pedro Sousa, Bernardo Pinto, Carlos Madaleno
 * 
 * @date 28/05/2023  
*/


#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#define TIMER_MASK BIT(0)

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8254.h"

/**
 * @brief Sets the frequency of the timer.
 * 
 * @param timer The timer to configure (always 0 for this project).
 * 
 * @param freq The desired frequency of the timer (30).
 * 
 * @return 0 on success, non-zero if there was an error setting the frequency.
*/

int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes to timer interrupts.
 * 
 * @return Return the hook_id used for the subscription on success, negative value otherwise.
*/

int (timer_subscribe_interrupts)();

/**
 * @brief Unsubscribes from timer interrupts.
 * 
 * @return 0 on success, non-zero if there was an error unsubscribing from the interrupts.
*/

int (timer_unsubscribe_interrupts)();

/**
 * @brief Retrieves the configuration of a timer.
 * 
 * @param timer The timer to retrieve the configuration from (always 0 for this project).
 * 
 * @param st Pointer to an 8-bit variable where the configuration will be stored.
 * 
 * @return 0 on success, non-zero if the pointer is invalid or there was an error retrieving the configuration.
*/

int (timer_get_conf)(uint8_t timer, uint8_t *st);


#endif
