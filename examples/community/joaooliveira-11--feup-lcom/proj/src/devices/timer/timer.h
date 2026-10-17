#ifndef _TIMER_PROJECT
#define _TIMER_PROJECT

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

#define BIT(n) (1 << (n))

///@brief Set the frequency of a timer.
///@note This function sets the frequency of a timer specified by the timer parameter. The frequency is set to the value specified by the freq parameter.
///@param timer The timer to set the frequency for (0, 1, or 2).
///@param freq The desired frequency.
///@return 0 if the frequency is set successfully, otherwise returns 1.
int (timer_set_frequency)(uint8_t timer, uint32_t freq);


///@brief Get the configuration of a timer.
///@note This function gets the configuration of a timer specified by the timer parameter and stores it in the st variable.
///@param timer The timer to get the configuration for (0, 1, or 2).
///@param st Pointer to store the timer configuration.
///@return 0 if the configuration is retrieved successfully, otherwise returns 1.
int (timer_get_conf)(uint8_t timer, uint8_t *st);

///@brief Subscribe to timer interrupts.
///@note This function subscribes to timer interrupts by setting the appropriate bit in the mask and calling sys_irqsetpolicy to register the interrupt handler.
///@param mask Pointer to the mask that will hold the timer interrupt bit.
///@return 0 if the subscription is successful, otherwise returns 1.
int (timer_subscribe_int)(uint8_t *bit_no);


///@brief Unsubscribe from timer interrupts.
///@note This function unsubscribes from timer interrupts by removing the previously set interrupt policy using sys_irqrmpolicy.
///@return 0 if the unsubscription is successful, otherwise returns 1.
int (timer_unsubscribe_int)();

#endif
