#include <lcom/lcf.h>
#include "rtc.h"

static int hook_rtc = 4;
char date_rtc[20];  // 17 max
bool isDay = true;
static uint8_t backup_config_rtc;

int (rtc_subscribe_int)(uint8_t *bit_no){

  int idhook_back = hook_rtc;

  if(sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &hook_rtc) == OK){
    *bit_no = BIT(idhook_back);
    return 0;
  }
  return 1;
}

int (rtc_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_rtc);
}

int rtc_choose_reg(uint8_t reg){
  return sys_outb(RTC_ADDR_REG, reg);
}

int rtc_write_reg(uint8_t reg, uint8_t d){
  if (rtc_choose_reg(reg))
    return 1;
  return sys_outb(RTC_DATA_REG, d);
}

int rtc_read_reg(uint8_t reg, uint8_t *d){
  if (rtc_choose_reg(reg))
    return 1;
  uint32_t data;
  if (sys_inb(RTC_DATA_REG, &data) == OK){
    *d = (uint8_t) data;
    return 0;
  }
  return 1;
}

int rtc_enable_just_update_interrupts(){
  uint8_t d;
  uint8_t tmp;
  if (rtc_read_reg(REGISTER_B, &d))
    return 1;
  d |= RTC_UIE;
  d &= (~RTC_AIE) & (~RTC_PIE) & (~RTC_SET);
  rtc_read_reg(REGISTER_C, &tmp);
  return rtc_write_reg(REGISTER_B, d);
}

int rtc_backup(){
  uint8_t d;
  if (rtc_read_reg(REGISTER_B, &d))
    return 1;
  backup_config_rtc = d;
  return 0;
}

int rtc_restore(){
  return rtc_write_reg(REGISTER_B, backup_config_rtc);
}

void rtc_initial_state_is_day(){
  uint8_t hour;
  if (!rtc_read_reg(RTC_HOURS, &hour)){
    if (hour >= 0x19 || hour <= 0x8)
      isDay = false;
    else
      isDay = true;
  }
}

void rtc_ih() {
  uint8_t regC;
  if (rtc_read_reg(REGISTER_C, &regC))
    return;
  if( regC & RTC_UF ){
    uint8_t year = 0, month = 0, day_of_month = 0, hour = 0, minute = 0, second = 0;
    uint8_t tmp;
    if (!rtc_read_reg(RTC_YEAR, &tmp))
      year = tmp;
    if (!rtc_read_reg(RTC_MONTH, &tmp))
      month = tmp;
    if (!rtc_read_reg(RTC_DAY_OF_MONTH, &tmp))
      day_of_month = tmp;
    if (!rtc_read_reg(RTC_HOURS, &tmp))
      hour = tmp;
    if (!rtc_read_reg(RTC_MINUTES, &tmp))
      minute = tmp;
    if (!rtc_read_reg(RTC_SECONDS, &tmp))
      second = tmp;
    if (hour >= 0x19 || hour <= 0x8)
      isDay = false;
    else
      isDay = true;
    sprintf(date_rtc, "%02x:%02x:%02x %02x/%02x/%02x", hour, minute, second, day_of_month, month, year);
  }
}
