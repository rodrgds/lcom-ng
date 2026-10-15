/**
 * @file rtclock.h
 * @brief Contains some functions to operate the Real Time Clock
 * @date 2024-06-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _RTCLOCK_H_
#define _RTCLOCK_H_

#include <lcom/lcf.h>

#include "utils_drivers.h"

#include "../models/Datetime.h"


#define RTC_IRQ 8
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_SECONDS 0
#define RTC_MINUTES 2
#define RTC_HOURS 4
#define RTC_WEEKDAY 6
#define RTC_MONTHDAY 7
#define RTC_MONTH 8
#define RTC_YEAR 9

/**
 * @brief Writes value_idx (year, month, day hours, seconds, or minutes selector) to the RTC address register, then the RTC's output is placed in output
 * 
 * @param value_idx selector
 * @param output 
 * @return 0 upon success, 1 otherwise 
 */
int rtc_get_out(uint8_t value_idx, uint8_t *output);
/**
 * @brief Returns the current date in datetime format
 * 
 * @return Datetime 
 */
Datetime (rtc_read_datetime)();

#endif
