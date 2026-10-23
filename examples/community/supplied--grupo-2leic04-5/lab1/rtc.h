#ifndef GRUPO_2LEIC04_5_RTC_H
#define GRUPO_2LEIC04_5_RTC_H
#include <stdint.h>
typedef struct { uint8_t day, month, year; } rtc_date;
int rtc_read_date(rtc_date *date);
#endif
