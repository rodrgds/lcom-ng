#include <stdint.h>
#include <stdio.h>
#include <lcom/lcf.h>
#include "rtc.h"
#include "utils.h"
#include "rtc_macros.h"

date_t date;
int rtc_hook_id; 

int (rtc_data_io)(uint8_t address, uint8_t * content, bool read) 
{    
    if (sys_outb(RTC_ADDR_REG, (uint32_t)address)) {     //writing to adress register the adress of the register we want to read
        printf("Error in sys_outb(): couldn't write in rtc adress register\n");
        return 1;
    }
    if (read) {
    
        if (util_sys_inb(RTC_DATA_REG, content)) {     //reading the data register 
            printf("Error in util_sys_inb(): couldn't read rtc data register\n");
            return 1;
        }          
    }
    else {
        if (sys_outb(RTC_DATA_REG, (uint32_t)*content)) {   //writing in data register
            printf("Error in sys_outb(): couldn't write to rtc data register\n");
            return 1;
        }
    }
    
    
    return 0;
}

int (read_date)() {
    uint8_t seconds, minutes, hours, day_month, day_week, month, year;
    uint8_t reg_A;
    if(rtc_data_io(REGISTER_A, &reg_A, true)) {
        printf("Error in rtc_data_io(): couldn't read register A to read date safely\n");       
        return 1;
    }
    while(true) {
        if (reg_A & REG_A_UIP) {   //if there is an update
            continue;
        }
        if (rtc_data_io(SECONDS_ADDR, &seconds, true)) {
            printf("Error in rtc_data_io(): couldn't get seconds \n");
            return 1;
        }
        if (rtc_data_io(MINUTES_ADDR, &minutes, true)) {
            printf("Error in rtc_data_io(): couldn't get minutes \n");
            return 2;
        }
        if (rtc_data_io(HOURS_ADDR, &hours, true))  {
            printf("Error in rtc_data_io(): couldn't get hours \n");
            return 3;
        }
        if (rtc_data_io(DAY_WEEK_ADDR, &day_week, true))  {
            printf("Error in rtc_data_io(): couldn't get day of week \n");
            return 4;
        }
    
        if (rtc_data_io(DAY_MONTH_ADDR, &day_month, true))  {
            printf("Error in rtc_data_io(): couldn't get day of week \n");
            return 5;
        }
    
        if (rtc_data_io(MONTH_ADDR, &month, true))  {
            printf("Error in rtc_data_io(): couldn't get day of week \n");
            return 6;
        }
    
        if (rtc_data_io(YEAR_ADDR, &year, true))  {
            printf("Error in rtc_data_io(): couldn't get day of week \n");
            return 7;
        }
        break;          
    }
    bcd_conversion(&seconds);
    date.seconds = seconds;
    bcd_conversion(&minutes);
    date.minutes =minutes;
    bcd_conversion(&hours);
    date.hours = hours;
    bcd_conversion(&day_month);
    date.day_month = day_month;
    bcd_conversion(&day_week);
    date.day_week = day_week;
    bcd_conversion(&month);
    date.month = month;
    bcd_conversion(&year);
    date.year = year;
    return 0;
}



int bcd_conversion(uint8_t *bcd_num) {
    uint8_t registerB;
    if (rtc_data_io(REGISTER_B, &registerB, true)) {
        printf("Error retrieving register B content\n");
        return 1;
    }
    if (registerB & REG_B_DM) {     //if registerB has bit2 set to 1
        printf("Error converting from bcd to binary: rtc is already giving values as binary\n");
        return 2;
    }

    *bcd_num = ((*bcd_num) & 0xf) +10*((*bcd_num)>>4);
    return 0;

}

void print_date() {
    printf("day of month = %d\n", date.day_month);
    printf("day of week = %d\n", date.day_week);
    printf("year = %d\n", date.year);
    printf("month = %d\n", date.month);
    printf("hours = %d\n", date.hours);
    printf("minutes = %d\n", date.minutes);
    printf("seconds = %d\n", date.seconds);
}

int(rtc_subscribe_int)(int *irq_set) {
    rtc_hook_id = RTC_IRQ_LINE;
    *irq_set = BIT(rtc_hook_id);

    if (sys_irqsetpolicy(RTC_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id)) {
        printf("sys_irqsetpolicy: Error setting policy\n");
        return 1;
    }
    uint8_t reg_C;
    rtc_data_io(REGISTER_C, &reg_C, true);
    reg_C = reg_C | REG_C_IRQF;
    rtc_data_io(REGISTER_C, &reg_C, false);
    rtc_enable_interrupts();
    

  return 0;
}

int(rtc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&rtc_hook_id)) {
    printf("sys_irqrmpolicy: Error unsubscribing\n");
    return 1;
  }
  return 0;
}

int (rtc_ih)() {
    uint8_t reg_C;
    if(rtc_data_io(REGISTER_C, &reg_C, true)) {
        printf("Error in rtc_data_io(): couldn't read register C to process interrupts\n");
        return 1;
    }

    if (reg_C & REG_C_AF) {

        printf("Alarm interupt\n");
        
    }
    else if(reg_C & REG_C_PF) {
        printf("Periodic interrupt\n");
    }
    else if(reg_C & REG_C_UF) {
        printf("Update interrupt\n");
    }
    return 0;


}

int(rtc_enable_interrupts)() {
    uint8_t reg_B;
    if (rtc_data_io(REGISTER_B, &reg_B, true)) {
        printf("Error in rtc_data_io(): couldn't read register B to enable interrupts\n");
        return 1;
    }

    reg_B = reg_B | REG_B_PIE | REG_B_UIE | REG_B_AIE;

    if(reg_B & REG_B_SET) {  //if updates are inhibited
        reg_B = reg_B & ~ REG_B_SET;
    }
    if (rtc_data_io(REGISTER_B, &reg_B, false)) {
        printf("Error in rtc_data_io(): couldn't write register B to enable interrupts\n");
        return 1;
    }
    return 0;
}

