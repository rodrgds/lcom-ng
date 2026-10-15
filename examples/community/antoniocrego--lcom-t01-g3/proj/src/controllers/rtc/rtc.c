#include "rtc.h"

int get_rtc(uint8_t command, uint8_t *rtc){
    if(sys_outb(0x70,command)){return 1;}
    if(util_sys_inb(0x71,rtc)){return 1;}
    return 0;
}
