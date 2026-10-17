#include "rtc.h"

int isRTCUpdating(){
  unsigned long regA;

  sys_outb(RTC_ADDR_REG, REGA);
  util_sys_inb_RTC(RTC_DATA_REG, &regA);
  //check UIP flag - 1 if updating
  return (regA & REGA_UIP);

}

int isBCD(){
  unsigned long regB;

  sys_outb(RTC_ADDR_REG, REGB);
  util_sys_inb_RTC(RTC_DATA_REG, &regB);

  return (!(regB & REGB_BIN));
}

unsigned long BCDtoBin(unsigned long* bcd){
  /* From:
	 * https://stackoverflow.com/questions/28133020/how-to-convert-bcd-to-decimal
   */
  return (((*bcd) & 0xF0) >> 4) * 10 + ((*bcd) & 0x0F);
}

void getDate(unsigned long *day, unsigned long *month, unsigned long *year){
  sys_outb(RTC_ADDR_REG, DAY);
  util_sys_inb_RTC(RTC_DATA_REG, day);

  sys_outb(RTC_ADDR_REG, MONTH);
  util_sys_inb_RTC(RTC_DATA_REG, month);

  sys_outb(RTC_ADDR_REG, YEAR);
  util_sys_inb_RTC(RTC_DATA_REG, year);

  if(isBCD()){
    (*day) = BCDtoBin(day);
    (*month) = BCDtoBin(month);
    (*year) = BCDtoBin(year);
  }

  *year += CURRENT_MILLENIUM;

}

void getHour(unsigned long *hour, unsigned long *minutes,
             unsigned long *seconds){

  sys_outb(RTC_ADDR_REG, HOURS);
  util_sys_inb_RTC(RTC_DATA_REG, hour);

  sys_outb(RTC_ADDR_REG, MINUTES);
  util_sys_inb_RTC(RTC_DATA_REG, minutes);

  sys_outb(RTC_ADDR_REG, SECONDS);
  util_sys_inb_RTC(RTC_DATA_REG, seconds);

  if (isBCD()) {
    (*hour) = BCDtoBin(hour);
    (*minutes) = BCDtoBin(minutes);
    (*seconds) = BCDtoBin(seconds);
  }
/*
  if (*minutes <= 30) *minutes += 29;
  else{
    *minutes = 29 - (60 - *minutes);
    if (*hour == 23) *hour = 0;
    else *hour += 1;
  }
*/
}

int(util_sys_inb_RTC)(int port, unsigned long *value) {
  uint32_t value2 = 0x00000000;
  if (sys_inb(port, &value2) == OK)
  {
    *value = value2;
    return 0;
  }
  return 1;
}
