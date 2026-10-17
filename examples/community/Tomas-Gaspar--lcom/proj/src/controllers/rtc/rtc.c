#include <lcom/lcf.h>

#include "macro_rtc.h"
#include "rtc.h"

static int rtc_hook_id = RTC_IRQ;
bool change_theme = false;

int (rtc_subscribe_int)(uint8_t *bit_no) {
  *bit_no = rtc_hook_id;
  return sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id);
}

int (rtc_unsubscribe_int)() {
  return sys_irqrmpolicy(&rtc_hook_id);
}

int rtc_enable_int() {
  return sys_irqenable(&rtc_hook_id);
}

int rtc_disable_int() {
  return sys_irqdisable(&rtc_hook_id);
}

int rtc_alarm_interrupts(bool enable){
  int r;
  uint8_t regB;

  if ((r = sys_outb(RTC_ADDR_REG, RTC_REG_B)) != 0)
    return r;
  if ((r = util_sys_inb(RTC_DATA_REG, &regB)) != 0)
    return r;

  if (enable){
    regB |= RTC_AIE;          //enable alarm interrupts
    if ((r = rtc_setup_alarm()) != 0)
      return r;
  } else {
    regB &= ~RTC_AIE;
  }

  if ((r = sys_outb(RTC_ADDR_REG, RTC_REG_B)) != 0)
    return r;
  if ((r = sys_outb(RTC_DATA_REG, regB)) != 0)
    return r;

  return 0;
}

int rtc_setup_alarm(){
  int r;

  uint8_t alarm_hours = get_next_alarm();
  
  if ((r = rtc_set_alarm_hour(alarm_hours)) != 0)
    return r;

  return 0;
}

int get_next_alarm(){
  int r;
  uint8_t hours, alarm_hours = 0;

  if ((r = rtc_get_hours(&hours)) != 0)
    return r;

  if (LIGHTMODE_HOURS > hours || DARKMODE_HOURS > hours){
    for (int i = hours; i <= 0x24; i++){
      if (i == LIGHTMODE_HOURS || i == DARKMODE_HOURS){
        alarm_hours = i;
        break;
      }
    }
  } else alarm_hours = (LIGHTMODE_HOURS < DARKMODE_HOURS) ? LIGHTMODE_HOURS : DARKMODE_HOURS;

  return alarm_hours;
}

int rtc_inhibit_updates(bool inhibit){
  int r;
  uint8_t regB;

  if ((r = sys_outb(RTC_ADDR_REG, RTC_REG_B)) != 0)
    return r;
  if ((r = util_sys_inb(RTC_DATA_REG, &regB)) != 0)
    return r;

  if (inhibit)
    regB |= RTC_SET;
  else regB &= ~RTC_SET;

  if ((r = sys_outb(RTC_ADDR_REG, RTC_REG_B)) != 0)
    return r;
  if ((r = sys_outb(RTC_DATA_REG, regB)) != 0)
    return r;

  return 0;
}

void rtc_ih(){
  uint8_t data;

  wait_valid_rtc();

  sys_outb(RTC_ADDR_REG, RTC_REG_C);
  util_sys_inb(RTC_DATA_REG, &data);

  if (data & RTC_AF){
    change_theme = true;

    rtc_get_hours(&data);

    if (data == LIGHTMODE_HOURS){
      rtc_set_alarm_hour(LIGHTMODE_HOURS);
    }
    else if (data == DARKMODE_HOURS){
      rtc_set_alarm_hour(DARKMODE_HOURS);
    }
  }
}

void wait_valid_rtc() {
  uint8_t regA = 0;
  do {
    rtc_disable_int();
    sys_outb(RTC_ADDR_REG, RTC_REG_A);
    util_sys_inb(RTC_DATA_REG, &regA);
    rtc_enable_int();
  } while (regA & RTC_UIP);
}

int rtc_set_alarm_hour(uint8_t hour){
  int r;

  if ((r = sys_outb(RTC_ADDR_REG, RTC_HOURS_ALARM)) != 0)
    return r;
  if ((r = sys_outb(RTC_DATA_REG, hour)) != 0)
    return r;

  if ((r = sys_outb(RTC_ADDR_REG, RTC_MINUTES_ALARM)) != 0)
    return r;
  if ((r = sys_outb(RTC_DATA_REG, 0)) != 0)
    return r;

  if ((r = sys_outb(RTC_ADDR_REG, RTC_SECONDS_ALARM)) != 0)
    return r;
  if ((r = sys_outb(RTC_DATA_REG, 0)) != 0)
    return r;
  
  return 0;
}

int rtc_get_hours(uint8_t* hours){
  int r;

  //inhibit updates to prevent inconsistent values, due to asynchronous changes
  if ((r = rtc_inhibit_updates(true)) != 0)
    return r;

  if ((r = sys_outb(RTC_ADDR_REG, RTC_HOURS)) != 0)
    return r;
  
  if ((r = util_sys_inb(RTC_DATA_REG, hours)) != 0)
    return r;

  //stop inhibiting updates
  if ((r = rtc_inhibit_updates(false)) != 0)
    return r;

  return 0;
}
