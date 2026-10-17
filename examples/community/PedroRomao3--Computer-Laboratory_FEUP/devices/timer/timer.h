/**
 * @file timer.h
 * @brief ...
 * @author musta
 * @date 15/04/2023
 */


#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"
#include "rtc.h"
#include "../../settings.h"

#define mask_timer BIT(0)


/**
 @brief Sets the frequency of a specified timer.
This function sets the frequency of the specified timer by calculating and writing
the appropriate values to the control and counter registers.
 @param timer The number of the timer to be configured (0, 1, or 2).
 @param freq The desired frequency in Hz (between 19 and TIMER_FREQ).
 @return 0 if successful, 1 if an error occurs (e.g., invalid timer number or frequency).
 @warning This function assumes the TIMER_FREQ constant is defined.
 @complexity This function has a time complexity of O(1) as all operations are constant time.
*/
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 @brief Subscribes to the timer interrupts.
 @param bit_no pointer to the memory address where the mask of the timer interrupt bit will be stored
 @return Return 0 upon success and non-zero otherwise
This function subscribes to the timer interrupts, storing the corresponding interrupt bit in the memory pointed by bit_no. It also sets the IRQ_REENABLE policy for the timer.
*/
int (timer_subscribe_int)(uint8_t *bit_no);

/**
Function: int timer_unsubscribe_int()
Description: Unsubscribes from timer interrupts.
This function cancels a previous interrupt notification that was set using sys_irqsetpolicy.
Returns: 0 if the operation is successful, non-zero otherwise.
Complexity: This function has a time complexity of O(1) as all operations are constant time.
*/
int (timer_unsubscribe_int)();

/**
 @brief Handles timer interrupt events and increments a global counter.
This function is designed to be called whenever a timer interrupt occurs.
It increments a global counter variable, which can be used to track the passage of time or the number of timer interrupt events.
 @note This function is intended to be used as an interrupt service routine
(ISR) and should be registered with the appropriate interrupt
controller or timer hardware.
 @note The complexity of this function is O(1).
*/
void (timer_ih)();

/**
 @brief Retrieves the configuration of a specified timer.
This function retrieves the configuration of a given timer (0, 1, or 2) and
stores it in the provided uint8_t pointer. In case of invalid timer value
or null pointer, the function returns 1 (error).
 @param timer The timer number (0, 1, or 2) for which the configuration is to be retrieved.
 @param st Pointer to a uint8_t variable where the configuration will be stored.
 @return 0 on success, 1 on failure (invalid timer value or null pointer).
 @note The complexity of this function is O(1).
*/
int (timer_get_conf)(uint8_t timer, uint8_t *status);

#endif
