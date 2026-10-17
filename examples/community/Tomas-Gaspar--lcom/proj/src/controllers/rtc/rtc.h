#ifndef _RTC_H_
#define _RTC_H_

#include <stdint.h>

/** @ingroup RTC
 * @{
 **/

/**
 * @brief Subscribes to RTC interrupts.
 *
 * @param bit_no Pointer to store the RTC hook id.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from RTC interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (rtc_unsubscribe_int)();

/**
 * @brief Enables RTC interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int rtc_enable_int();

/**
 * @brief Disables RTC interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int rtc_disable_int();

/**
 * @brief Enables or disables RTC alarm interrupts.
 *
 * @param enable If true, enables RTC alarm interrupts. If false, disables RTC alarm interrupts.
 * @return Return 0 upon success, non-zero otherwise.
 */
int rtc_alarm_interrupts(bool enable);

/**
 * @brief Sets up the RTC alarm.
 *
 * This function configures the RTC to trigger an alarm at the next predefined hour.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int rtc_setup_alarm();

/**
 * @brief Gets the next alarm time.
 * 
 * Based on the current time, finds out when the next alarm should be.
 *
 * @return The next alarm time hours.
 */
int get_next_alarm();

/**
 * @brief Inhibits or enables updates to the RTC registers.
 *
 * Allows inhibiting updates to prevent data inconsistency.
 * 
 * @param inhibit If true, inhibits updates to the RTC registers. If false, enables updates.
 * @return Return 0 upon success, non-zero otherwise.
 */
int rtc_inhibit_updates(bool inhibit);

/**
 * @brief RTC interrupt handler.
 *
 * This function is the interrupt handler for RTC interrupts.
 */
void rtc_ih();

/**
 * @brief Waits until the RTC is ready for communication.
 */
void wait_valid_rtc();

/**
 * @brief Sets the RTC alarm hour.
 *
 * @param hour The hour at which the RTC alarm should trigger.
 * @return Return 0 upon success, non-zero otherwise.
 */
int rtc_set_alarm_hour(uint8_t hour);

/**
 * @brief Gets the current hours from the RTC.
 *
 * @param hours Pointer to store the current hours value.
 * @return Return 0 upon success, non-zero otherwise.
 */
int rtc_get_hours(uint8_t* hours);

/** }@ */

#endif
