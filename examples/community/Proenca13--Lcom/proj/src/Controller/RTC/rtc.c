#include "rtc.h"
Time_Irl timeIrl;
int rtc_hook_id = 5;
int read_rtc(uint8_t command, uint8_t *st){
    if(sys_outb(RTC_ADDR_REG,command)!=0)return 1;
    return util_sys_inb(RTC_DATA_REG,st);
}
int rtc_update(){
    uint8_t st;
    if(read_rtc(0x0A,&st)!=0)return 1;
    return st & UPDATING;
}
uint8_t binary_converter(uint8_t num) {
    uint8_t result = ((num >> 4) * 10);
    result += (num & 0xF);
    return result;
}
int rtc_update_time(){
    if(rtc_update()!=0)return 1;
    uint8_t status;
    if(read_rtc(SECONDS,&status)!=0)return 1;
    timeIrl.seconds = binary_converter(status);
    if(read_rtc(MINUTES,&status)!=0)return 1;
    timeIrl.minutes = binary_converter(status);
    if(read_rtc(HOURS,&status)!=0)return 1;
    timeIrl.hours = binary_converter(status);
    if(read_rtc(DAY_OF_THE_WEEK,&status)!=0)return 1;
    timeIrl.day_week = binary_converter(status);
    if(read_rtc(DAY_OF_THE_MONTH,&status)!=0)return 1;
    timeIrl.day = binary_converter(status);
    if(read_rtc(MONTH,&status)!=0)return 1;
    timeIrl.month = binary_converter(status);
    if(read_rtc(YEAR,&status)!=0)return 1;
    timeIrl.year = binary_converter(status);
    return 0;
}
int rtc_subscribe_int(){
    return sys_irqsetpolicy(IRQ_RTC,IRQ_REENABLE,&rtc_hook_id);
}
int rtc_unsubscribe_int(){
    return sys_irqrmpolicy(&rtc_hook_id);
}

