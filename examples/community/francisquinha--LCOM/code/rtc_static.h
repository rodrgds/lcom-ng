#ifndef _RTC_STATIC_H
#define _RTC_STATIC_H

/** @file rtc_static.h
 * @brief file containing the static function declarations for the RTC controller
 */

/** @defgroup rtc_static rtc_static
 * @{
 *
 * @brief static functions for programming the RTC.
 */

/**
 * @brief waits for the rtc to have valid output
 */
static void wait_valid_rtc();

/**
 * @brief transforms a binary coded decimal into a decimal
 * @param bcd bynary coded decimal
 * @return decimal number
 */
static unsigned short bcd2dec(unsigned long bcd);

/**@}*/

#endif /*_RTC_STATIC_H*/
