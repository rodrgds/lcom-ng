
#include <lcom/lcf.h>
#include "RTC.h"

int rtc_hook_id = 3;
timeType timeData;  //Current time
dateType dateData;  //Current date


/*
    To read/write a register of the RTC requires always:
    1. writing the address of the register to the RTC_ADDR_REG
    2. reading/writing one byte from/to the RTC_DATA_REG
*/
int rtc_read_byte(uint8_t address, uint8_t *byte) {
    if (sys_outb(RTC_ADDR_REG_PORT, address) != 0) return 1;

    if (util_sys_inb(RTC_DATA_REG_PORT, byte) != 0) return 1;
    return 0;
}



//Reading the Date or the Time of the Day

int rtc_update_date() {
     uint8_t registerA;
     uint8_t wday, mday, month, year;

    if (rtc_read_byte(REGISTER_A, &registerA) != 0) return 1;
    if (registerA & BIT(7)) return 1; //Update in progress, can't read

    //Get weekday in binary
    if (rtc_read_byte(WEEKDAY, &wday) != 0) return 1;
    if (get_binary(wday, &dateData.weekday) != 0) return 1;

    //Get monthday in binary
    if (rtc_read_byte(MONTHDAY, &mday) != 0) return 1;
    if (get_binary(mday, &dateData.monthday) != 0) return 1;

    //Get month in binary
    if (rtc_read_byte(MONTH, &month) != 0) return 1;
    if (get_binary(month, &dateData.month) != 0) return 1;

    //Get year in binary
    if (rtc_read_byte(YEAR, &year) != 0) return 1;
    if (get_binary(year, &dateData.year) != 0) return 1;

    return 0;


}


int rtc_update_time() {
     uint8_t registerA;
     uint8_t sec, min, hour;

    if (rtc_read_byte(REGISTER_A, &registerA) != 0) return 1;
    if (registerA & BIT(7)) return 1; //Update in progress, can't read

    //Get seconds in binary
    if (rtc_read_byte(SECONDS, &sec) != 0) return 1;
    if (get_binary(sec, &timeData.seconds) != 0) return 1;

    //Get minutes
    if (rtc_read_byte(MINUTES, &min) != 0) return 1;
    if (get_binary(min, &timeData.minutes) != 0) return 1;

    //Get hours
    if (rtc_read_byte(HOURS, &hour) != 0) return 1;
    if (get_binary(hour, &timeData.hours) != 0) return 1;

    return 0;

}


int get_binary(uint8_t byte, uint8_t *binary) {
     uint8_t registerB;

    //Check if its in binary by reading REGISTER_B
    if (rtc_read_byte(REGISTER_B, &registerB) != 0) return 1;
    if (registerB & BIT(2)) {
        //Already in binary
        *binary = byte;
    }
    else { //Convert BCD to binary
        *binary = ((byte >> 4) * 10) + (byte & 0xF);
    }
    return 0;

}



int rtc_subscribe_int(uint8_t *irq_set_mask) {
    if (irq_set_mask == NULL) return 1;

    *irq_set_mask = BIT(rtc_hook_id);

    if (sys_irqsetpolicy(RTC_IRQ_LINE, IRQ_REENABLE, &rtc_hook_id) != 0) return 1;  //IRQ_EXCLUSIVE to disable default IH

    return 0;
}


int rtc_unsubscribe_int() {
    if (sys_irqrmpolicy(&rtc_hook_id) != 0) return 1;
    return 0;
}



int rtc_ih() {
    if (rtc_update_time() != 0) return 1;
    if (rtc_update_date() != 0) return 1;
    return 0;
}

