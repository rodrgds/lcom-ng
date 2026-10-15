#include <lcom/lcf.h>

#include "rtc.h"

static int hook_id;

int (rtc_read_data)(uint8_t address, uint8_t *data) {
    if (sys_outb(RTC_ADDR_REG, address)) {
        return 1;
    }
    if (util_sys_inb(RTC_DATA_REG, data)) {
        return 1;
    }

    return 0;
}

int (rtc_write_data)(uint8_t address, uint8_t data) {
    if (sys_outb(RTC_ADDR_REG, address)) {
        return 1;
    }
    if (sys_outb(RTC_DATA_REG, data)) {
        return 1;
    }

    return 0;
}

int (rtc_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) {
        return 1;
    }

    hook_id = RTC_IRQ;
    *bit_no = hook_id;

    if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &hook_id)) {
        return 1;
    }

    return 0;
}

int (rtc_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id)) {
        return 1;
    }

    return 0;
}

int (rtc_is_updating)() {
    uint8_t st;
    if (rtc_read_data(RTC_UPDATE_REG_A, &st)) {
        return 2;
    }

	return st & RTC_UPDATE;
}

int (rtc_is_binary)() {
    uint8_t st;
    if (rtc_read_data(RTC_BINARY_REG_B, &st)) {
        return 2;
    }

	return st & RTC_BINARY;
}

uint8_t (bcd_to_binary)(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0xF);
}

int (get_time_date_info)(date_time_info *info) {
    int is_updating;
    do {
        is_updating = rtc_is_updating();

        if (is_updating == 2) {
            return 1;
        }
    } while (is_updating == 1);
    
    uint8_t sec, min, hour, day, month, year;
    int is_binary = rtc_is_binary();
    if (is_binary == 2) {
        return 1;
    }
    
    if (rtc_read_data(RTC_SECOND_ADDR_LOC, &sec)) {
        return 1;
    }

    if (rtc_read_data(RTC_MINUTE_ADDR_LOC, &min)) {
        return 1;
    }

    if (rtc_read_data(RTC_HOUR_ADDR_LOC, &hour)) {
        return 1;
    }

    if (rtc_read_data(RTC_DAY_ADDR_LOC, &day)) {
        return 1;
    }

    if (rtc_read_data(RTC_MONTH_ADDR_LOC, &month)) {
        return 1;
    }

    if (rtc_read_data(RTC_YEAR_ADDR_LOC, &year)) {
        return 1;
    }

    if (is_binary) {
        info->sec = sec;
        info->min = min;
        info->hour = hour;
        info->day = day;
        info->month = month;
        info->year = year;
    } else {
        info->sec = bcd_to_binary(sec);
        info->min = bcd_to_binary(min);
        info->hour = bcd_to_binary(hour);
        info->day = bcd_to_binary(day);
        info->month = bcd_to_binary(month);
        info->year = bcd_to_binary(year);        
    }

    return 0;
}
