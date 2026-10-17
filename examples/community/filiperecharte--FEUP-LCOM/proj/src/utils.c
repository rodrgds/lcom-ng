#include "utils.h"

int calls_counter = 0;

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
	uint8_t temp = val & (0x0F);

	*lsb = temp;
	return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
	uint8_t temp = val & (0xF0);

	*msb = temp;
	return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
	uint32_t temp;
	
	if (sys_inb(port, &temp) != 0)
		return 1;

	*value = (temp & 0x000000FF);

	return 0;
}

int util_sys_inb_counter(int port, uint32_t* value) {
	if(sys_inb(port, value) != 0)
		return 1;
	calls_counter++;

	return 0;
}

int bcd_to_decimal(uint8_t n){
  uint8_t n1 = (n & 0x0F);
  uint8_t n2 = (n & 0xF0) >> 4;

  return (n1 + n2*10);
}

void get_date(char *date){

    uint8_t year, month, day, hour, minute, second;

    day = rtc_read_day();
 
    month = rtc_read_month();

    year = rtc_read_year();

    hour = rtc_read_hour();

    minute = rtc_read_minute();

    second = rtc_read_second();

    sprintf(date, "%02x %02x 20%02x %02x %02x %02x", day, month, year, hour, minute, second);
}
