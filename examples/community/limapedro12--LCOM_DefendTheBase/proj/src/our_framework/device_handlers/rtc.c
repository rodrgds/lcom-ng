#include "rtc.h"

int rtc_input(uint8_t cmd, uint *place_to_store) {
  if(sys_outb(0x70, cmd) != 0)
    return 1;
  if(sys_inb(0x71, place_to_store) != 0)
    return 1;
  return 0;
}

int rtc_output(uint8_t cmd, uint8_t output) {
  if(sys_outb(0x70, cmd) != 0)
    return 1;
  if(sys_outb(0x71, output) != 0)
    return 1;
  return 0;
}

bool is_rtc_updating() {
  uint ret;
  if(rtc_input(10, &ret)) return 1;
	if(ret & BIT(7)) return true;
  return false;
}

bool is_rtc_in_binary(){
  uint ret;
  if(rtc_input(11, &ret)) 
    return 1;
  is_rtc_in_binary_store = ret & BIT(2);
  return ret & BIT(2);
}

int bcd_to_binary(uint8_t bcd) {
  return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}


int rtc_update_time(uint *hours, uint *minutes) {
    if (is_rtc_updating() != 0){
      return 1;
    }
    if (rtc_input(4, hours) != 0){
      return 1;
    }
    if (rtc_input(2, minutes) != 0){
      return 1;
    }

    if (!is_rtc_in_binary_store){
      *hours = bcd_to_binary(*hours);
      *minutes = bcd_to_binary(*minutes);
    }

    // *hours += 24;
    // *hours -= 6;
    // *hours %= 24;

    return 0;
}
