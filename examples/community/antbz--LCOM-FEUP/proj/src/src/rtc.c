#include <lcom/lcf.h>
#include "../headers/rtc.h"

int rtc_hookid = 4;
int rand_seed;

int subscribe_rtc(uint8_t *bit_no) {
  *bit_no = rtc_hookid;
  if (sys_irqsetpolicy(RTC_IRQ , IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hookid)) {
    return 1;
  }
  return 0;
}

int unsubscribe_rtc() {
  if (sys_irqrmpolicy(&rtc_hookid)) {
    return 1;
  }
  return 0;
}

void rtc_generate_seed()
{
  uint32_t hour, minutes, seconds;
  uint32_t check;
  do
	{
		sys_outb(RTC_ADDR_REG, RTC_CTRL_REG_A);
		sys_inb(RTC_DATA_REG, &check);
	} while(check & UIP);


	sys_outb(RTC_ADDR_REG, RTC_HOURS);
	sys_inb(RTC_DATA_REG, &hour);

  do
	{
		sys_outb(RTC_ADDR_REG, RTC_CTRL_REG_A);
		sys_inb(RTC_DATA_REG, &check);
	} while(check & UIP);

	sys_outb(RTC_ADDR_REG, RTC_MINUTES);
	sys_inb(RTC_DATA_REG, &minutes);

  do
	{
		sys_outb(RTC_ADDR_REG, RTC_CTRL_REG_A);
		sys_inb(RTC_DATA_REG, &check);
	} while(check & UIP);

	sys_outb(RTC_ADDR_REG, RTC_SECONDS);
	sys_inb(RTC_DATA_REG, &seconds);

	rand_seed = hour + minutes + seconds;
}
