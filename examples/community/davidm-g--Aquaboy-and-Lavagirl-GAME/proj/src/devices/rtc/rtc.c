#include "rtc.h"
int rtc_hook_id = 3;
bool is_binary;
rtc_values rtc;
int read_rtc_output(uint8_t cmdbyte, uint8_t *result) {
  if (sys_outb(RTC_CTL_REGISTER, cmdbyte) != 0)
    return 1; // to tell the rtc we want to read something
  if (util_sys_inb(RTC_OUT_REGISTER, result) != 0)
    return 1; // actually reading
  return 0;
}
int(rtc_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(rtc_hook_id);
  return sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id);
}

int(rtc_unsubscribe_int)() {
  return sys_irqrmpolicy(&rtc_hook_id);
}

int is_updating_rtc() {
  uint8_t output;
  if (read_rtc_output(RTC_UPDATE_REG, &output) != 0)
    return 1;
  return output & RTC_UPDATING;
}

bool is_binary_rtc() {
  uint8_t output;
  if (read_rtc_output(RTC_COUNTING_REG, &output) != 0)
    return 1;
  return output & RTC_BINARY;
}

uint8_t to_binary(uint8_t bcd_number) {
  uint8_t high_digit = bcd_number >> 4;
  uint8_t low_digit = bcd_number & 0x0F;
  uint8_t binary_number = high_digit * 10 + low_digit;
  return binary_number;
}
int rtc_update_values() {
  if (is_updating_rtc())
    return 1;
  uint8_t seconds, minutes, hours, days, months, years;
  if (read_rtc_output(READ_SECONDS, &seconds) != 0)
    return 1;
  if (read_rtc_output(READ_MINUTES, &minutes) != 0)
    return 1;
  if (read_rtc_output(READ_HOURS, &hours) != 0)
    return 1;
  if (read_rtc_output(READ_DAYS, &days) != 0)
    return 1;
  if (read_rtc_output(READ_MONTHS, &months) != 0)
    return 1;
  if (read_rtc_output(READ_YEARS, &years) != 0)
    return 1;
  if (!is_binary) { // if the rtc is incrementing in binary mode we don't need to convert the values
    seconds = to_binary(seconds);
    minutes = to_binary(minutes);
    hours = to_binary(hours);
    days = to_binary(days);
    months = to_binary(months);
    years = to_binary(years);
  }
  rtc.second = seconds;
  rtc.minute = minutes;
  rtc.hour = hours;
  rtc.day = days;
  rtc.month = months;
  rtc.year = years;
  return 0;
}

void start_rtc() {
  is_binary = is_binary_rtc();
  rtc_update_values();
}
