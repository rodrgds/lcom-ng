#pragma once

#include <stdint.h>

#define RTC_SECONDS 0x00
#define RTC_MINUTES 0x02
#define RTC_HOURS 0x04

typedef struct {
  uint8_t day;
  uint8_t month;
  uint8_t year;
} rtc_date;

/**
 * Reads the current date from the RTC and fills the provided `rtc_date`
 * structure. Returns 0 on success, non-zero on failure.
 *
 * This function takes care of ensuring data consistency by checking the Update
 * In Progress (UIP) flag before each read. It also should check the RTC
 * configuration and perform data conversions if necessary (e.g. BCD to binary).
 */
int rtc_read_date(rtc_date *date);
int rtcGetSeconds();
