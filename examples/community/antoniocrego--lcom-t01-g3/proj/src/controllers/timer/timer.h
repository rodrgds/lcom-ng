#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#include <minix/sysutil.h>

#include <lcom/lcf.h>

/// @brief Timer's interrupt handler 
void (timer_ih)();

/// @brief Subscribes the interrupts for timer
/// @param bit_no The bit of timer's hook id
/// @return Returns 0 on success and 1 otherwise
int (timer_subscribe_int)(uint8_t *bit_no);

/// @brief Unsubscribes the interrupts for timer
int (timer_unsubscribe_int)();

/// @brief Sets the timer's frequency 
/// @param timer Timer to set the frequency
/// @param freq Frequency to be set
/// @return Returns 0 on success and 1 otherwise
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

#endif
