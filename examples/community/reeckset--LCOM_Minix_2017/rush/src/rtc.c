#include <minix/drivers.h>
#include <minix/syslib.h>
#include "rtc.h"


int rtc_getHours(unsigned long *hours) {
  while(1){ // wait for rtc to update
    if (rtc_available()) break;
  }
  sys_outb(RTC_ADDR_REG, RTC_HOURS);
  sys_inb(RTC_DATA_REG, hours);

  return 0;
}

int rtc_getMinutes(unsigned long *minutes) {
  while(1){ // wait for rtc to update
    if (rtc_available()) break;
  }
  sys_outb(RTC_ADDR_REG, RTC_MINUTES);
  sys_inb(RTC_DATA_REG, minutes);

  return 0;
}

int rtc_getSeconds(unsigned long *seconds) {
  while(1){ // wait for rtc to update
    if (rtc_available()) break;
  }
  sys_outb(RTC_ADDR_REG, RTC_SECONDS);
  sys_inb(RTC_DATA_REG, seconds);

  return 0;
}

int rtc_available() {

  unsigned long regData;

  sys_outb(RTC_ADDR_REG, RTC_REG_A);
  sys_inb(RTC_DATA_REG, &regData);

  if((regData & REG_A_UIP) == 0) return 1;
  else return 0;
}

unsigned long convertBCDtoBinary(unsigned long data) {
  return (data & 0x0f) + (((data & 0x0f0) >> 4) * 10);
}
