/**
 * @file rtc.h
 * @brief Real-Time Clock device interface.
 */
#ifndef RTC_H
#define RTC_H

#include <stdint.h>

/**
 * @brief Structure to hold RTC time information.
 */
typedef struct {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t day;
  uint8_t month;
  uint8_t year;
} RTC_Time;

/**
 * @brief Initializes the RTC.
 */
void rtc_init(void);

/**
 * @brief Reads the current time from the RTC.
 * @return The current time as RTC_Time.
 */
RTC_Time rtc_get_time(void);

#endif
