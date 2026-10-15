/**
 * @file mrtc
 * @brief This file has all defines related to the real time clock.
 */

#ifndef _MRTC_H_
#define _MRTC_H_

/** @brief RTC (Real Time Clock) command register I/O port */
#define RTC_COMMAND_REGISTER 0x70
/** @brief RTC (Real Time Clock) data register I/O port */
#define RTC_DATA_REGISTER    0x71

/** @brief RTC (Real Time Clock) register number for seconds */
#define RTC_SECONDS 0
/** @brief RTC (Real Time Clock) register number for minutes */
#define RTC_MINUTES 2
/** @brief RTC (Real Time Clock) register number for hours */
#define RTC_HOURS   4
/** @brief RTC (Real Time Clock) register number for day of the month */
#define RTC_DAY     7
/** @brief RTC (Real Time Clock) register number for month */
#define RTC_MONTH   8
/** @brief RTC (Real Time Clock) register number for year */
#define RTC_YEAR    9

#endif
