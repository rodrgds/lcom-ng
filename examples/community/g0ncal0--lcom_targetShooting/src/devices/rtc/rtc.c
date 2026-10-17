#include "rtc.h"
#include <lcom/lcf.h>
#include <stdint.h>

int rtc_hook_id = RTC_IRQ;

int(rtc_subscribe_int)(uint8_t *bit_no) {
  *bit_no = rtc_hook_id;
  if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id) != 0) {
    printf("Error in sys_irqsetpolicy inside: %s\n", __func__);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int(rtc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&rtc_hook_id) != 0) {
    printf("Error in sys_irqrmpolicy inside: %s\n", __func__);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int(read_time_register)(uint8_t reg, uint8_t *output) {
  if (sys_outb(RTC_ADDR_REG, reg) != 0) {
    printf("Error in sys_outb inside: %s\n", __func__);
    return EXIT_FAILURE;
  }

  if (util_sys_inb(RTC_DATA_REG, output) != 0) {
    printf("Error in util_sys_inb inside: %s\n", __func__);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int(rtc_is_blocked)() {

  uint8_t reb_A;
  if (read_time_register(RTC_REG_A, &reb_A) != 0) {
    printf("Error in read_time_register inside: %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (reb_A & BLOCKED_BIT)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int(rtc_is_binary)() {
  uint8_t reb_B;
  if (read_time_register(RTC_REG_B, &reb_B) != 0) {
    printf("Error in read_time_register inside: %s\n", __func__);
    return EXIT_FAILURE;
  }
  return reb_B & BINARY_BIT;
}

uint8_t(convert_bcd_to_decimal)(uint8_t *bcd) {
  *bcd = ((*bcd & 0xF0) >> 4) * 10 + (*bcd & 0x0F);
  return EXIT_SUCCESS;
}

void(rtc_update_time)(rtc_time *time) {
  static int rtcCounter; // Time only updates when this value is 60
  rtcCounter++;

  if (rtcCounter == 60) {
    time->seconds++;
    if (time->seconds == 60) {
      time->seconds = 0;
      time->minutes++;
      if (time->minutes == 60) {
        time->minutes = 0;
        time->hours++;
        if (time->hours == 24) {
          time->hours = 0;
        }
      }
    }
    rtcCounter = 0;
  }
}

int(rtc_read_time)(rtc_time *time) {
  uint8_t seconds, minutes, hours;
  int tries = 20;

  while (rtc_is_blocked()) {
    tries--;
    if (tries == 0)
      return EXIT_FAILURE;
  }
  if (read_time_register(SECONDS, &seconds) != 0)
    return 1;
  if (read_time_register(MINUTES, &minutes) != 0)
    return 1;
  if (read_time_register(HOURS, &hours) != 0)
    return 1;

  if (!rtc_is_binary()) {
    convert_bcd_to_decimal(&seconds);
    convert_bcd_to_decimal(&minutes);
    convert_bcd_to_decimal(&hours);
  }
  time->seconds = seconds;
  time->minutes = minutes;
  time->hours = hours;

  return EXIT_SUCCESS;
}
