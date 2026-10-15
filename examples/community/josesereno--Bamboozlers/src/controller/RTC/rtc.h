#ifndef RTC_H_
#define RTC_H_
/**
 * @file rtc.h
 * @brief Real-Time Clock (RTC) library for time and alarm management.
 */

#include <stdint.h>
#include <stdbool.h>

#include "rtc_macros.h"

/** @defgroup rtc rtc
 * @{
 */

/**
 * @brief Subscribes RTC interrupts.
 *
 * @param bit_no Address of memory to be initialized with the bit mask for RTC interrupts.
 * @return Return 0 upon success and non-zero otherwise.
 */
int rtc_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes RTC interrupts.
 *
 * @return Return 0 upon success and non-zero otherwise.
 */
int rtc_unsubscribe_int();

/**
 * @brief Enables RTC updates.
 *
 * @return Return 0 upon success and non-zero otherwise.
 */
int enable_update();

/**
 * @brief Disables RTC updates.
 *
 * @return Return 0 upon success and non-zero otherwise.
 */
int disable_update();

/**
 * @brief Waits until the RTC finishes updating.
 *
 * @return Return 0 upon success and non-zero otherwise.
 */
int wait_valid_rtc();

/**
 * @brief RTC interrupt handler.
 */
void rtc_ih();

/**
 * @brief Sets the RTC interrupts.
 *
 * @param value Value to set for the interrupts (true or false).
 * @return Return 0 upon success and non-zero otherwise.
 */
int set_alarm_upd_interrupt(bool value);

/**
 * @brief Reads the hours value from the specified RTC register.
 *
 * @param reg RTC register to read.
 * @param hours Pointer to store the read hours value.
 * @return Return 0 upon success and non-zero otherwise.
 */
int rtc_read_hours(uint8_t reg, uint8_t* hours);

/**
 * @brief Updates the RTC time.
 */
void rtc_upd();

/**
 * @brief Sets the power-up alarm based on the current time.
 */
void set_darkMode_alarm();

/** @} */ // end of rtc

#endif
