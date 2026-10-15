#include "rtclock.h"


int (rtc_get_out)(uint8_t value_idx, uint8_t *output) {
  if (sys_outb(RTC_ADDR_REG, value_idx) != 0) {
    return 1;
  } 

  if (util_sys_inb(RTC_DATA_REG, output) != 0) {
    return 1;
  }

  return 0;
}


/*
  Bug estranho, por algum motivo, se suspender o PC, quando voltar a abrir, a hora que vem do RTC
  é a hora atual menos a diferença de tempo entre a hora de suspensão, e hora de arcordar do PC...
*/
Datetime (rtc_read_datetime)() {
  Datetime datetime = { 
    .seconds = 0, 
    .minutes = 0, 
    .hours = 0, 
    .day = 0, 
    .month = 0, 
    .year = 0,
  };

  if (rtc_get_out(RTC_MONTHDAY, &datetime.day) != 0) {
    printf("rtc_read_datetime(): Error reading day\n");
    return datetime;
  }

  if (rtc_get_out(RTC_MONTH, &datetime.month) != 0) {
    printf("rtc_read_datetime(): Error reading month\n");
    return datetime;
  }

  if (rtc_get_out(RTC_YEAR, &datetime.year) != 0) {
    printf("rtc_read_datetime(): Error reading year\n");
    return datetime;
  }

  if (rtc_get_out(RTC_SECONDS, &datetime.seconds) != 0) {
    printf("rtc_read_datetime(): Error reading seconds\n");
    return datetime;
  }

  if (rtc_get_out(RTC_MINUTES, &datetime.minutes) != 0) {
    printf("rtc_read_datetime(): Error reading minutes\n");
    return datetime;
  }

  if (rtc_get_out(RTC_HOURS, &datetime.hours) != 0) {
    printf("rtc_read_datetime(): Error reading hours\n");
    return datetime;
  }

  datetime.day = bcd_to_binary(datetime.day);
  datetime.month = bcd_to_binary(datetime.month);
  datetime.year = bcd_to_binary(datetime.year);
  datetime.seconds = bcd_to_binary(datetime.seconds);
  datetime.minutes = bcd_to_binary(datetime.minutes);
  datetime.hours = bcd_to_binary(datetime.hours);

  // printf("SECONDS: %d, MINUTES: %d, HOURS: %d\n", datetime.seconds, datetime.minutes, datetime.hours);
  // printf("DAY: %d, MONTH: %d, YEAR: %d\n", datetime.day, datetime.month, datetime.year);

  return datetime;
}
