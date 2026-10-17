#include "rtc.h"

rtc_info time_info;

void (rtc_initialize)() {
  update_date();
}

int (read_from_rtc)(uint8_t command, uint8_t *output) {
  if (sys_outb(0x70, command) != 0) return 1;
  if (util_sys_inb(0x71, output) != 0) return 1;
  return 0;
}

int (check_update_status)() {
  uint8_t status;
  if (read_from_rtc(10, &status)) return 1;
  return status & BIT(7);
}

uint8_t (get_time_in_binary)(int selected_bit) {
  uint8_t status;
  uint8_t output;
  if (read_from_rtc(11, &status)) return 1;
  if (status & BIT(2)) {
    if (read_from_rtc(selected_bit, &output)) return 1;
    return output;
  } else {
    if (read_from_rtc(selected_bit, &output)) return 1;
    uint8_t MS = (output >> 4);
    uint8_t LS = output & 0x0F;
    return MS * 10 + LS;
  }
}

int (update_date)() {
  if (check_update_status()) return 1;
  time_info.seconds = get_time_in_binary(0);
  time_info.minutes = get_time_in_binary(2);
  time_info.hours = get_time_in_binary(4);
  time_info.day = get_time_in_binary(7);
  time_info.month = get_time_in_binary(8);
  time_info.year = get_time_in_binary(9);
  return 0;
}
