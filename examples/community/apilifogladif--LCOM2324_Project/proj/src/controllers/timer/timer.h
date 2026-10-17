#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

/**
 * @file timer.h
 * 
 * @brief This file contains the timer functions.
*/


/**
 * @brief This function sets the frequency of a timer specified by 'timer' to the value 'freq'.
 * 
 * It calculates the appropriate values for the timer to generate the desired frequency
 * and writes them to the timer's control registers and data ports.
 * 
 * @param timer : Timer (0,1 or 2).
 * @param freq : The desired frequency.
 * @return int : 0 if sucess and 1 otherwise.
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes to timer interrupts, setting a bit number for the interrupt line.
 * 
 * @param bit_no : IRQ.
 * @return int : 0 if sucess and 1 otherwise.
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from timer interrupts, removing the interrupt policy.
 * 
 * @return int : 0 if sucess and 1 otherwise.
 */
int (timer_unsubscribe_int)();

/**
 * @brief Handles timer interrupts by incrementing a global counter
 * 
 */
void (timer_int_handler)();

/**
 * @brief This function is intended to get the configuration of a timer specified by 'timer'.
 * 
 * It reads the configuration byte of the specified timer and stores it in the memory
 * location pointed to by the pointer 'st'.
 * 
 * @param timer : Timer (0,1 or 2).
 * @param st : The status byte of the timer.
 * @return int : 0 if sucess and 1 otherwise.
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief This function is intended to display the configuration of a timer 
 * specified by 'timer', based on the requested field 'field'.
 * 
 * @param timer : Timer (0,1 or 2).
 * @param st : The status byte of the timer.
 * @param field : The field of the timer configuration to display.
 * @return int : 0 if sucess and 1 otherwise.
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

#endif
