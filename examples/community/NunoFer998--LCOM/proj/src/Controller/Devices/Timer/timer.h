/**
 * @file timer.h
 * @brief Timer device driver header file
 * 
 * This header file defines the timer driver interface including
 * interrupt subscription, frequency configuration, and timer management functions.
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "../macros.h"
#include "../func_aux.h"

/**
 * @brief Subscribes to timer interrupts
 * 
 * @param bit_no Pointer to store the interrupt bit number
 * @return 0 on success, non-zero on error
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from timer interrupts
 * 
 * @return 0 on success, non-zero on error
 */
int (timer_unsubscribe_int)();

/**
 * @brief Sets the frequency of a timer
 * 
 * @param timer Timer number (0, 1, or 2)
 * @param freq Desired frequency in Hz
 * @return 0 on success, non-zero on error
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Gets the configuration of a timer
 * 
 * @param timer Timer number to query
 * @param st Pointer to store the timer status
 * @return 0 on success, non-zero on error
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Timer interrupt handler
 * 
 * This function handles timer interrupts and updates game counters.
 */
void (timer_int_handler)();

#endif
