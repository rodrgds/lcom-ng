/**
 * @file timer.h
 *
 */

#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8254.h"

/* IRQ */
#define TIMER0_IRQ 0

/**
 * @brief different components of the status of a timer
 * 
 * @param timer timer chosen
 * 
 * @param init_mode initialisation mode of the timer
 * 
 * @param count_mode count_mode of the timer
 * 
 * @param bcd tells if the timer is bcd
 * 
 */
typedef struct {
    uint8_t timer;
    enum timer_init init_mode;
    uint8_t count_mode;
    bool bcd;
} timer_status;

/**
 * @brief writes command to timer controller
 * 
 * @param command command to be written
 * 
 */
int (timer_write_command)(uint8_t command);
/**
 * @brief makes control word for given timer
 * 
 * @param timer timer chosen
 * 
 * @param init_mode initialization mode of the time
 * 
 * @param control_word address on which the control_word is returned
 * 
 */
int (timer_make_control_word)(uint8_t timer, enum timer_init init_mode, uint8_t *control_word);
/**
 * @brief makes read_back bit for given timer
 * 
 * @param timer chosen timer
 * 
 * @param read_back address on which the read back command is written
 * 
 */
int (timer_make_read_back)(uint8_t timer, uint8_t *read_back);
/**
 * @brief returns the status of the given timer and returns it on the given address
 * 
 * @param timer timer chosen to get status
 * 
 * @param st address on which the status is put
 * 
 */
int (timer_get_status)(uint8_t timer, uint8_t *st);
/**
 * @brief receives a status bit and parse it into its different components
 * 
 * @param st status to be parsed
 * 
 */
timer_status (timer_parse_status)(uint8_t st);
/**
 * @brief sets the frequency of a given timer
 * 
 * @param timer timer chosen to have its frequency altered
 * 
 * @param freq chosen frequency
 * 
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

// for interrupts
/**
 * @brief subscribes the timers interrupts
 * 
 */
int (timer_subscribe_int)(uint8_t* bit_no);
/**
 * @brief unsubscribed«s timer interrupts
 * 
 */
int (timer_unsubscribe_int)();

#endif // _LCOM_TIMER_H_
