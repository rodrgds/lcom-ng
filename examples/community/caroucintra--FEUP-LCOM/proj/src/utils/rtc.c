#include <lcom/lcf.h>
#include "rtc.h"

//Gets

bool update_in_process(){
    uint8_t reg = get_mode(REG_A);

    if(reg & UIP){
        return true;
    }
    return false;
}

uint8_t get_mode(uint8_t rtc_register){
    uint8_t value;
    sys_outb(RTC_ADDR_REG, rtc_register);
    util_sys_inb(RTC_DATA_REG, &value);
    return value;
}

uint8_t get_second(){
    while(update_in_process()){}
    return get_mode(SEC);
}

uint8_t get_minute(){
    while(update_in_process()){}
    return get_mode(MNT);
}

uint8_t get_hour(){
    while(update_in_process()){}
    return get_mode(HOUR);
}

uint8_t get_day(){
    while(update_in_process()){}
    return get_mode(DAY_OF_MONTH);
}

uint8_t get_day_of_week(){
    while(update_in_process()){}
    return get_mode(DAY_OF_WEEK);
}

uint8_t get_month(){
    while(update_in_process()){}
    return get_mode(MONTH);
}

uint8_t get_year(){ 
    while(update_in_process()){}
    return get_mode(YEAR);
}

//Sets
int init_mode(){
    
    uint8_t value = get_mode(REG_B);
    value |= BCD_MODE | HOURS_24_MODE | SQWE;
    if(sys_outb(RTC_DATA_REG, value)) return 1;

    return 0;
}

int disable_updates(){
    uint8_t value = get_mode(REG_B);
    value |= INHIBIT_UPDATES;
    if(sys_outb(REG_B, value)) return 1;

    return 0;
}

int enable_updates(){
    uint8_t value = get_mode(REG_B);
    value <<= 1;
    value >>= 1; 
    if(sys_outb(REG_B, value)) return 1;

    return 0;
}

Date get_date(){
    Date date;
    while(update_in_process()){} //just waits for update end

    date.second = get_second();
    date.minute = get_minute();
    date.hour = get_hour();
    date.day = get_day();
    date.day_of_week = get_day_of_week();
    date.month = get_month();
    date.year = get_year();

    return date;
}
