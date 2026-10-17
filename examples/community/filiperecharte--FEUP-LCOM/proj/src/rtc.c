#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "rtc.h"
#include "utils.h"

int rtc_hook_id = RTC_IRQ;

int rtc_read_reg_data(uint8_t reg, uint8_t *data){

  sys_outb(RTC_ADDR_REG,reg); //write the address of the register we want to read

  util_sys_inb(RTC_DATA_REG,data);
  return 1;
}

int rtc_read_year(){
  uint8_t year;
  rtc_read_reg_data(RTC_YEAR,&year);
  return year;
}

int rtc_read_month(){
  uint8_t month;
  rtc_read_reg_data(RTC_MONTH,&month);
  return month;
}

int rtc_read_day(){
  uint8_t day;
  rtc_read_reg_data(RTC_DAYOFMONTH,&day);
  return day;
}

int rtc_read_hour(){
  uint8_t hour;
  rtc_read_reg_data(RTC_HOURS,&hour);
  return hour;
}

int rtc_read_minute(){
  uint8_t minute;
  rtc_read_reg_data(RTC_MINUTES,&minute);
  return minute;
}

int rtc_read_second(){
  uint8_t seconds;
  rtc_read_reg_data(RTC_SECONDS,&seconds);
  return seconds;
}

/*
void wait_valid_rtc(){
  uint8_t regA = 0;
  do{
    sys_outb(RTC_ADDR_REG,RTC_REG_C);
    util_sys_inb(RTC_DATA_REG,&regA);
  }while(regA & RTC_UIP);
}*/

