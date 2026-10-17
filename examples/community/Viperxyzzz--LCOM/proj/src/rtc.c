#include "rtc.h"


static int hook_id = RTC_IRQ;



int (rtc_subscribe_int)()
{
    return sys_irqsetpolicy(RTC_IRQ,IRQ_REENABLE,&hook_id);
}

int (read_rtc)(uint32_t address,uint8_t* input)
{
    sys_outb(RTC_ADDR_REG,address);
    util_sys_inb(RTC_DATA_REG,input);
    return 0;
}


void (get_date)(Date* date)
{



    uint8_t seconds = 0;
    read_rtc(RTC_SECONDS,&seconds);
    date->seconds = seconds;


    uint8_t minutes = 0;
    read_rtc(RTC_MINUTES,&minutes);
    date->minutes = minutes;

    uint8_t hours = 0;
    read_rtc(RTC_HOURS,&hours);
    date->hours = hours;

    uint8_t day = 0;
    read_rtc(RTC_DAY,&day);
    date->day = day;

    uint8_t month = 0;
    read_rtc(RTC_MONTH,&month);
    date->month = month;

    uint8_t year = 0;
    read_rtc(RTC_YEAR,&year);
    date->year = year;
    
}


int (rtc_unsuscribe_int)()
{
    return sys_irqrmpolicy(&hook_id);
}
