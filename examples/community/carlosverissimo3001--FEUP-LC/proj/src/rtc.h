#ifndef _RTC_H_
#define _RTC_H_

#include <lcom/lcf.h>
#include <stdio.h>

#include "macros.h"

/** @defgroup rtc
 * @{
 *
 *
 *
 * RTC related functions and structures.
 */

typedef struct {
    int seconds;        /**< Seconds field */
    int minutes;        /**< Minutes field */
    int hours;          /**< Hours fields */
} Time;

/**
 * @brief Checks if the dark mode needs to be enabled, i.e, if its past 7pm
 * and before 7am
 * @return True is dark mode needs to be enabled, false otherwise
 */
bool (is_dark_mode)();

/**
 * @brief Reads data from a RTC register
 * @param register Register to read from
 * @param data Where to read to
 * @return 0 on success, 1 otherwise
 */
int (read_from_rtc_register)(int register, uint32_t *data);

/**
 * @brief Reads current time from the Real Time Clock (RTC)
 * @param time Time struct to read time to
 * @return 0 on success, 1 otherwise
 */
int (read_curr_time_from_rtc)(Time *time);

/**
 * \author https://stackoverflow.com/a/42340213
 * @brief Converts to decimal format, a nubmer represent by the BCD format
 * @param x bcd number to be converted
 * @return decimal representation of x
 */
int bcd_to_decimal(uint32_t x);




/** @} end of rtc */

#endif
