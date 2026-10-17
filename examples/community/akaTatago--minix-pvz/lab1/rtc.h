#pragma once



#include <stdint.h>

typedef struct {
  uint8_t day;
  uint8_t month;
  uint8_t year;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
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

int (rtc_subscribe_int)(uint8_t *bit_no);
int (rtc_unsubscribe_int)();

