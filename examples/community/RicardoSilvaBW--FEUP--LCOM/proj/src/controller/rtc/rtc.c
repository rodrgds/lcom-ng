#include "rtc.h"
#include <minix/syslib.h>
#include <minix/sysutil.h>

#define TODO return -1

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 0x0A
#define RTC_REG_B 0x0B
#define RTC_REG_DAY 0x07
#define RTC_REG_MONTH 0x08
#define RTC_REG_YEAR 0x09
#define RTC_UIP_MSK (1 << 7)
#define RTC_DM_MSK (1 << 2)

static int bdc_to_bin(uint8_t bcd) {
    return (bcd & 0xF) + 10 * (bcd >> 4);
}

int rtc_read_date(rtc_date *date) {
    uint32_t day, month, year, reg_A_value, reg_B_value;
    
    while (1) {
        sys_outb(RTC_ADDR_REG, RTC_REG_A);
        sys_inb(RTC_DATA_REG, &reg_A_value);
        if (!(reg_A_value & RTC_UIP_MSK)) break;
        else tickdelay(micros_to_ticks(500));
    }

    sys_outb(RTC_ADDR_REG, RTC_REG_B);
    sys_inb(RTC_DATA_REG, &reg_B_value);

    sys_outb(RTC_ADDR_REG, RTC_REG_DAY);
    sys_inb(RTC_DATA_REG, &day);

    sys_outb(RTC_ADDR_REG, RTC_REG_MONTH);
    sys_inb(RTC_DATA_REG, &month);

    sys_outb(RTC_ADDR_REG, RTC_REG_YEAR);
    sys_inb(RTC_DATA_REG, &year);

    if (!(reg_B_value & RTC_DM_MSK)) {
        day = bdc_to_bin(day);
        month = bdc_to_bin(month);
        year = bdc_to_bin(year);
    }

    date->day = day;
    date->month = month;
    date->year = year;

    return 0;
}
