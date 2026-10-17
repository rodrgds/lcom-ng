#include <lcom/lcf.h>

#include "rtc.h"

bool(is_dark_mode)() {
  uint32_t var;
  read_from_rtc_register(HOURS_REG, &var);

  int hours = bcd_to_decimal(var);

  // printf("%d\n", hours);

  return (hours >= 13 || hours <= 7);
}

int(read_from_rtc_register)(int register_addr, uint32_t *data) {
  /* Write the address of the register to RTC_ADDR_REG */
  if (sys_outb(RTC_ADDR_REG, register_addr))
    return 1;

  /* Read from KBC_DATA_REG */
  return sys_inb(RTC_DATA_REG, data);
}

int(read_curr_time_from_rtc)(Time *time) {
  uint32_t seconds, minutes, hours;

  if (read_from_rtc_register(SECONDS_REG, &seconds))
    return 1;

  if (read_from_rtc_register(MINUTES_REG, &minutes))
    return 1;

  if (read_from_rtc_register(HOURS_REG, &hours))
    return 1;

  time->seconds = bcd_to_decimal(seconds);
  time->minutes = bcd_to_decimal(minutes);
  time->hours = bcd_to_decimal(hours);

  return 0;
}

int bcd_to_decimal(uint32_t x) {
  return x - 6 * (x >> 4);
}
