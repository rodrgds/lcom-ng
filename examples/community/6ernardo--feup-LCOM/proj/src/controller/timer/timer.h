
#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#define TIMER_MASK BIT(0)

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8254.h"

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @brief Changes the operating frequency of a timer
 * @param timer Timer to configure.
 * @param freq Timer operating frequency
 * @return Return 0 upon success and non-zero otherwise
 */

int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes timer interrupts
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return 0 if it is successful, not 0 if otherwise
 */

int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes timer interrupts
 * @return 0 if it is successful, not 0 if otherwise
 */

int (timer_unsubscribe_interrupts)();

/**
 * @brief Reads the input timer configuration via read-back command
 * @param timer Timer whose config to read (Ranges from 0 to 2)
 * @param st    Address of memory position to be filled with the timer config
 * @return Return 0 upon success and non-zero otherwise
 */

int (timer_get_conf)(uint8_t timer, uint8_t *st);

#endif
