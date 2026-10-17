#include "rtc.h"

int rtc_hook_id = 5;
uint8_t res;
uint8_t binary_conf;
real_time curr_time;


void rtc_init(){
  binary_conf = is_binary();
  rtc_update_time();
}

int rtc_subscribe_int(){
  if(sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE , &rtc_hook_id) != 0) return 1;
  return 0;
}

int rtc_unsubscribe_int(){
  if(sys_irqrmpolicy(&rtc_hook_id) != 0) return 1;
  return 0;
}

int is_binary(){
  if(read_rtc_output(REGISTER_COUNTING, &res) != 0) return 1;
  return res & IS_BINARY;
}

int rtc_is_updating() {
  uint8_t res;
  if (read_rtc_output(REGISTER_UPDATING, &res)) return 1;
	  return res & UPDATING;
}

int read_rtc_output(uint8_t command, uint8_t *output){
  if(sys_outb(REG_INPUT, command) != 0) return 1;
    if(util_sys_inb(REG_OUTPUT, output) != 0) return 1;
  return 0;  
}

int rtc_update_time(){
  if(rtc_is_updating() != 0) return 1;
  uint8_t out;

  if (read_rtc_output(SECONDS, &out) != 0) return 1;
  curr_time.second = is_binary() ? out : to_binary(out);

  if (read_rtc_output(MINUTES, &out) != 0) return 1;
  curr_time.minute = is_binary() ? out : to_binary(out);

  if (read_rtc_output(HOURS, &out) != 0) return 1;
  curr_time.hour = is_binary() ? out : to_binary(out);

  if (read_rtc_output(DAY, &out) != 0) return 1;
  curr_time.day = is_binary() ? out : to_binary(out);

  if (read_rtc_output(MONTH, &out) != 0) return 1;
  curr_time.month = is_binary() ? out : to_binary(out);

  if (read_rtc_output(YEAR, &out) != 0) return 1;
  curr_time.year = is_binary() ? out : to_binary(out);

  return 0;


}



