#include "rtc.h"

RTC_structure time_info;
int rtc_hook_id = 5;

int rtc_subscribe_interrupts() {
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id);
}

int rtc_unsubscribe_interrupts() {
    return sys_irqrmpolicy(&rtc_hook_id);
}

int get_rtc_output(uint8_t infoByte, uint8_t *answer) {
    if (sys_outb(0x70, infoByte)) return 1;
	if (util_sys_inb(0x71, answer)) return 1;
    return 0;
}

int fill_rtc_structure() {
    uint8_t result;
    if (get_rtc_output(10, &result)) return 1;
	if (result & BIT(7)) return 1;
    time_info.year = check_binary(9);
    time_info.month = check_binary(8);
    time_info.day = check_binary(7);
    time_info.hour = check_binary(4);
    time_info.minute = check_binary(2);
    return 0;
}

uint8_t check_binary(int bit) {
    uint8_t time_output;
    uint8_t result;
    if (get_rtc_output(11, &result)) return 1;
	if (!(result & BIT(2))){
        if (get_rtc_output(bit, &time_output)) return 1;
        uint8_t units = time_output & 0x0F;
        uint8_t tens = (time_output >> 4) & 0x0F;
        
        uint8_t binary_number = tens * 10 + units;
        
        return binary_number;
    }
    else {
        if (get_rtc_output(bit, &time_output)) return 1;
        return time_output;
    }
}
