#include "rtc.h"

Rtc rtc;
int rtc_hook_id;
//extern int rtc_counter = 0;


// Methods

int (rtc_output)(uint8_t command, uint8_t *output){
    if(sys_outb(RTC_INPUT_PORT, command) != 0){
        return 1;
    }
    if(util_sys_inb(RTC_OUTPUT_PORT, output) != 0){
        return 1;
    }
    return 0;
}

int (rtc_available)(){
    uint8_t output;
    if(rtc_output(RTC_AVAILABLE, &output) != 0){
        return 1;
    }
    return output & RTC_CHECK_AVAILABLE_BIT;
}

int (rtc_binary_mode)(){
    uint8_t mode;
    if(rtc_output(RTC_COUNTING_MODE, &mode) != 0){
        return 1;
    }
    return mode & RTC_CHECK_MODE_BIT;
}

uint8_t (rtc_convert_to_binary)(uint8_t bcd){
    bcd = ((bcd >> 4) * 10) + (bcd & 0xF);
    return bcd;
}


int(rtc_update)(){
    if(rtc_available() != 0){
        return 1;
    }
    if (rtc_binary_mode() == 1){
        return 1;
    }

    uint8_t output;

    if(rtc_output(YEAR, &output) != 0){
        return 1;
    }
    rtc.year = rtc_binary_mode() ? output : rtc_convert_to_binary(output);

    if(rtc_output(MONTH, &output) != 0){
        return 1;
    }
    rtc.month = rtc_binary_mode() ? output : rtc_convert_to_binary(output);

    if(rtc_output(DAY, &output) != 0){
        return 1;
    }
    rtc.day = rtc_binary_mode() ? output : rtc_convert_to_binary(output);

    if(rtc_output(HOURS, &output) != 0){
        return 1;
    }
    rtc.hours = rtc_binary_mode() ? output : rtc_convert_to_binary(output);

    if(rtc_output(MINUTES, &output) != 0){
        return 1;
    }
    rtc.minutes = rtc_binary_mode() ? output : rtc_convert_to_binary(output);

    if(rtc_output(SECONDS, &output) != 0){
        return 1;
    }
    rtc.seconds = rtc_binary_mode() ? output : rtc_convert_to_binary(output);

    return 0;
}

