#include "rtc.h"

static int read_reg(uint8_t reg, uint8_t *value) {
  if (value == NULL) return 1;
  if (lcom_port_write8(RTC_ADDR_REG, reg) != LCOM_OK) return 1;
  return lcom_port_read8(RTC_DATA_REG, value) == LCOM_OK ? 0 : 1;
}

static uint8_t bcd_to_binary(uint8_t value) {
  return (uint8_t)((value & 0x0Fu) + ((value >> 4) * 10u));
}

static int rtc_binary_mode(bool *binary) {
  uint8_t reg_b = 0;
  if (read_reg(RTC_REG_B, &reg_b) != 0) return 1;
  *binary = (reg_b & RTC_DM) != 0;
  return 0;
}

int rtc_read_date(rtc_date *date) {
  if (date == NULL) return 1;
  bool binary = false;
  if (rtc_binary_mode(&binary) != 0) return 1;
  if (read_reg(RTC_REG_DAY, &date->day) != 0 ||
      read_reg(RTC_REG_MONTH, &date->month) != 0 ||
      read_reg(RTC_REG_YEAR, &date->year) != 0) {
    return 1;
  }
  if (!binary) {
    date->day = bcd_to_binary(date->day);
    date->month = bcd_to_binary(date->month);
    date->year = bcd_to_binary(date->year);
  }
  return 0;
}

int rtc_read_time(rtc_time *time) {
  if (time == NULL) return 1;
  bool binary = false;
  if (rtc_binary_mode(&binary) != 0) return 1;
  if (read_reg(RTC_REG_SECONDS, &time->second) != 0 ||
      read_reg(RTC_REG_MINUTES, &time->minute) != 0 ||
      read_reg(RTC_REG_HOURS, &time->hour) != 0) {
    return 1;
  }
  if (!binary) {
    time->second = bcd_to_binary(time->second);
    time->minute = bcd_to_binary(time->minute);
    time->hour = bcd_to_binary(time->hour);
  }
  return 0;
}
