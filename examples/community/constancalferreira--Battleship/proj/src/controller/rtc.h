/**
 * @file rtc.h
 * @brief Real Time Clock (RTC) interface for time management
 * 
 * This file provides functions for interacting with the PC's Real Time Clock.
 * It allows access to time information (hours, minutes, etc.) and includes
 * interrupt subscription/handling mechanisms for real-time updates.
 */

#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/**
 * @brief Structure to store current date and time information
 */
struct real_time_clock_info{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
};


/// @brief Global structure that holds the current time information
struct real_time_clock_info rtc_info;

/// @brief Temporary storage for RTC register output
uint8_t output;

/**
 * @brief Reads the current time from the RTC
 * 
 * Updates the global rtc_info structure with the current time (hours and minutes).
 * Automatically handles BCD to binary conversion if needed.
 *
 * @return 0 upon success, 1 on error
 */
int read_rtc_time();

/**
 * @brief Reads a specific register from the RTC
 * 
 * @param command Register number to read (0-14)
 * @param[out] output Pointer where the register value will be stored
 * @return 0 upon success, 1 on error
 */
int read_rtc_output(uint8_t command, uint8_t *output);

/**
 * @brief Converts a BCD (Binary Coded Decimal) value to binary
 * 
 * @param bcd_value Value in BCD format
 * @return Equivalent value in binary format
 */
uint8_t to_binary(uint8_t bcd_value);

/**
 * @brief Checks if the RTC is in binary mode
 * 
 * Tests bit 2 of the RTC's status register B to determine if
 * values are stored in binary format (otherwise they use BCD).
 *
 * @return Non-zero if RTC is in binary mode, 0 if in BCD mode, 1 on error
 */
int is_rtc_binary();

/**
 * @brief Subscribes RTC interrupts
 * 
 * @param[out] bit_no Pointer to variable that will be filled with the bit position for RTC interrupts
 * @return 0 upon success, non-zero otherwise
 */
int rtc_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes RTC interrupts
 * 
 * @return 0 upon success, non-zero otherwise
 */
int rtc_unsubscribe_int();

/**
 * @brief Enables the Update Ended interrupt on the RTC
 * 
 * Sets the UIE (Update Interrupt Enable) bit in RTC's register B
 * to enable periodic time update interrupts.
 *
 * @return 0 upon success, 1 on error
 */
int rtc_enable_update_int();

/**
 * @brief RTC interrupt handler
 * 
 * Called when an RTC interrupt occurs. Reads register C to acknowledge
 * the interrupt and updates the current time information.
 */
void rtc_ih();

#endif

