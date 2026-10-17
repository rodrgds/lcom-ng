#ifndef TIMER_H
#define TIMER_H

#include <lcom/lcf.h>
#include "i8254.h"

#define TIMER_KEY 31
#define IRQ_SET_TIMER BIT(TIMER_KEY)

/**
 * @brief function that subscribes the timer interrupts
 * 
 * @return int 0 if success, 1 otherwise
 */
int timer_subscribe_interrupt();

/**
 * @brief function that unsubscribes the timer interrupts
 * 
 * @return int 0 if success, 1 otherwise
 */
int timer_unsubscribe_interrupt();

/**
 * @brief function that checks if timer 0 is interrupting
 * 
 * @param ipc_status status of the ipc
 * @param msg message 
 * @return true if timer 0 is interrupting, false otherwise
 */
bool is_timer_0_interrupt(int ipc_status, message msg);


#endif
