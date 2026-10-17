#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "rtc.h"
#include "i8042.h"

bool checkUpdate() {
    uint32_t stat = 0;
    sys_outb(RTC_ADDR_REG, REG_A);
    sys_inb(RTC_DATA_REG, &stat);
    if ((stat & UIP) == 1) return true;
    return false;
}

int bcd_to_dec(uint32_t no) {
    return ((no / 16*10) + (no % 16));
}

bool (isBinary)() {
    uint32_t stat = 0;
    sys_outb(RTC_ADDR_REG, REG_B);
    sys_inb(RTC_DATA_REG, &stat);
    if ((stat & DM) == 1) return true;
    return false;
}

void (readData)(time_date* data) {
    sys_outb(RTC_ADDR_REG, DAYS);
    sys_inb(RTC_DATA_REG, &(data->day));
    sys_outb(RTC_ADDR_REG, MONTH);
    sys_inb(RTC_DATA_REG, &(data->month));
    sys_outb(RTC_ADDR_REG, YEAR);
    sys_inb(RTC_DATA_REG, &(data->year));
    sys_outb(RTC_ADDR_REG, HOURS);
    sys_inb(RTC_DATA_REG, &(data->hour));
    sys_outb(RTC_ADDR_REG, MINUTES);
    sys_inb(RTC_DATA_REG, &(data->min));
    sys_outb(RTC_ADDR_REG, SECONDS);
    sys_inb(RTC_DATA_REG, &(data->sec));

    if (!isBinary()) {
        data->year = bcd_to_dec(data->year);
        data->month = bcd_to_dec(data->month);
        data->day = bcd_to_dec(data->day);
        data->hour = bcd_to_dec(data->hour);
        data->min = bcd_to_dec(data->min);
        data->sec = bcd_to_dec(data->sec);
    }
}

time_date (getData)() {
    time_date data;
    do {
        if(!checkUpdate())
            readData(&data);
    } while(checkUpdate());
    /*while(checkUpdate())
        if(!checkUpdate()) readData(&data);*/
    return data;
}
