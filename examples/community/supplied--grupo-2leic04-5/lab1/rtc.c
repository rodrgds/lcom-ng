#include "rtc.h"
#include <time.h>
int rtc_read_date(rtc_date *date) {
    if (!date) return 1;
    time_t now = time(NULL);
    struct tm *today = localtime(&now);
    if (!today) return 1;
    date->day = (uint8_t)today->tm_mday;
    date->month = (uint8_t)(today->tm_mon + 1);
    date->year = (uint8_t)(today->tm_year % 100);
    return 0;
}
