#include <lcom/lcf.h>
#include "RTC.h"

int hook_id_rtc = RTC_IRQ;
Time time_rtc;
Time previous_time_rtc;

int rtc_subscribe_int(uint8_t* bit_no) {
  *bit_no = hook_id_rtc;

  if (sys_irqsetpolicy(RTC_IRQ , IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_rtc) != 0) 
      return 1;
  
  return 0;
}

int rtc_unsubscribe_int() {

  if(sys_irqdisable(&hook_id_rtc) != 0)
		return 1;

  if (sys_irqrmpolicy(&hook_id_rtc) != 0)
    return 1;

  return 0;
}

int read_rtc_register(int registo, uint8_t* data) {

  if (sys_outb(RTC_ADDR_REG, registo) != 0)
    return 1;
  
  if (util_sys_inb(RTC_DATA_REG, data) != 0)
    return 1;

  return 0;
}

int read_hour() {
	uint8_t hour;
  if(read_rtc_register(HOUR_REGISTER, &hour) != 0)
    return -1;

  return hour;
}

int read_second() {
  uint8_t second;
  if (read_rtc_register(SECOND_REGISTER, &second) != 0)
    return -1;
  
  return second;
}

int is_in_BCD() {
	uint8_t b_register_status;
	if (read_rtc_register(B_REGISTER, &b_register_status) != 0)
		return -1;
 
	return b_register_status & BCD;
}

bool is_in_update() {
	uint8_t a_register_status;
	if (read_rtc_register(A_REGISTER, &a_register_status) != 0)
		return false;

	return a_register_status & UPDATE_IN_PROGRESS;
}

// https://stackoverflow.com/a/42340213
uint8_t bcd_to_decimal(uint8_t number_in_bcd) {
  return number_in_bcd - 6 * (number_in_bcd >> 4);
}

void read_time() {

  previous_time_rtc.hour = time_rtc.hour;
  previous_time_rtc.second = time_rtc.second;
  
  if(is_in_update())
		usleep(UPDATE_TIME); // suspend execution for UPDATE_TIME microseconds

	if (is_in_BCD() == 0) {
	time_rtc.hour = bcd_to_decimal(read_hour());
	time_rtc.second = bcd_to_decimal(read_second());
	}

	else {
		time_rtc.hour = read_hour();
		time_rtc.second = read_second();
	}

}

bool a_second_passed() {
  if (time_rtc.second > previous_time_rtc.second)
    return true;
  return false;
}



