#include <lcom/lcf.h>
#include "rtc.h"

static RTC_Time current_time;

static uint8_t bcd_to_binary(uint8_t bcd) {
  return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

static int rtc_read_reg(uint8_t reg, uint8_t *val) {
  if (sys_outb(0x70, reg) != OK) {
    return 1;
  }
  uint32_t data;
  if (sys_inb(0x71, &data) != OK) {
    return 1;
  }
  *val = (uint8_t) data;
  return 0;
}

static bool rtc_is_updating(void) {
  uint8_t status;
  if (rtc_read_reg(10, &status) != 0) {
    return false; /* Return false on failure to prevent freezing */
  }
  return (status & 0x80) != 0;
}

void rtc_init(void) {
  current_time.seconds = 0;
  current_time.minutes = 0;
  current_time.hours = 0;
  current_time.day = 1;
  current_time.month = 1;
  current_time.year = 26;
}

RTC_Time rtc_get_time(void) {
  int retries = 3;
  while (rtc_is_updating() && retries > 0) {
    tickdelay(micros_to_ticks(50));
    retries--;
  }

  uint8_t sec, min, hr, day, month, year, reg_b;

  if (rtc_read_reg(0, &sec) != 0) return current_time;
  if (rtc_read_reg(2, &min) != 0) return current_time;
  if (rtc_read_reg(4, &hr) != 0) return current_time;
  if (rtc_read_reg(7, &day) != 0) return current_time;
  if (rtc_read_reg(8, &month) != 0) return current_time;
  if (rtc_read_reg(9, &year) != 0) return current_time;
  if (rtc_read_reg(11, &reg_b) != 0) return current_time;

  /* Convert from BCD if Register B bit 2 is 0 */
  if (!(reg_b & 0x04)) {
    sec = bcd_to_binary(sec);
    min = bcd_to_binary(min);
    hr = bcd_to_binary(hr);
    day = bcd_to_binary(day);
    month = bcd_to_binary(month);
    year = bcd_to_binary(year);
  }

  /* Convert from 12-hour format if Register B bit 1 is 0 */
  if (!(reg_b & 0x02)) {
    bool pm = (hr & 0x80) != 0;
    hr = hr & 0x7F; /* Remove PM indicator bit */
    if (pm && hr != 12) {
      hr += 12;
    }
    else if (!pm && hr == 12) {
      hr = 0;
    }
  }

  current_time.seconds = sec;
  current_time.minutes = min;
  current_time.hours = hr;
  current_time.day = day;
  current_time.month = month;
  current_time.year = year;

  return current_time;
}
