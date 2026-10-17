#include <lcom/lcf.h>
#include <stdint.h>

#include <minix/sysutil.h>

#include "rtc.h"
#include "rtcVars.h"

/**
 * @brief hook id if the RTC
 * 
 */
int rtc_hook_id = RTC_IRQ;

int read_date(Date_t * date){
    uint8_t var;
    int conv_var = 0;

    sys_outb(RTC_ADDR_REG,SECONDS);
    util_sys_inb(RTC_DATA_REG,&var);
    conv_var = convert_to_decimal(var);
    if(date->seconds == conv_var) // if seconds are the same don´t continue the iteration
        return 0;
    else{
        date->seconds = conv_var;
    }

    sys_outb(RTC_ADDR_REG,MINUTES);
    util_sys_inb(RTC_DATA_REG,&var);
    date->minutes = convert_to_decimal(var);

    sys_outb(RTC_ADDR_REG,HOURS);
    util_sys_inb(RTC_DATA_REG,&var);
    date->hours = convert_to_decimal(var);

    sys_outb(RTC_ADDR_REG,MONTH_DAY);
    util_sys_inb(RTC_DATA_REG,&var);
    date->month_day = convert_to_decimal(var);

    sys_outb(RTC_ADDR_REG,MONTH);
    util_sys_inb(RTC_DATA_REG,&var);
    date->month = convert_to_decimal(var);

    sys_outb(RTC_ADDR_REG,YEAR);
    util_sys_inb(RTC_DATA_REG,&var);
    date->year = convert_to_decimal(var);

    return 0;
}

int convert_to_decimal(uint8_t bcd){
    return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

bool valid_read_date(){
    uint8_t var;
    sys_outb(RTC_ADDR_REG,RTC_REG_A);
    util_sys_inb(RTC_DATA_REG,&var);
    return !((var & RTC_UPDATE_IN_P_A) >> 6);
}

int rtc_subscribe_int(uint8_t *bit_no){
    rtc_hook_id = RTC_IRQ;
    *bit_no = rtc_hook_id;
    if(sys_irqsetpolicy(RTC_IRQ,IRQ_REENABLE,&rtc_hook_id) != 0){
        printf("RTC sys_irqsetpolicy error!\n");
        return 1;
    }
    return 0;
}

int rtc_unsubscribe_int(){
    if( sys_irqrmpolicy(&rtc_hook_id) != 0){
        printf("RTC sys_irqrmpolicy error!\n");
        return 1;
    } 
    return 0;
}

int enable_periodic_interrupts(){
    uint8_t var;

    // enable periodic interrupts
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    util_sys_inb(RTC_DATA_REG,&var);

    var |= RTC_P_INT_EN_B;

    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_outb(RTC_DATA_REG,var);

    // Choose rate selector
    sys_outb(RTC_ADDR_REG,RTC_REG_A);
    util_sys_inb(RTC_DATA_REG,&var);

    var |= INTERRUPT_INTERVAL;

    sys_outb(RTC_ADDR_REG,RTC_REG_A);
    sys_outb(RTC_DATA_REG,var);



    return 0;
}

int disable_periodic_interrupts(){
    uint8_t var;
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    util_sys_inb(RTC_DATA_REG,&var);

    var = var & 0xBF; // activate all bits except RTC_P_INT_EN_B

    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_outb(RTC_DATA_REG,var);

    return 0;
}

int read_registerC(uint8_t * regC){
    sys_outb(RTC_ADDR_REG,RTC_REG_C);
    util_sys_inb(RTC_DATA_REG,regC);

    return 0;
}
