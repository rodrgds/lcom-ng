#include "../inc/rtc.h"
#include "../inc/timer.h"

extern int counter_timer;

int rtc_hook_id = 5;
int read_constant_info = 1;
rtc_time game_clock;


int (rtc_subscribe)(uint8_t *bit_no){
    if(bit_no == NULL) return 1;
    *bit_no = BIT(rtc_hook_id);
    if(sys_irqsetpolicy(IRQ_LINE_RTC, IRQ_REENABLE, &rtc_hook_id)) return 1;
    return 0;
}
int (rtc_unsubscribe)(){
    if(sys_irqrmpolicy(&rtc_hook_id)) return 1;
    return 0;
}

int (read_rtc)(uint8_t command, uint8_t *output){
    if(sys_outb(WRITE_RTC_REG,command)) return 1;
    if(util_sys_inb(READ_RTC_REG,output)) return 1;
    return 0;
}

uint8_t binary_number(uint8_t n) {
    return ((n >> 4) * 10) + (n & 0xF);
}

int (conversion)(){
    uint8_t st;
    if(read_rtc(COUNT_ST_COMMAND,&st)) return 1;
    if(st & BIT(2)) return 0;

    game_clock.seconds = binary_number(game_clock.seconds);
    game_clock.minutes = binary_number(game_clock.minutes);
    game_clock.hours = binary_number(game_clock.hours);
    game_clock.days = binary_number(game_clock.days);
    game_clock.months = binary_number(game_clock.months);
    game_clock.years = binary_number(game_clock.years);
    
    return 0;

}
int (update_rtc_time)(rtc_time *clocking){
    uint8_t response;
    if(read_rtc(SEC_COMMAND,&response)) return 1;
    clocking->seconds = response;
    if(read_rtc(MIN_COMMAND,&response)) return 1;
    clocking->minutes = response;
    if(read_rtc(HOUR_COMMAND,&response)) return 1;
    clocking->hours = response;

    if(read_rtc(DAY_COMMAND,&response)) return 1;
    clocking->days = response;
    if(read_rtc(MON_COMMAND,&response)) return 1;
    clocking->months = response;
    if(read_rtc(YEAR_COMMAND,&response)) return 1;
    clocking->years = response;

    return 0;
}

void (rtc_ih)(){
    int attempts = 10;

    while(attempts>0){
        if (update_rtc_time(&game_clock)) break;
    
        tickdelay(micros_to_ticks(2000));
        attempts--;
    }
    conversion();
}
