#include "rtc.h"

int interrupts = 0;
uint8_t rtc_binary;
uint8_t rtc_data[6];

int (rtc_writeRead)(uint8_t rtc_command, uint8_t *rtc_output){
    if (sys_outb(0x70, rtc_command) != 0) return 1;
    if(util_sys_inb(0x71, rtc_output) != 0) return 1;
    return 0;
}

int (check_rtc_mode)(){
    uint8_t rtc_status;
    if(rtc_writeRead(11, &rtc_status) != 0) return -1;
    if(rtc_status & BIT(2)) return 0; // 1 if bcd, 0 if binary
    else return 1;
}

void (rtc_start)(){
    if(check_rtc_mode() == 1) rtc_binary = 0;
    else rtc_binary = 1;
    rtc_update_time();
}

int (rtc_update_seconds)(){
    uint8_t output;
    if(rtc_writeRead(0, &output) != 0) return 1;
    rtc_data[5] = rtc_binary ? output : bcd_to_bin(output);
    return 0;
}

int (rtc_update_minutes)(){
    uint8_t output;
    if(rtc_writeRead(2, &output) != 0) return 1;
    rtc_data[4] = rtc_binary ? output : bcd_to_bin(output);
    return 0;
}

int (rtc_update_hours)(){
    uint8_t output;
    if(rtc_writeRead(4, &output) != 0) return 1;
    rtc_data[3] = rtc_binary ? output : bcd_to_bin(output);
    return 0;
}

int (rtc_update_year)(){
    uint8_t output;
    if(rtc_writeRead(9, &output) != 0) return 1;
    rtc_data[0] = rtc_binary ? output : bcd_to_bin(output);
    return 0;
}

int (rtc_update_month)(){
    uint8_t output;
    if(rtc_writeRead(8, &output) != 0) return 1;
    rtc_data[1] = rtc_binary ? output : bcd_to_bin(output);
    return 0;
}

int (rtc_update_day)(){
    uint8_t output;
    if(rtc_writeRead(7, &output) != 0) return 1;
    rtc_data[2] = rtc_binary ? output : bcd_to_bin(output);
    return 0;
}

int (rtc_update_time)(){
    uint8_t busy;
    if (rtc_writeRead(10, &busy)) return 1;

    if(! (busy & BIT(7)) ){
        
        if(rtc_update_seconds() != 0) return 1;
        if(rtc_update_minutes() != 0) return 1;
        if(rtc_update_hours() != 0) return 1;
        if(rtc_update_year() != 0) return 1;
        if(rtc_update_month() != 0) return 1;
        if(rtc_update_day() != 0) return 1;

    }
    return 0;

}

void (refresh_timer)(){
    interrupts++;
}

void (refresh_rtc)(){
    if(interrupts % 60 == 0) refresh_timer();
}

uint8_t (bcd_to_bin)(uint8_t bcd_number){
    return (bcd_number - 6 * (bcd_number >> 4));
}
