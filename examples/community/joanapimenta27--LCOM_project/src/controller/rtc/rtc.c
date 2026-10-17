#include "rtc.h"

int rtc_hook_id = 4;
extern rtc_info time_info;
uint8_t bin_mode; 

void(rtc_set)() {
  bin_mode = rtc_get_bin();
  rtc_update_time();
}

int(rtc_subscribe)(uint8_t *bit_no){
    if(bit_no==NULL){return 1;}
    *bit_no = BIT(rtc_hook_id);
    return sys_irqsetpolicy(IRQ_RTC,IRQ_REENABLE,&rtc_hook_id);
}

int(rtc_unsubscribe)(){
    return sys_irqrmpolicy(&rtc_hook_id);
}

int(rtc_output)(uint8_t command, uint8_t *output) {
    if (sys_outb(RTC_ADDR_REG, command)) return 1;
    if (util_sys_inb(RTC_DATA_REG, output)) return 1;
    return 0;
}

int(rtc_get_bin)() {
  uint8_t res;
  if(rtc_output(RTC_REG_B, &res)) return 1;
  return res & DM;
}

int(rtc_get_uip)(){
  uint8_t res;
  if(rtc_output(RTC_REG_A, &res)) return 1;
  return res & UIP;
}

uint16_t (bcd_to_bin)(uint16_t bcd) {
  return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

int(rtc_update_time()) {
  uint8_t res;
  if(rtc_get_uip()) return 1;

  if(rtc_output(RTC_SECONDS, &res)) return 1;
  time_info.seconds = bin_mode ? res : bcd_to_bin(res);

  if(rtc_output(RTC_MINUTES, &res)) return 1;
  time_info.minutes = bin_mode ? res : bcd_to_bin(res);

  if(rtc_output(RTC_HOURS, &res)) return 1;
  time_info.hours = bin_mode ? res : bcd_to_bin(res);

  if(rtc_output(RTC_DAY, &res)) return 1;
  time_info.day = bin_mode ? res : bcd_to_bin(res);

  if(rtc_output(RTC_MONTH, &res)) return 1;
  time_info.month = bin_mode ? res : bcd_to_bin(res);

  if(rtc_output(RTC_YEAR, &res)) return 1;
  time_info.year = bin_mode ? res : bcd_to_bin(res);

  return 0;
}
