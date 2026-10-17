#include <lcom/lcf.h>

#include <string.h>

#include "rtc.h"

unsigned short seconds; /**< @brief the current time's seconds */
unsigned short minutes; /**< @brief the current time's minutes */
unsigned short hours;   /**< @brief the current time's hours */

char formatted_time[9]; /**< @brief character string containing a formatted representation of the current time */

int32_t rtc_global_hook_id; /**< @brief hook id used to unsubscribe interrupts from the RTC */

int(rtc_subscribe_int)(uint8_t *bit_no) {
  int hook_id = RTC_IRQ;

  *bit_no = (uint8_t) hook_id;

  int res = sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &hook_id);

  rtc_global_hook_id = hook_id;

  return res;
}

int(rtc_unsubscribe_int)() {
  return sys_irqrmpolicy((int *) &rtc_global_hook_id);
}

uint8_t(read_from_reg)(uint8_t reg_no) {
  uint8_t data;
  sys_outb(RTC_ADDR_REG, reg_no);
  util_sys_inb(RTC_DATA_REG, &data);
  return data;
}

void(write_to_reg)(uint8_t reg_no, uint8_t data) {
  sys_outb(RTC_ADDR_REG, reg_no);
  sys_outb(RTC_DATA_REG, data);
}

void(setup_update_interrupts)() {
  write_to_reg(RTC_CTRL_REG_B, 0 | ENABLE_UPDATE_INTERRUPT | BINARY_COUNTING_MODE | HOUR_RANGE_24H | DST);
}

void(handle_update_int)() { // no documentation needed, just a helper for rtc int handler
  seconds = read_from_reg(RTC_SECONDS_REG);
  minutes = read_from_reg(RTC_MINUTES_REG);
  hours = read_from_reg(RTC_HOURS_REG);

  sprintf(formatted_time, "%s%d:%s%d:%s%d", hours < 10 ? "0" : "", hours, minutes < 10 ? "0" : "", minutes, seconds < 10 ? "0" : "", seconds);
  formatted_time[8] = '\0';

  printf("Current time: %s\n", formatted_time);
}

void(rtc_int_handler)() {

  uint8_t regC = read_from_reg(RTC_CTRL_REG_C);

  if (!(regC & IQRF))
    return;

  if (regC & UPDATE_INT_FLAG)
    handle_update_int();
}

unsigned short(rtc_get_seconds)() {
  return seconds;
}

unsigned short(rtc_get_minutes)() {
  return minutes;
}

unsigned short(rtc_get_hours)() {
  return hours;
}

char *(rtc_get_formatted_time) () {
  return formatted_time;
}
