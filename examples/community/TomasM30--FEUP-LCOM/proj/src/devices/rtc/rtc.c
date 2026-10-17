#include "rtc.h"

int rtc_hook_id;
int read_already = 0;
uint8_t date_rtc[3], time_rtc[3];

int (rtc_subscribe_int)(uint8_t *bit_no){
    rtc_hook_id = 3;
    *bit_no = rtc_hook_id;
    printf("sure\n");

    if (sys_irqsetpolicy(RTC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &rtc_hook_id) != 0) return 1;
    if (enable_update_interrupts(true) != 0) return 1;

    return 0;
}

int (rtc_unsubscribe_int)(){
    if (sys_irqrmpolicy(&rtc_hook_id) != 0) return 1;
    if (enable_update_interrupts(false) != 0) return 1;

    return 0;
}

int (enable_update_interrupts)(bool enable){
    uint32_t reg;
    if (sys_outb(RTC_ADDR_REG, RTC_B) != 0) return 1;
    if (sys_inb(RTC_DATA_REG, &reg) != 0) return 1;

    if (enable) reg |= RTC_B_UIE;
    else reg &= ~RTC_B_UIE;

    if (sys_outb(RTC_ADDR_REG, RTC_B) != 0) return 1;
    if (sys_outb(RTC_DATA_REG, reg) != 0) return 1;

    return 0;
}

int (can_read_datetime)(){
    uint32_t reg;

    do{
        if (sys_outb(RTC_ADDR_REG, RTC_A) != 0) return 1;
        if (sys_inb(RTC_DATA_REG, &reg) != 0) return 1;
    } while(reg & RTC_A_UIP);

    return 0;
}

int (read_datetime)(uint8_t reg){
    if (can_read_datetime() != 0) return 1;

    uint32_t read_bcd;
    if (sys_outb(RTC_ADDR_REG, reg) != 0) return 1;
    if (sys_inb(RTC_DATA_REG, &read_bcd) != 0) return 1;

    uint8_t result = ((uint8_t) read_bcd >> 4) * 10;
    result = result + ((uint8_t) read_bcd & 0x0F);

    if (reg == RTC_SECONDS || reg == RTC_MINUTES || reg == RTC_HOURS){
        int time_index = reg == RTC_SECONDS ? 2 : reg == RTC_MINUTES ? 1 : 0;
        time_rtc[time_index] = result;
    }
    else if (reg == RTC_DAY || reg == RTC_MONTH || reg == RTC_YEAR){
        int date_index = reg == RTC_DAY ? 0 : reg == RTC_MONTH ? 1 : 2;
        date_rtc[date_index] = result;
    }

    return 0;
}

int (rtc_ih)(){
    if (read_already) return 0;

    uint32_t reg;

    sys_outb(RTC_ADDR_REG, RTC_C);
    sys_inb(RTC_DATA_REG, &reg);

    if (reg & RTC_C_UF){
        if (time_rtc[1] == 59 || time_rtc[1] == 0){
            read_datetime(RTC_HOURS);
        }
        read_datetime(RTC_MINUTES);
        read_datetime(RTC_SECONDS);

        read_datetime(RTC_DAY);
        read_datetime(RTC_MONTH);
        read_datetime(RTC_YEAR);
    }

    if(time_rtc[0] != 0 && time_rtc[1] != 0 && time_rtc[2] != 0){
        read_already = 1;
    }

    return 0;
}

