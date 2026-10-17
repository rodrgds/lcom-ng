#include <lcom/lcf.h>
#include <stdint.h>

#include "../macros.h"
#include "rtc.h"

int RTCBinary(uint8_t *bcd) {
    uint8_t regB = 0;
    
    if(sys_outb(REGISTER_INPUT, REG_B)){return 1;} 
    if(util_sys_inb(REGISTER_OUTPUT, &regB)){return 1;}

    if(regB & BIT(2)){*bcd = 0;}
    else {*bcd = 1;}
    return 0;
}

void toBinary(uint8_t * time) {
    *time = (((*time & 0xF0) >> 4) * 10) + (*time & 0x0F); 
}

int readRTC(uint8_t * time, uint8_t port){
    uint8_t regA = 0;
    
    do { 
        if(sys_outb(REGISTER_INPUT, REG_A)){return 1;} 
        if(util_sys_inb(REGISTER_OUTPUT, &regA)){return 1;}
    } while(regA & BIT(7));

    if (sys_outb(REGISTER_INPUT, port)) {return 1;}
    if (util_sys_inb(REGISTER_OUTPUT, time)) {return 1;}

    uint8_t bcd = 0;
    if(RTCBinary(&bcd)){return 1;}
    if(bcd){toBinary(time);}
    return 0;
}

int timeRTC(){
    uint8_t hour;
    readRTC(&hour, HOUR);
    int h = hour;
    if(h >= 6 && h < 18){
        return 0;
    } else if (h >= 18 && h < 21) {
        return 2;
    } else if ((h >= 21 && h <= 23 )|| (h >= 0 && h < 6)) {
        return 1;
    }
    return 3;
}
