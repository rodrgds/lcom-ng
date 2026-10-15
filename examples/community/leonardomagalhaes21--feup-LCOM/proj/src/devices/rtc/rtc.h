/**
 * @file rtc.h
 * @brief Contains functions for handling Real-Time Clock (RTC) interactions.
 */

#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
struct real_time_clock_info{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
};


struct real_time_clock_info rtc_info;
uint8_t output;


/**
 * @brief Reads the output from the RTC.
 *
 * @param command The RTC command to send.
 * @param output Pointer to the variable that will store the output.
 * @return 0 on success, 1 on failure.
 */
int read_rtc_output(uint8_t command, uint8_t *output);


/**
 * @brief Checks if the RTC is currently updating.
 *
 * @return 0 if the RTC is not updating, 1 if it is updating.
 */
int is_rtc_updating();


/**
 * @brief Checks if the RTC is in binary mode.
 *
 * @return 1 if the RTC is in binary mode, 0 otherwise.
 */
int is_rtc_binary();


/**
 * @brief Converts a BCD value to binary.
 *
 * @param bcd_value The BCD value to convert.
 * @return The binary representation of the BCD value.
 */
uint8_t to_binary(uint8_t bcd_value);


/**
 * @brief Reads the current time from the RTC and stores it in the rtc_info structure.
 *
 * @return 0 on success, 1 on failure.
 */

int read_rtc_time();



#endif
