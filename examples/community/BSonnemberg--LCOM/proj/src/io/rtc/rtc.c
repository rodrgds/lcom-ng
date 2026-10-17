#include "rtc.h"

date_t datetime;

int rtc_read(uint8_t reg, uint8_t *r) {
  if(sys_outb(RTC_ADDR_REG, reg) != 0) {
    return 1;
  }
  return util_sys_inb(RTC_DATA_REG, r);
}

int update_datetime() {

  uint8_t st;
  if(rtc_read(RTC_REG_A, &st) != 0) return 1;

  if(!(st & BIT(7))) {
    if (rtc_read(0, &datetime.second) != 0) {
    return 1;
    }
    if (rtc_read(2, &datetime.minute) != 0) {
      return 1;
    }
    if (rtc_read(4, &datetime.hour) != 0) {
      return 1;
    }
    if (rtc_read(7, &datetime.day) != 0) {
      return 1;
    }
    if (rtc_read(8, &datetime.month) != 0) {
      return 1;
    }
    if (rtc_read(9, &datetime.year) != 0) {
      return 1;
    }
  }
  else return 1;


  // then parse if necessary

  uint8_t tmp;
  if (rtc_read(RTC_REG_B, &tmp) != 0) {
    return 1;
  }

  if (!(tmp & BIT(2))) {
    // in bcd, convert to binary
    datetime.second = to_binary(datetime.second);
    datetime.minute = to_binary(datetime.minute);
    datetime.hour = to_binary(datetime.hour);
    datetime.day = to_binary(datetime.day);
    datetime.month = to_binary(datetime.month);
    datetime.year = to_binary(datetime.year);
  }
  return 0;
}

uint8_t to_binary(uint8_t bcd) {
  return ((bcd >> 4) * 10) + (bcd & 0xF);
}
