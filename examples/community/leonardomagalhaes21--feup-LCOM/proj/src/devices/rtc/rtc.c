#include "rtc.h"


int read_rtc_output(uint8_t command, uint8_t *output) {
    if (sys_outb(0x70, command) != 0){
        return 1;
    }
    if (util_sys_inb(0x71, output) != 0){
        return 1;
    }
    return 0;
}

int is_rtc_updating() {
    uint8_t result;
    if (read_rtc_output(10, &result)){
        return 1;
    }
    result &= BIT(7);
	return result;
}

int is_rtc_binary() {
    uint8_t status;
    if (read_rtc_output(11, &status)){
        return 1;
    }
    status &= BIT(2);
	return status;
}

uint8_t to_binary(uint8_t bcd_value) {
    return ((bcd_value >> 4) * 10) + (bcd_value & 0xF);
}

int read_rtc_time(){
    int is_binary = is_rtc_binary();

    if(is_rtc_updating()){
        return 1;
    }

    if(read_rtc_output(0,&output)){
        return 1;
    }
    if(is_binary){
        rtc_info.seconds = output;
    }else{
        rtc_info.seconds = to_binary(output);
    }

    if(read_rtc_output(2,&output)){
        return 1;
    }
    if(is_binary){
        rtc_info.minutes = output;
    }else{
        rtc_info.minutes = to_binary(output);
    }

    if(read_rtc_output(4,&output)){
        return 1;
    }
    if(is_binary){
        rtc_info.hours = output;
    }else{
        rtc_info.hours = to_binary(output);
    }

    if(read_rtc_output(7,&output)){
        return 1;
    }
    if(is_binary){
        rtc_info.day = output;
    }else{
        rtc_info.day = to_binary(output);
    }

    if(read_rtc_output(8,&output)){
        return 1;
    }
    if(is_binary){
        rtc_info.month = output;
    }else{
        rtc_info.month = to_binary(output);
    }

    if(read_rtc_output(9,&output)){
        return 1;
    }
    if(is_binary){
        rtc_info.year = output;
    }else{
        rtc_info.year = to_binary(output);
    }

    return 0;
}
