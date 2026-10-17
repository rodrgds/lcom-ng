#include "rtc.h"

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <minix/syslib.h>
#include <minix/sysutil.h>

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_REG_A     0x0A
#define RTC_REG_B     0x0B
#define RTC_REG_SEC   0x00
#define RTC_REG_MIN   0x02
#define RTC_REG_HOUR  0x04
#define RTC_REG_DAY   0x07
#define RTC_REG_MONTH 0x08
#define RTC_REG_YEAR  0x09

#define RTC_UIP_MSK (1 << 7)
#define RTC_DM_MSK  (1 << 2)

static int rtc_hook_id = 8;

static int bdc_to_bin(uint8_t bcd) {
  uint8_t first_digit, second_digit;
  first_digit = (bcd & 0xF0)>>4;
  second_digit = (bcd & 0x0F);
  return first_digit * 10 + second_digit;
}

int rtc_read_date(rtc_date *date) {
  uint32_t value;

  // Wait for the UIP bit to be 0
  if (sys_outb(RTC_ADDR_REG, RTC_REG_A) != 0) {return 1;}
  if (sys_inb(RTC_DATA_REG, &value) != 0) {return 1;}

  while (value & RTC_UIP_MSK) {
    if (sys_outb(RTC_ADDR_REG, RTC_REG_A) != 0) {return 1;}
    if (sys_inb(RTC_DATA_REG, &value) != 0) {return 1;}
  }

  // Check for BCD format
  if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != 0) {return 1;}
  if (sys_inb(RTC_DATA_REG, &value) != 0) {return 1;}
  bool is_bcd_format = !(value & RTC_DM_MSK);

  // Read the seconds
  if (sys_outb(RTC_ADDR_REG, RTC_REG_SEC) != 0) {return 1;}
  if (sys_inb(RTC_DATA_REG, &value) != 0) {return 1;}
  uint8_t seconds = (uint8_t) value;
  if (is_bcd_format) {seconds = bdc_to_bin(seconds);}
  date->seconds = seconds;

  // Read the minutes
  if (sys_outb(RTC_ADDR_REG, RTC_REG_MIN) != 0) {return 1;}
  if (sys_inb(RTC_DATA_REG, &value) != 0) {return 1;}
  uint8_t minutes = (uint8_t) value;
  if (is_bcd_format) {minutes = bdc_to_bin(minutes);}
  date->minutes = minutes;

  // Read the hours
  if (sys_outb(RTC_ADDR_REG, RTC_REG_HOUR) != 0) {return 1;}
  if (sys_inb(RTC_DATA_REG, &value) != 0) {return 1;}
  uint8_t hours = (uint8_t) value;
  if (is_bcd_format) {hours = bdc_to_bin(hours);}
  date->hours = hours;

  // Read the day
  if (sys_outb(RTC_ADDR_REG, RTC_REG_DAY) != 0) {return 1;}
  if (sys_inb(RTC_DATA_REG, &value) != 0) {return 1;}
  uint8_t day = (uint8_t) value;
  if (is_bcd_format) {day = bdc_to_bin(day);}
  date->day = day;

  // Read the month
  if (sys_outb(RTC_ADDR_REG, RTC_REG_MONTH) != 0) {return 1;}
  if (sys_inb(RTC_DATA_REG, &value) != 0) {return 1;}
  uint8_t month = (uint8_t) value;
  if (is_bcd_format) {month = bdc_to_bin(month);}
  date->month = month;

  // Read the year
  if (sys_outb(RTC_ADDR_REG, RTC_REG_YEAR) != 0) {return 1;}
  if (sys_inb(RTC_DATA_REG, &value) != 0) {return 1;}
  uint8_t year = (uint8_t) value;
  if (is_bcd_format) {year = bdc_to_bin(year);}
  date->year = year;

  return 0;
}


int (rtc_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = rtc_hook_id;
  return sys_irqsetpolicy(8, IRQ_REENABLE, &rtc_hook_id);
}

int rtc_unsubscribe_int(void) {
  return sys_irqrmpolicy(&rtc_hook_id);
}
