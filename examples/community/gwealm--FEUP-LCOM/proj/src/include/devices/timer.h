#pragma once

#include <lcom/lcf.h>
#include "errors.h"

#include "events/events.h"

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */

/* I/O port addresses */

#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define TIMER_1    0x41 /**< @brief Timer 1 count register */
#define TIMER_2    0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */

#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB     BIT(4)                  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB     BIT(5)                  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_OPERATING_MODE (BIT(1) | BIT(2) | BIT(3))
#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */


/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)

/**@}*/

#define TIMER_DONT_CHANGE (BIT(0) | BIT(1) | BIT(2) | BIT(3))
#define TIMER_SEL(n) (n << 6) 

/**
 * @brief Handles operations to inicialize timer for usage
 *
 * @return 0 if the operation was successful, non-zero otherwise 
 */
int (timer_init)();

/**
 * @brief Handles operations to terminate timer usage
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (timer_cleanup)();

/**
 * @brief Subscribes timer interrupts
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (timer_subscribe_interrupts)();

/**
 * @brief Unubscribes timer interrupts
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
int (timer_unsubscribe_interrupts)();

/**
 * @brief Handles a timer interrupt
 * 
 */
void (timer_handle_interrupt)(struct event *event);

/**
 * @return The timer's current frequency
 * 
 */
unsigned long (timer_get_frequency)();
