#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "rtc.h"

int hook_rtc_ID = 0x04;
extern uint8_t seconds_boost;

int (rtc_subscribe_int)(uint8_t *bit_no){
    *bit_no = hook_rtc_ID;
    if(sys_irqsetpolicy(8,IRQ_REENABLE, &hook_rtc_ID) != 0) return 1;
    return 0;
}

int (rtc_unsubscribe_int)(){
    if(sys_irqrmpolicy(&hook_rtc_ID) != 0) return 1;
    return 0;
}

void (rtc_ih)(){
    uint8_t aux = 0x00;
    get_secs(&seconds_boost);
    sys_outb(RTC_ADDR_REG,RTC_REGISTER_C);
    util_sys_inb(RTC_DATA_REG, &aux);
}

int (get_mins)(uint8_t *minutes){

    uint8_t aux = 0x00;

    sys_outb(RTC_ADDR_REG,RTC_REGISTER_B);    
    uint8_t ctrl_register_B = BIT(2) | BIT(1) | BIT(4);
    sys_outb(RTC_DATA_REG,ctrl_register_B);
    sys_outb(RTC_ADDR_REG,RTC_REGISTER_MINS);

    if(util_sys_inb(RTC_DATA_REG,minutes) != 0) return 1;

    sys_outb(RTC_ADDR_REG, RTC_REGISTER_C);
    util_sys_inb(RTC_DATA_REG,&aux);
    printf("bytes 2 - %x\n", aux);
    
    return 0;
}

int (get_secs)(uint8_t *seconds){

    sys_outb(RTC_ADDR_REG,RTC_REGISTER_B);    
    uint8_t ctrl_register_B = BIT(2) | BIT(1) | BIT(4);
    sys_outb(RTC_DATA_REG,ctrl_register_B);
    sys_outb(RTC_ADDR_REG,RTC_REGISTER_SECONDS);

    if(util_sys_inb(RTC_DATA_REG,seconds) != 0) return 1;
    
    
    return 0;
}

