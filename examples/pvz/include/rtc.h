#ifndef RTC_H
#define RTC_H

#include <stddef.h>

void rtc_update_cached_datetime(void);
void rtc_load_last_played(void);
void rtc_save_last_played(void);
void rtc_format_last_played(char *buffer, size_t size);

#endif
