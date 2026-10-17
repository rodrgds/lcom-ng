/**
 * @file timer.h
 * @brief Interface for handling i8254 timer operations.
 *
 * This header provides functions for configuring the i8254 timer, subscribing and unsubscribing
 * timer interrupts, handling timer input, and displaying timer configuration.
 */
#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#define TIMER_MASK BIT(0)

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8254.h"


/**
 * @defgroup timer timer
 * 
 *
 * @brief Functions for handling timer via the i8254 controller.
 * @{
 */

/**
 * @brief Sets the frequency of the specified timer.
 * 
 * @param timer Timer to configure (0, 1, or 2).
 * @param freq Desired frequency in Hz.
 * @return 0 on success, 1 otherwise.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes and enables timer interrupts.
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * @return 0 on success, 1 otherwise.
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes timer interrupts.
 * 
 * @return 0 on success, 1 otherwise.
 */
int (timer_unsubscribe_int)();

/**
 * @brief Gets the current configuration of the specified timer.
 * 
 * @param timer Timer to query (0, 1, or 2).
 * @param st Pointer to variable to store the status byte.
 * @return 0 on success, 1 otherwise.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Timer interrupt handler.
 * 
 * Handles timer interrupts (should be called on each timer interrupt).
 */
void (timer_int_handler)();

/**
 * @brief Displays the configuration of the specified timer.
 * 
 * @param timer Timer to display (0, 1, or 2).
 * @param st Status byte to interpret.
 * @param field Enum value specifying which field of the status byte to display.
 * @return 0 on success, 1 otherwise.
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

/** @} */

#endif //_LCOM_TIMER_H_
