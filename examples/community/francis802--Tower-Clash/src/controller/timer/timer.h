/**
 * @file timer.h
 * 
 * @brief This file contains the definitions of the functions to configure and interact with the timer
*/

#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#define TIMER_MASK BIT(0)

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8254.h"

/**
 * @brief This function configures any of the timers to the given frequency. It writes a configuration
 * to select the desired timer to be configured. Then it writes the new frequency to the timer. 
 * 
 * @param timer Timer to configure
 * @param freq Frequency to be set
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief This function subscribes to timer interrupts, using the sys_irqsetpolicy() function, 
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (timer_subscribe_interrupts)();

/**
 * @brief This function unsubscribes timer interrupt, using the sys_irqrmpolicy() function,
 * and the hook_id fetched by the timer_subscribe_interrupts() function.
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (timer_unsubscribe_interrupts)();

/**
 * @brief This function gets the configuration of any of the timers. It writes a
 * read-back command to the control register, and then reads the configuration
 * of the desired timer.
 * 
 * @param timer Timer to get the configuration of
 * @param st Address of memory position to be filled with the configuration
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int (timer_get_conf)(uint8_t timer, uint8_t *st);

#endif
