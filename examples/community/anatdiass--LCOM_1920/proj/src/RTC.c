#include <lcom/lcf.h>
#include <assert.h>
#include "RTC.h"

int bcd_decimal(uint8_t hex){
    assert(((hex & 0xF0) >> 4) < 10);
    assert((hex & 0x0F) < 10);
    int dec = ((hex & 0xF0) >> 4) * 10 + (hex & 0x0F);
    return dec;
}       

int getDayRTC() {
	uint8_t day = 0;
	sys_outb(RTC_ADDR_REG, 0x07);
	util_sys_inb(RTC_DATA_REG, &day);
	return bcd_decimal(day);
}

int getMonthRTC() {
	uint8_t month = 0;
	sys_outb(RTC_ADDR_REG, 0x08);
	util_sys_inb(RTC_DATA_REG, &month);
	return bcd_decimal(month);
}

int getYearRTC() {
	uint8_t year = 0;
	sys_outb(RTC_ADDR_REG, 0x09);
	util_sys_inb(RTC_DATA_REG, &year);
	return bcd_decimal(year);
}

int getSecondsRTC(){
    uint8_t seconds = 0;
    sys_outb(RTC_ADDR_REG, 0x00);
    util_sys_inb(RTC_DATA_REG, &seconds);
    return bcd_decimal(seconds);
}

int getMinutesRTC(){
    uint8_t minutes = 0;
    sys_outb(RTC_ADDR_REG, 0x02);
    util_sys_inb(RTC_DATA_REG, &minutes);
    return bcd_decimal(minutes);
}

int getHoursRTC(){
    uint8_t hours = 0;
    sys_outb(RTC_ADDR_REG, 0x04);
    util_sys_inb(RTC_DATA_REG, &hours);
    return bcd_decimal(hours);
}

