/// @file rtc.h
/// @brief Defines and declarations for interacting with the Real-Time Clock (RTC).

#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/// @brief RTC interrupt hook mask
#define RTC_MASK            5

/// @brief RTC IRQ line
#define IRQ_RTC             8

/// @brief RTC register input port
#define REGISTER_INPUT      0x70

/// @brief RTC register output port
#define REGISTER_OUTPUT     0x71

/// @brief RTC register: updating status
#define REGISTER_UPDATING   10

/// @brief RTC register: counting mode
#define REGISTER_COUNTING   11

/// @brief RTC register: seconds
#define SECONDS    0

/// @brief RTC register: minutes
#define MINUTES    2

/// @brief RTC register: hours
#define HOURS      4   

/// @brief RTC register: day of month
#define DAY        7

/// @brief RTC register: month
#define MONTH      8

/// @brief RTC register: year
#define YEAR       9

/// @brief Binary mode flag in RTC register
#define BINARY              BIT(2)

/// @brief Update in progress flag in RTC register
#define UPDATING            BIT(7)

/// @brief Structure to store real-time clock information
typedef struct {
    uint8_t year;     ///< Year
    uint8_t month;    ///< Month
    uint8_t day;      ///< Day
    uint8_t hours;    ///< Hours
    uint8_t minutes;  ///< Minutes
    uint8_t seconds;  ///< Seconds
} real_time_info;

/**
 * @brief Subscribes to RTC interrupts.
 * @return 0 on success, non-zero on failure
 */
int rtc_subscribe_int();

/**
 * @brief Unsubscribes from RTC interrupts.
 * @return 0 on success, non-zero on failure
 */
int rtc_unsubscribe_int();

/**
 * @brief Reads a value from a specified RTC register.
 * @param command RTC register to read from
 * @param output Pointer to variable to store the read value
 * @return 0 on success, 1 on failure
 */
int rtc_output(uint8_t command, uint8_t *output);

/**
 * @brief Checks whether the RTC is currently updating.
 * @return 1 if updating or error, 0 otherwise
 */
int rtc_is_updating();

/**
 * @brief Checks if the RTC is in binary counting mode.
 * @return 1 if in binary mode, 0 if in BCD or on error
 */
int rtc_is_binary();

/**
 * @brief Updates the time_info structure with the current time from RTC.
 * @return 0 on success, 1 on failure
 */
int rtc_update_time();

/**
 * @brief Converts a BCD value to binary.
 * @param bcd_number BCD value to convert
 * @return Converted binary value
 */
uint8_t to_binary(uint8_t bcd_number);

#endif /* _LCOM_RTC_H_ */

