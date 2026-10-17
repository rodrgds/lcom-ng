#include "rtc.h"
#include "utils.h"

#include <minix/sysutil.h>



static int rtc_hookId = 5;
static char current_time[20];
static int round_time = 40;
static int game_time = 0;


static int(rtc_read)(uint8_t reg, uint8_t *byte);

static int(rtc_write)(uint8_t reg, uint8_t byte);

void(update_progress)();

static int(inhibit_updates)(bool status);

static int(alarm_interrupts)(bool status);

static int(update_interrupts)(bool status);

static int(periodic_interrupts)(bool status);

static int(disable_interrupts)();

void(set_alarm)();

static int(rtc_second)(uint8_t *second);

static int(rtc_minute)(uint8_t *minute);

static int(rtc_hour)(uint8_t *hour);

static int(rtc_day)(uint8_t *day);

static int(rtc_month)(uint8_t *month);

static int(rtc_year)(uint8_t *year);

static int(rtc_current_time_ih)();

static void(rtc_alarm_ih)();


static int(rtc_read)(uint8_t reg, uint8_t *byte) {

    if (sys_outb(RTC_ADDR_REG, reg) != 0) {
        printf("Error: could not write to RTC_ADDR_REG\n");  //copiado
        return 1;
    }
    if (util_sys_inb(RTC_DATA_REG, byte) != 0) {
        printf("Error: could read from RTC_DATA_REG\n"); //copiado
        return 1;
    }
    return 0;
}

static int(rtc_write)(uint8_t reg, uint8_t byte) {

    if (sys_outb(RTC_ADDR_REG, reg) != 0) {
        printf("Error: could not write to RTC_ADDR_REG\n"); //copiado
        return 1;
    }
    if (sys_outb(RTC_DATA_REG, byte) != 0) {
        printf("Error: could write to RTC_DATA_REG\n"); //copiado
        return 1;
    }
    return 0;
}


int (get_game_time)(){
    return game_time;
}

void (set_game_time)(int time){
    game_time = time;
}

int (get_round_time)(){
    return round_time;
}

void (set_round_time)(int time){
    round_time = time;
}

char* (get_current_time)(){
    return current_time;
}




void(update_progress)() { //check if there is an update in progress
    uint8_t regA = 0; 
    while (true)
    {
        rtc_read(RTC_A, &regA);
        if (!(regA & RTC_REG_A_UIP)) break;
        delay_milli_seconds(RTC_DELAY); 
    }
    
}


static int(inhibit_updates)(bool status) {
    uint8_t regB = 0;
    if (rtc_read(RTC_B, &regB) != 0) return 1;

    if(status){
        regB |= RTC_REG_B_SET;
    }
    else{
        regB &= ~RTC_REG_B_SET;
    }

    if (rtc_write(RTC_B, regB) != 0) return 1;
    return 0;

}

static int(alarm_interrupts)(bool status) {

    uint8_t regB = 0;
    if (rtc_read(RTC_B, &regB) != 0) return 1;

    if(status){
        regB |= RTC_REG_B_AIE;
    }
    else{
        regB &= ~RTC_REG_B_AIE;
    }

    if (rtc_write(RTC_B, regB) != 0) return 1;
    return 0;
}


static int(update_interrupts)(bool status) {

    uint8_t regB = 0;
    if (rtc_read(RTC_B, &regB) != 0) {
        return 1;
    }

    if(status){
        regB |= RTC_REG_B_UIE;
    }
    else{
        regB &= ~RTC_REG_B_UIE;
    }

    if (rtc_write(RTC_B, regB) != 0) {
        return 1;
    }
    return 0;
}


static int(periodic_interrupts)(bool status) {
    uint8_t regB;
    if (rtc_read(RTC_B, &regB) != 0) {
        return 1;
    }

    if(status){
        regB |= RTC_REG_B_PIE;
    }
    else{
        regB &= ~RTC_REG_B_PIE;
    }

    if (rtc_write(RTC_B, regB) != 0) {
        return 1;
    }
    return 0;
}

static int(disable_interrupts)() {
  if(alarm_interrupts(false)) return 1;
  if(periodic_interrupts(false)) return 1;  
  if(update_interrupts(false)) return 1;
  return 0;
}

void(set_alarm)() {
  update_progress();
  inhibit_updates(true);
  rtc_write(RTC_HOUR_ALARM, RTC_DONT_CARE_VALUE);
  rtc_write(RTC_MINUTES_ALARM, RTC_DONT_CARE_VALUE);
  rtc_write(RTC_SECONDS_ALARM, RTC_DONT_CARE_VALUE);
  inhibit_updates(false);
}

int(rtc_subscribe)(uint8_t *bit_no) {

    if (bit_no == NULL) return 1;    
    //clear any pending interrupt
    uint8_t regC = 0;
    rtc_read(RTC_C, &regC);

    *bit_no = BIT(rtc_hookId);
    if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hookId) != 0) {
        printf("Error: could not subscribe RTC interruption\n");
        return 1;
        }
    set_alarm();
    if(disable_interrupts()) return 1;
    if(update_interrupts(true)) return 1;
    if(alarm_interrupts(true)) return 1;
    printf("RTC subscribed\n");
    return 0;
}

int(rtc_unsubscribe)() {

    if(disable_interrupts()) return 1;

    if (sys_irqrmpolicy(&rtc_hookId) != 0) {
        printf("Error: could not unsubscribe RTC interruption\n"); 
        return 1;
    }
    printf("RTC unsubscribed\n");

    return 0;
}

static int(rtc_second)(uint8_t *second) {
  return rtc_read(RTC_SECONDS, second);
}

static int(rtc_minute)(uint8_t *minute) {
  return rtc_read(RTC_MINUTES, minute);
}

static int(rtc_hour)(uint8_t *hour) {
  return rtc_read(RTC_HOUR, hour);
}

static int(rtc_day)(uint8_t *day) {
  return rtc_read(RTC_DAY_OF_MONTH, day);
}

static int(rtc_month)(uint8_t *month) {
  return rtc_read(RTC_MONTH, month);
}

static int(rtc_year)(uint8_t *year) {
  return rtc_read(RTC_YEAR, year);
}


static int(rtc_current_time_ih)() {
    update_progress();
    uint8_t hour = 0, minute = 0, second = 0, day = 0, month = 0, year = 0;

    if (rtc_second(&second) != 0 || rtc_minute(&minute) != 0 || rtc_hour(&hour) != 0 || rtc_day(&day) != 0 || rtc_month(&month) != 0 || rtc_year(&year) != 0) {
        return 1;
    }

    sprintf(current_time, "%02d/%02d/%04d %02d:%02d:%02d",
    bcd_binary(day), bcd_binary(month), 
    (uint32_t) bcd_binary(year) + RTC_MIN_YEAR, bcd_binary(hour),
    bcd_binary(minute), bcd_binary(second));
    return 0;
}


static void(rtc_alarm_ih)() { 
     round_time--;
     game_time++;
}

void(rtc_ih)() {
    uint8_t regC = 0;
    rtc_read(RTC_C, &regC);
    if (regC & RTC_REG_C_AF) rtc_alarm_ih();
    if (regC & RTC_REG_C_UF) rtc_current_time_ih();
}


