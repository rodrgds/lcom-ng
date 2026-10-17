/** @file timer.h
    @author Daniel Marques
    @author João Almeida
    Prototypes of functions used to receive interruptions from timer.
    Imported from lab2
*/

#ifndef TIMER_H
#define TIMER_H

/** @defgroup timer timer
 * @{
 *
 * Functions responsible for communicating with the timer
 */

//TOOLS
#define BIT(n) (0x01<<(n))

//HARDWARE
#define TIMER0_IRQ 0

//GLOBAL VARIABLES


//FUNCTIONS

/**
  @brief Subscribes the Timer Interrupts
  @param timer_id To be ID of the Timer Interrupts
*/
void timerSubscribe(int timer_id);

/**
  @brief Unsubscribes the Timer Interrupts
  @param timer_id ID of the Timer Interrupts
*/
void timerUnsubscribe(int timer_id);
#endif
