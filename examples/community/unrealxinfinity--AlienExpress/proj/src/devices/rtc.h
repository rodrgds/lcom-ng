#ifndef RTC_H
#define RTC_H

#include "../macros/includes.h"
#include <lcom/lcf.h>

/**
 * @brief Subscribes the real time clock device, setting into the interrupt mask the respective real time clock hook id bit to 1
 * @return Returns the bit number that was set
 */
uint8_t (rtc_subscribe)();
/**
 * @brief Unsubscribes the real time clock device, setting into the interrupt mask the respective real time clock hook id bit to 0
 * @return Returns 0 if successfull and 1 if otherwise
 */
int (rtc_unsubscribe)();

/**
 * @brief Allows or does not allow interrupts in the real time clock
 * @param flag Boolean to decide whether if real time clock interrupts are allowed or not. Has value true if allowed and false if otherwise
 * @return Returns 0 if successfull
 */

int (update)(bool flag);
/**
 * @brief Waits while the real time clock updates the values of the time
 * @return Returns 0 if successfull
 */
int (rtc_wait_ready)();
/**
 * @brief Reads the seconds, minutes, hours, day, month and year from real time clock
 * @param command Command to sent to the real time clock adress in order to let it read the time values
 * @param index The index of the value to read
 */
void (rtc_read)(uint8_t command, uint8_t index);
/**
 * @brief Handles the interrupts of the real time clock, calling the reading function to update the array that stores the values of the time
 */
void (rtc_ih)();

#endif

