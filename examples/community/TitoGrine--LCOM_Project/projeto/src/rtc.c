#include "rtc.h"

static int hook_id_rtc = 4;

int rtc_subscribe_int(uint8_t *bit_no){

    *bit_no = hook_id_rtc;

    sys_irqsetpolicy(RTC_IRQ , IRQ_REENABLE , &hook_id_rtc);
    
    return 0;
}

int rtc_unsubscribe_int(){

    sys_irqrmpolicy(&hook_id_rtc);

    return 0;
}

int rtc_handler(){
  uint32_t reg;

  sys_outb(RTC_ADDR_REG, REG_C);

  sys_inb(RTC_DATA_REG, &reg);

  // If its an alarm interruption (bit 5 is active) returns 1
  if(reg & ALARM_INTERRUPT)
    return 1;

  return 0;
}

void rtc_enable_alarm(){

  uint32_t reg;

  
  // Reads what's on register B of the RTC
  sys_outb(RTC_ADDR_REG, REG_B);
  sys_inb(RTC_DATA_REG, &reg);

  // Sets bit 5 to one to enable alarm interruptions
  reg |= ENABLE_ALARM; 

  // Writes to register B of the RTC
  sys_outb(RTC_ADDR_REG, REG_B);
  sys_outb(RTC_DATA_REG, reg);
}

void rtc_disable_alarm(){

  uint32_t reg;

  // Reads what's on register B of the RTC
  sys_outb(RTC_ADDR_REG, REG_B);
  sys_inb(RTC_DATA_REG, &reg);

  // Sets bit 5 to 0 to disable alarm interruptions
  reg &= DISABLE_ALARM; 

  // Writes to register B of the RTC
  sys_outb(RTC_ADDR_REG, REG_B);
  sys_outb(RTC_DATA_REG, reg);
}

void rtc_set_alarm(uint8_t interval){

  TIME time_config;

  // Reads current time
  do{
    get_time(&time_config);
  }while(rtc_is_updating());

  // Adds to the given interval the current seconds
  interval += time_config.seconds;

  // Sets seconds alarm register(doesn't allow for more then 60 seconds)
  sys_outb(RTC_ADDR_REG, REG_SECONDS);
  sys_outb(RTC_DATA_REG, convert_decimal(interval % 60));

  // Sets minutes alarm register to ignore
  sys_outb(RTC_ADDR_REG, REG_MINUTES);
  sys_outb(RTC_DATA_REG, 0xFF);

  // Sets hours alarm register to ignore
  sys_outb(RTC_ADDR_REG, REG_HOURS);
  sys_outb(RTC_DATA_REG, 0xFF);
}

bool rtc_is_updating() {
  uint32_t reg;

  sys_outb(RTC_ADDR_REG, REG_A);
  sys_inb(RTC_DATA_REG, &reg);

  // Returns if bit 7 is one (true) or not (false)
  return (reg & UIP);
}

bool is_BCD(){
  uint32_t reg;

  sys_outb(RTC_ADDR_REG, REG_B);
  sys_inb(RTC_DATA_REG, &reg);

  // Return if bit 2 is one (false) or not (true)
  return !(reg & DM);
}

uint32_t convert_BCD(uint32_t bcd_number){
  return (((bcd_number) & 0xF0) >> 4) * 10 + (bcd_number & 0x0F);
}

uint32_t convert_decimal(uint32_t decimal_number){
  return ((((decimal_number / 10) % 10) << 4) + (decimal_number % 10));
}

void get_date(TIME *t) {
  sys_outb(RTC_ADDR_REG, DAYS);
  sys_inb(RTC_DATA_REG, &t->day);

  sys_outb(RTC_ADDR_REG, MONTHS);
  sys_inb(RTC_DATA_REG, &t->month);

  sys_outb(RTC_ADDR_REG, YEARS);
  sys_inb(RTC_DATA_REG, &t->year);

  // If the values are in BCD, converts them to decimal
  if(is_BCD()){
    t->day = convert_BCD(t->day);
    t->month = convert_BCD(t->month);
    t->year = convert_BCD(t->year);
  }
}

void get_time(TIME *t) {

  sys_outb(RTC_ADDR_REG, SECONDS);
  sys_inb(RTC_DATA_REG, &t->seconds);

  sys_outb(RTC_ADDR_REG, MINUTES);
  sys_inb(RTC_DATA_REG, &t->minutes);

  sys_outb(RTC_ADDR_REG, HOURS);
  sys_inb(RTC_DATA_REG, &t->hours);

  // If the values are in BCD, converts them to decimal
  if(is_BCD()){
    t->seconds = convert_BCD(t->seconds);
    t->minutes = convert_BCD(t->minutes);
    t->hours = convert_BCD(t->hours);
  }
}

void get_time_string(char t[20]){
  TIME time_config;

  get_date(&time_config);
  get_time(&time_config);

  
  // Stores it in a user friendly way (adds current millenium to year). The %02d means that if the number of characters in d is less then 2, fills all previous spaces with 0's. Used when the values are less then 10 so the date doesn't look misformatted.
  sprintf(t , "%02d/%02d/%d  %02d:%02d:%02d", time_config.day, time_config.month, 2000 + time_config.year, time_config.hours, time_config.minutes, time_config.seconds);
}
