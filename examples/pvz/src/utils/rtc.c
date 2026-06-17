#include "rtc.h"

#include <stdio.h>
#include <stdint.h>
#include "pvz_platform.h"
#include "pvz_platform.h"

#include "constants.h"


typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t valid;
} rtc_datetime;

static rtc_datetime cached_datetime;
static rtc_datetime last_played;
static uint8_t has_last_played;

static uint8_t bcd_to_bin(uint8_t bcd) {
    return (bcd & 0x0F) + ((bcd >> 4) * 10);
}

static uint8_t is_leap_year(uint16_t year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}


static long long rtc_datetime_to_seconds(const rtc_datetime *datetime) {
    static const int month_offsets[12] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
    };
    int year = 2000 + datetime->year;
    long long days = 0;

    for (int y = 2000; y < year; y++) {
        days += is_leap_year((uint16_t) y) ? 366 : 365;
    }

    days += month_offsets[datetime->month - 1];
    if (datetime->month > 2 && is_leap_year((uint16_t) year)) {
        days++;
    }
    days += datetime->day - 1;

    return (((days * 24) + datetime->hour) * 60 + datetime->minute) * 60 + datetime->second;
}

static int rtc_read_register(uint8_t reg, uint32_t *value) {
    if (sys_outb(RTC_ADDR_REG, reg) != 0) return 1;
    if (sys_inb(RTC_DATA_REG, value) != 0) return 1;
    return 0;
}

static int rtc_read_datetime(rtc_datetime *datetime) {
    uint32_t reg_a;
    uint32_t reg_b;
    uint32_t second, minute, hour, day, month, year;
    uint8_t is_24_hour;
    uint8_t is_binary;
    uint8_t is_pm;

    do {
        if (rtc_read_register(RTC_REG_A, &reg_a) != 0) return 1;
        if (reg_a & RTC_UIP_MSK) {
            tickdelay(micros_to_ticks(10000));
        }
    } while (reg_a & RTC_UIP_MSK);

    if (rtc_read_register(RTC_REG_SECONDS, &second) != 0) return 1;
    if (rtc_read_register(RTC_REG_MINUTES, &minute) != 0) return 1;
    if (rtc_read_register(RTC_REG_HOURS, &hour) != 0) return 1;
    if (rtc_read_register(RTC_REG_DAY, &day) != 0) return 1;
    if (rtc_read_register(RTC_REG_MONTH, &month) != 0) return 1;
    if (rtc_read_register(RTC_REG_YEAR, &year) != 0) return 1;
    if (rtc_read_register(RTC_REG_B, &reg_b) != 0) return 1;

    is_24_hour = (reg_b & RTC_24_HOUR_MSK) != 0;
    is_binary = (reg_b & RTC_BINARY_MSK) != 0;
    is_pm = (!is_24_hour && (hour & RTC_PM_MSK) != 0);

    if (!is_binary) {
        second = bcd_to_bin((uint8_t) second);
        minute = bcd_to_bin((uint8_t) minute);
        hour = bcd_to_bin((uint8_t) (hour & (is_24_hour ? 0xFF : ~RTC_PM_MSK)));
        day = bcd_to_bin((uint8_t) day);
        month = bcd_to_bin((uint8_t) month);
        year = bcd_to_bin((uint8_t) year);
    } else if (!is_24_hour) {
        hour &= ~RTC_PM_MSK;
    }

    if (!is_24_hour) {
        if (hour == 12) hour = 0;
        if (is_pm) hour += 12;
    }

    datetime->second = (uint8_t) second;
    datetime->minute = (uint8_t) minute;
    datetime->hour = (uint8_t) hour;
    datetime->day = (uint8_t) day;
    datetime->month = (uint8_t) month;
    datetime->year = (uint8_t) year;
    datetime->valid = 1;

    return 0;
}

void rtc_update_cached_datetime(void) {
    if (rtc_read_datetime(&cached_datetime) != 0) {
        cached_datetime.valid = 0;
    }
}

void rtc_load_last_played(void) {
    FILE *file = fopen(LAST_PLAYED_FILE, "r");

    has_last_played = 0;
    if (file == NULL) return;

    if (fscanf(file, "%hhu %hhu %hhu %hhu %hhu %hhu",
               &last_played.day, &last_played.month, &last_played.year,
               &last_played.hour, &last_played.minute, &last_played.second) == 6) {
        last_played.valid = 1;
        has_last_played = 1;
    }

    fclose(file);
}

void rtc_save_last_played(void) {
    FILE *file;

    rtc_update_cached_datetime();
    if (!cached_datetime.valid) return;

    last_played = cached_datetime;
    has_last_played = 1;

    file = fopen(LAST_PLAYED_FILE, "w");
    if (file == NULL) return;

    fprintf(file, "%u %u %u %u %u %u\n",
            last_played.day, last_played.month, last_played.year,
            last_played.hour, last_played.minute, last_played.second);
    fclose(file);
}

void rtc_format_last_played(char *buffer, size_t size) {
    long long elapsed_seconds;
    long long elapsed_minutes;
    long long elapsed_hours;
    long long elapsed_days;

    if (buffer == NULL || size == 0) return;

    if (!has_last_played) {
        snprintf(buffer, size, "NO LAST PLAYED");
        return;
    }

    if (!cached_datetime.valid) {
        snprintf(buffer, size, "RTC UNAVAILABLE");
        return;
    }

    elapsed_seconds = rtc_datetime_to_seconds(&cached_datetime) -
                      rtc_datetime_to_seconds(&last_played);
    elapsed_minutes = elapsed_seconds / 60;
    if (elapsed_minutes < 1) {
        elapsed_minutes = 1;
    }

    if (elapsed_minutes < 60) {
        if (elapsed_minutes == 1) {
            snprintf(buffer, size, "1 MIN AGO");
        } else {
            snprintf(buffer, size, "%lld MIN AGO", elapsed_minutes);
        }
        return;
    }

    elapsed_hours = elapsed_minutes / 60;
    if (elapsed_hours < 24) {
        if (elapsed_hours == 1) {
            snprintf(buffer, size, "1 HR AGO");
        } else {
            snprintf(buffer, size, "%lld HRS AGO", elapsed_hours);
        }
        return;
    }

    elapsed_days = elapsed_hours / 24;
    if (elapsed_days == 1) {
        snprintf(buffer, size, "1 DAY AGO");
    } else {
        snprintf(buffer, size, "%lld DAYS AGO", elapsed_days);
    }
}
