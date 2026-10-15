#include "controller/rtc.h"


static int rtc_hook_id = 8; //RTC IRQ is 8 so might as well use it as hook_id


int rtc_subscribe_int(uint8_t *bit_no) {
    *bit_no = rtc_hook_id;
    return sys_irqsetpolicy(8, IRQ_REENABLE, &rtc_hook_id);
}

int rtc_unsubscribe_int() {
    return sys_irqrmpolicy(&rtc_hook_id);
}


int rtc_enable_update_int() {
    uint8_t reg_b;
    //read current value of register B
    if (read_rtc_output(11, &reg_b)) return 1;
    reg_b |= BIT(4); //set UIE (Update Interrupt Enable)
    //write back to register B
    if (sys_outb(0x70, 0x0B)) return 1;
    if (sys_outb(0x71, reg_b)) return 1;
    return 0;
}


void rtc_ih() {
    uint8_t reg_c;
    //we read register C to acknowledge the interrupt
    sys_outb(0x70, 0x0C);
    util_sys_inb(0x71, &reg_c);
    read_rtc_time(); //we update rtc_info with the current time
}



int read_rtc_output(uint8_t command, uint8_t *output) {
    if (sys_outb(0x70, command) != 0) {return 1;}
    if (util_sys_inb(0x71, output) != 0) {return 1;}
    
    return 0;
}

uint8_t to_binary(uint8_t bcd_value) {
    return ((bcd_value >> 4) * 10) + (bcd_value & 0xF);
}

int is_rtc_binary() {
    uint8_t status;
    if (read_rtc_output(11, &status)) {
        return 1;
    }
    return status & BIT(2);
}

int read_rtc_time() {
    uint8_t output;
    int is_binary = is_rtc_binary();

    if (read_rtc_output(4, &output)) {return 1;}
    rtc_info.hours = is_binary ? output : to_binary(output);

    if (read_rtc_output(2, &output)) {return 1;}
    rtc_info.minutes = is_binary ? output : to_binary(output);

    return 0;
}

