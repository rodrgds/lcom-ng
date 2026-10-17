#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_


#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define TIMER      0
#define TIMER_FREQ 1193182 
#define TIMER0_IRQ 0

/// @brief sets the timer frequency to the desired one
/// @param timer the timer that is configured
/// @param freq frequency that it sets the timer to
/// @return 1 in case of success, and 0 if there´s any problem with the system call
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/// @brief subscribes timer interrupts
/// @return 1 in case of success, and 0 if there´s any problem with the system call
int (timer_subscribe_interrupts)();

/// @brief unsubscribes timer interrupts
/// @return 1 in case of success, and 0 if there´s any problem with the system call
int (timer_unsubscribe_interrupts)();


/// @brief fetches the timer status
/// @param timer the timer that the configuration is fetch from
/// @param st variable where the status of the timer goes to
/// @return 1 in case of success, and 0 if there´s any problem with the system call
int (timer_get_conf)(uint8_t timer, uint8_t *st);


#endif
