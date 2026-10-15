#ifndef _LCOM_DRIVER_TIMER_H_
#define _LCOM_DRIVER_TIMER_H_

#include <lcom/lcf.h>
#include "i8254.h"

/**
 * @defgroup timer Timer
 * @ingroup drivers
 * @{
 * 
 * @brief Driver for the i8254 timer device. 
 * @details The i8254 contains 3 timers (identified from 0 to 2).
 */

/**
 * @brief Subscribes the program to interruptions from Timer 0.
 * 
 * @param bit_no Pointer where the bit number signaling the interruption will be saved
 * @return 0 if sucessful, 1 otherwise
*/
int		(timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes interruptions from Timer 0.
 * 
 * @return 0 if sucessful, 1 otherwise
*/
int		(timer_unsubscribe_int)();

/**
 * @brief Handles timer interruptions and updates the internal interruption counter.
*/
void	(timer_int_handler)();

/**
 * @brief Returns current timer configuration.
 * 
 * @param timer Timer number (from 0 to 2)
 * @param st Pointer to save the result
 * @return 0 if sucessful, 1 otherwise
*/
int		(timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays current time configuration, considering which 
 * status type field is indicated.
 * 
 * @param timer Timer number (from 0 to 2)
 * @param st Pointer to save the result
 * @param field Field to be displayed
 * @return 0 if sucessful, 1 otherwise
*/
int		(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

/**
 * @brief Sets the frequency of a timer (and by consequence the number of interrupts per second).
 * 
 * @details The default is 60 Hz = 60 interrupts per second.
 * 
 * @param timer Timer number (from 0 to 2)
 * @param freq New frequency
 * @return 0 if sucessful, 1 otherwise
*/
int		(timer_set_frequency)(uint8_t timer, uint32_t freq);

/** @} */

#endif /* _LCOM_DRIVER_TIMER_H */
