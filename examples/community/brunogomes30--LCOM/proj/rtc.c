#include "rtc.h"


static int rtc_hook_id;

void rtc_delay(){
    tickdelay(micros_to_ticks(10000)); // 10ms
}

int rtc_subscribe_int(){
    rtc_hook_id = RTC_IRQ;
    return sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id);
    
}


int rtc_unsubscribe_int(){
    return sys_irqrmpolicy(&rtc_hook_id);
}



int rtc_write(uint32_t reg, uint32_t data){
    if(sys_outb(RTC_ADDR_REG, reg) != OK){
        printf("RTC write reg error\n");
    }

    if(sys_outb(RTC_DATA_REG, data) != OK ){
        printf("RTC write data error\n");
    }
    return 0;
}

uint32_t rtc_read(uint32_t reg){
    uint32_t r;

    if(sys_outb(RTC_ADDR_REG, reg) != OK){
        printf("RTC read reg error\n");
        return 1;
    }
    
    if(sys_inb(RTC_DATA_REG, &r) != OK){
        printf("Error reading data\n");
        return 1;
    }
    return r;
}

int rtc_act_periodic_interrupts(uint8_t rate){
    

    uint32_t bData = rtc_read(RTC_REGISTER_B);
    bData = bData | BIT(6);
    rtc_write(RTC_REGISTER_B, bData);

    uint32_t aData = rtc_read(RTC_REGISTER_A);
    aData = (aData & 0xF0) | rate;
    rtc_write(RTC_REGISTER_A, aData);

    
    return 0;
}

int rtc_create_alarm(){
    uint32_t bData = rtc_read(RTC_REGISTER_B);
    bData = bData | BIT(7);
    rtc_write(RTC_REGISTER_B, bData);

    rtc_write(RTC_SECONDS_ALARM, 0xFF);
    rtc_write(RTC_HOURS_ALARM, 0xFF);
    rtc_write(RTC_MINUTES_ALARM, 0xFF);
    bData = bData & 0x7F;
    rtc_write(RTC_REGISTER_B, bData);
    printf("CREATE \n");
    return 0;
}

uint8_t rtc_ih(){
    uint32_t cData = rtc_read(RTC_REGISTER_C);
    uint8_t interruptsDetected = 0x0;
    if(cData & BIT(6)){
        //Periodic interrupts
        interruptsDetected |= BIT(0);
    } else if(cData & BIT(5)){
        //Alarm interrupts
        interruptsDetected |= BIT(1);
        
    } else if(cData & BIT(4)){
        //Update interrupt
        interruptsDetected |= BIT(2);
    }
    return interruptsDetected;
}

int rtc_disable_periodic(){
    uint32_t bData = rtc_read(RTC_REGISTER_B);
    bData = bData & ~BIT(6);
    rtc_write(RTC_REGISTER_B, bData);
    return 0;
}

Date getDate(){
    Date date;
    date.day = rtc_read(RTC_DAY_OF_MONTH);
    date.month = rtc_read(RTC_MONTH);
    date.year = rtc_read(RTC_YEAR);
    date.hours = rtc_read(RTC_HOURS);
    date.minutes = rtc_read(RTC_MINUTES);
    date.seconds = rtc_read(RTC_SECONDS);
    return date;
}


