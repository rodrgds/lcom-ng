#ifndef NINJIX_RTC_H
#define NINJIX_RTC_H

#include "ninjix_platform.h"

#include <lcom/rtc.h>

typedef struct {
  uint8_t day;
  uint8_t month;
  uint8_t year;
} rtc_date;

typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
} rtc_time;

int rtc_read_date(rtc_date *date);
int rtc_read_time(rtc_time *time);

#endif
