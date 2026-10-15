#include "rtc.h"
#include <minix/syslib.h>
#include <lcom/lcf.h>

#define TODO return -1

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 0x0A
#define RTC_REG_B 0x0B
#define RTC_REG_SECONDS 0x00
#define RTC_REG_MINUTES 0x02
#define RTC_REG_HOURS 0x04
#define RTC_REG_DAY 0x07
#define RTC_REG_MONTH 0x08
#define RTC_REG_YEAR 0x09
#define RTC_UIP_MSK (1 << 7)
#define RTC_DM_MSK (1 << 2)
#define RTC_24H_MSK (1 << 1)
#define RTC_PM_MSK (1 << 7)

uint8_t bcd_to_bin(uint8_t bcd) { 
  return ((bcd >> 4)*10) + (bcd & 0x0F);
}

int rtc_read_date(rtc_date *date) { 

  if(date == NULL) return 1;

  //Wait until the clock is not being written to
  uint32_t uipA;
  do {
    if(tickdelay(micros_to_ticks(1000))) return 1;
    if (sys_outb(RTC_ADDR_REG, RTC_REG_A)) return 1; 
    if (sys_inb(RTC_DATA_REG, &uipA)) return 1;
  } while (uipA & RTC_UIP_MSK);

  uint32_t day;
  if (sys_outb(RTC_ADDR_REG, RTC_REG_DAY)) return 1; 
  if (sys_inb(RTC_DATA_REG, &day)) return 1;

  uint32_t month;
  if (sys_outb(RTC_ADDR_REG, RTC_REG_MONTH)) return 1; 
  if (sys_inb(RTC_DATA_REG, &month)) return 1;

  uint32_t year;
  if (sys_outb(RTC_ADDR_REG, RTC_REG_YEAR)) return 1;
  if (sys_inb(RTC_DATA_REG, &year)) return 1;

  //Checks if the date is in BCD form, if it is turns it into binary
  uint32_t uipB;
  if (sys_outb(RTC_ADDR_REG, RTC_REG_B)) return 1; 
  if (sys_inb(RTC_DATA_REG, &uipB)) return 1;
  if (!(uipB & RTC_DM_MSK)) {
    day = bcd_to_bin((uint8_t)day);
    month = bcd_to_bin((uint8_t)month);
    year = bcd_to_bin((uint8_t)year);
  }

  date->day = day;
  date->month = month;
  date->year = year;

  return 0;
}

/* ---------------- Project-specific RTC helpers ---------------- */

static int rtc_wait_until_ready(void) {
  uint32_t reg_a;

  do {
    if(tickdelay(micros_to_ticks(1000))) return 1;
    if (sys_outb(RTC_ADDR_REG, RTC_REG_A)) return 1;
    if (sys_inb(RTC_DATA_REG, &reg_a)) return 1;
  } while (reg_a & RTC_UIP_MSK);

  return 0;
}

static int rtc_read_register(uint8_t reg, uint32_t *value) {
  if (value == NULL) return 1;

  if (sys_outb(RTC_ADDR_REG, reg)) return 1;
  if (sys_inb(RTC_DATA_REG, value)) return 1;

  return 0;
}

int rtc_read_time(rtc_time_t *time) {
  uint32_t seconds;
  uint32_t minutes;
  uint32_t hours;
  uint32_t reg_b;

  if (time == NULL) return 1;
  if (rtc_wait_until_ready()) return 1;

  if (rtc_read_register(RTC_REG_SECONDS, &seconds)) return 1;
  if (rtc_read_register(RTC_REG_MINUTES, &minutes)) return 1;
  if (rtc_read_register(RTC_REG_HOURS, &hours)) return 1;
  if (rtc_read_register(RTC_REG_B, &reg_b)) return 1;

  if (!(reg_b & RTC_DM_MSK)) {
    seconds = bcd_to_bin((uint8_t) seconds);
    minutes = bcd_to_bin((uint8_t) minutes);
    hours = (hours & RTC_PM_MSK) | bcd_to_bin((uint8_t) (hours & ~RTC_PM_MSK));
  }

  if (!(reg_b & RTC_24H_MSK)) {
    uint8_t is_pm = hours & RTC_PM_MSK;
    hours &= ~RTC_PM_MSK;
    if (is_pm && hours < 12) {
      hours += 12;
    }
    else if (!is_pm && hours == 12) {
      hours = 0;
    }
  }

  hours %= 24;

  time->seconds = (uint8_t) seconds;
  time->minutes = (uint8_t) minutes;
  time->hours = (uint8_t) hours;

  return 0;
}
