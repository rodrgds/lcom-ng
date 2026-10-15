/**
 * @file timer.h
 * @brief implements the functions related to the timer I/O.
 * 
 */

#ifndef _PROJ_TIMER_
#define _PROJ_TIMER_

#include <lcom/lcf.h>

/**
 * @brief subscribes to the interrupts of the Timer 0.,
 * Every time the timer sends a pulse an interrupt is sent to the program.
 * 
 * @param no_bit points to a variable that contains the bit number that will be set in the mask.
 * @return 0 if success, otherwise failure.
 */
int (timer_int_sub)(uint8_t *no_bit);
/**
 * @brief unsubscribes to the interrupts of the Timer 0.
 * Every time the timer sends a pulse it stops sending interrupts to the program.
 * 
 * @return 0 if sucess, otherwise failure.
 */
int (timer_int_unsub)();

/**
 * @brief sets the frequency of Timer 0.
 * This corresponds to the number of interrupts that the Timer 0 sends per second.
 * 
 * @param frequency number of interrupts send by Timer 0 per second.
 * @return 0 if sucess, otherwise failure.
 */
int (timer_set_freq)(uint32_t frequency);
/**
 * @brief obtains the current configuration of the timer.
 * Sends a Read-Back Command to get the status byte of the timer.
 * 
 * @param byte points to a variable that contains the status byte of the timer.
 * @return 0 if sucess, otherwise failure.
 */
int (timer_get_config)(uint8_t *byte);


#endif // _PROJ_TIMER_
