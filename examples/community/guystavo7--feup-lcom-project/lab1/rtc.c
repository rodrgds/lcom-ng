#include "rtc.h"
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include <stdio.h>

#define TODO return -1

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 0x0A
#define RTC_REG_B 0x0B
#define RTC_RG_SECONDS 0x00
#define RTC_RG_MIN 0x02
#define RTC_RG_HOUR 0x04
#define RTC_REG_DAY 0x07
#define RTC_REG_MONTH 0x08
#define RTC_REG_YEAR 0x09
#define RTC_UIP_MSK (1 << 7)
#define RTC_DM_MSK (1 << 2)

static int bdc_to_bin(uint8_t bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

int getUIP(uint32_t* uip) {
    if (sys_outb(RTC_ADDR_REG, RTC_REG_A) || sys_inb(RTC_DATA_REG, uip)) return -1;
    *uip &= RTC_UIP_MSK;

    return 0;
}

int readRTC(uint8_t reg, uint8_t* value) {
    uint32_t uip;

    do {
        if (getUIP(&uip)) return -1;
        tickdelay(micros_to_ticks(20000));

    } while (uip);

    uint32_t v;
    if (sys_outb(RTC_ADDR_REG, reg) || sys_inb(RTC_DATA_REG, &v)) return -1;
    *value = v;

    return 0;
}

int rtc_read_date(rtc_date* date) {
    uint8_t day, month, year, isBDC;
    if (readRTC(RTC_REG_DAY, &day) || readRTC(RTC_REG_MONTH, &month) || readRTC(RTC_REG_YEAR, &year) || readRTC(RTC_REG_B, &isBDC)) return -1;
    if (!(isBDC & RTC_DM_MSK)) {
        day = bdc_to_bin(day);
        month = bdc_to_bin(month);
        year = bdc_to_bin(year);
    }

    date->day = day;
    date->month = month;
    date->year = year;

    return 0;
}

int rtcGetSeconds(){
    uint8_t sec, min, hour, day, isBDC;
    if (readRTC(RTC_RG_SECONDS, &sec) || readRTC(RTC_RG_MIN, &min) || readRTC(RTC_RG_HOUR, &hour) || readRTC(RTC_REG_DAY, &day) || readRTC(RTC_REG_B, &isBDC)) return -1;
    if (!(isBDC & RTC_DM_MSK)) {
        sec = bdc_to_bin(sec);
        min = bdc_to_bin(min);
        hour = bdc_to_bin(hour);
        day = bdc_to_bin(day);
        isBDC = bdc_to_bin(isBDC);
    }

    return sec + min * 60 + hour * 3600 + day * 24 * 3600;
}