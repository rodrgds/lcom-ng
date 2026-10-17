#include <lcom/lcf.h>
#include "rtc.h"


bool RTC_is_Updating(){
    uint8_t regA;
    sys_outb(RTC_ADDR_REG,REG_A);
    util_sys_inb(RTC_DATA_REG,&regA);
    return (regA & UIP_SET );
}

bool is_binary(){
    uint8_t regB;
    sys_outb(RTC_ADDR_REG,REG_B);
    util_sys_inb(RTC_DATA_REG,&regB);
    return (regB & BINARY );
}

int bcd_to_binary(int num){
    return  ((num& 0xF0 )>>4 ) * 10 + (num & 0x0F);
}

Date * get_date(){
    Date * date=malloc(sizeof(Date));
    if(RTC_is_Updating())sleep(WAIT_UPDATE*0.000001);
    sys_outb(RTC_ADDR_REG, SEC_REG);
	util_sys_inb(RTC_DATA_REG, &date->second);
	sys_outb(RTC_ADDR_REG, MIN_REG);
	util_sys_inb(RTC_DATA_REG, &date->minute);
	sys_outb(RTC_ADDR_REG, HOUR_REG);
	util_sys_inb(RTC_DATA_REG, &date->hour);
    sys_outb(RTC_ADDR_REG, DAY_REG);
	util_sys_inb(RTC_DATA_REG, &date->day);
	sys_outb(RTC_ADDR_REG, MONTH_REG);
	util_sys_inb(RTC_DATA_REG, &date->month);
	sys_outb(RTC_ADDR_REG, YEAR_REG);
	sys_inb(RTC_DATA_REG, &date->year);
    if(!is_binary()){
        date->second=bcd_to_binary(date->second);
        date->minute=bcd_to_binary(date->minute);
        date->hour=bcd_to_binary(date->hour);
        date->day=bcd_to_binary(date->day);
        date->month=bcd_to_binary(date->month);
        date->year=bcd_to_binary(date->year);
    }
    return date;
}
