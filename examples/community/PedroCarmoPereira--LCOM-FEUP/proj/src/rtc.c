#include <lcom/lcf.h>

#include <stdint.h>

#include "i8254.h"
#include "rtc.h"

int rtc_hook_id = 8;

int rtc_subscribe_int(int *bit_no) {
  *bit_no = BIT(rtc_hook_id);
  sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id);
  return 0;
}

int rtc_unsubscribe_int() {
  sys_irqrmpolicy(&rtc_hook_id);
  return 0;
}

int rtc_enable(){
	uint32_t tmp;

	sys_outb(RTC_ADDR_REG, REGISTER_B);
	sys_inb(RTC_DATA_REG, &tmp);

	tmp = tmp | BIT(4);
  tmp = tmp & ~BIT(7);

	sys_outb(RTC_ADDR_REG, REGISTER_B);
	sys_outb(RTC_DATA_REG, tmp);

	return 0;
}

int rtc_disable(){
	unsigned tmp;

	sys_outb(RTC_ADDR_REG, REGISTER_B);
  sys_inb(RTC_DATA_REG, &tmp);

  tmp = tmp & ~BIT(4);

  sys_outb(RTC_ADDR_REG, REGISTER_B);
	sys_outb(RTC_DATA_REG, tmp);

	return 0;
}

int convert_bcd_to_int(unsigned x) {
    return x - 6 * (x >> 4);
}

Date NewDate(){
    Date date;
		date.year = 0;
		date.month = 0;
		date.day = 0;
		date.hour = 0;
		date.minute = 0;
		date.second = 0;
    return date;
}

void rtc_update(Date *date){
  uint32_t tmp;

  sys_outb(RTC_ADDR_REG, REGISTER_C);
  sys_inb(RTC_DATA_REG, &tmp);

  sys_outb(RTC_ADDR_REG, REGISTER_B);
  sys_inb(RTC_DATA_REG, &tmp);

  int bcd = ((tmp & BIT(2)) == 0);

  sys_outb(RTC_ADDR_REG, 0);
	sys_inb(RTC_DATA_REG, &tmp);
  if(bcd) date->second = convert_bcd_to_int(tmp);
  else date->second = tmp;

  sys_outb(RTC_ADDR_REG, 2);
	sys_inb(RTC_DATA_REG, &tmp);
  if(bcd) date->minute = convert_bcd_to_int(tmp);
  else date->minute = tmp;

  sys_outb(RTC_ADDR_REG, 4);
	sys_inb(RTC_DATA_REG, &tmp);
  if(bcd) date->hour = convert_bcd_to_int(tmp);
  else date->hour = tmp;

  sys_outb(RTC_ADDR_REG, 7);
	sys_inb(RTC_DATA_REG, &tmp);
  if(bcd) date->day = convert_bcd_to_int(tmp);
  else date->day = tmp;

  sys_outb(RTC_ADDR_REG, 8);
	sys_inb(RTC_DATA_REG, &tmp);
  if(bcd) date->month = convert_bcd_to_int(tmp);
  else date->month = tmp;

  sys_outb(RTC_ADDR_REG, 9);
	sys_inb(RTC_DATA_REG, &tmp);
  if(bcd) date->year = convert_bcd_to_int(tmp);
  else date->year = tmp;

}

void dateToString(Date *d, char *s){
	if (d->day < 10){
		if(d->month < 10) sprintf(s, "0%d/0%d/%d", d->day, d->month, d->year);

		else sprintf(s, "0%d/%d/%d", d->day, d->month, d->year);
	}

	else {

		if (d->month < 10) sprintf(s, "%d/0%d/%d", d->day, d->month, d->year);

		else sprintf(s, "%d/%d/%d", d->day, d->month, d->year);
	}
}

void stringToDate(Date *d, char *s){
	char tmp[3];
	char tmp2[5];
	tmp[2] = '\0';
	tmp[0] = s[0];
	tmp[1] = s[1];
	d->day =  atoi(tmp);
	tmp[0] = s[3];
	tmp[1] = s[4];
	d->month = atoi(tmp);
	tmp2[0] = s[6];
	tmp2[1] = s[7];
	tmp2[2] = s[8];
	tmp2[3] = s[9];
	d->year = atoi(tmp2);
}

