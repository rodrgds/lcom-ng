#ifndef SRC_TIMER_H
#define SRC_TIMER_H

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "macros/i8254.h"


int (timer_set_frequency)(uint8_t timer, uint32_t freq);
/**
 * @brief Subscribes and enables Timer 0 interrupts
 * @param bit_no address of memory to be initialized with the bit number
 * @return Return 0 upon success and non-zero otherwise
 */
int (timer_subscribe_int)(uint8_t *bit_no);
/**
 * @brief unsubscribes timer's interruptions
 * @return Return 0 upon success and non-zero otherwise
 */
int (timer_unsubscribe_int)();
/**
 * @brief increments the timer counter and the position of the snake
 */
void (timer_int_handler)();
/**
 * @brief Reads the input timer configuration (status) via read-back command
 * @param timer Timer whose configuration to read
 * @param st Address of memory position to be filled with the timer config
 * @return Return 0 upon success and non-zero otherwise
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);
/**
 * @brief Shows timer configuration
 * @param timer timer whose configuration should be displayed
 * @param st status read via the read-back command
 * @param field status field to display
 * @return Return 0 upon success and non-zero otherwise
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);


#endif // SRC_TIMER_H
