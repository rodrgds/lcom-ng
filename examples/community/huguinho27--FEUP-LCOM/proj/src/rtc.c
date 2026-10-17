#include "rtc.h"

void wait_valid_rtc()
{
	unsigned long regA = 0;
	do
	{
		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		sys_inb(RTC_DATA_REG, &regA);
	} while (regA & RTC_UIP);
}

unsigned long readSecondsFromRTC()
{
	unsigned long seconds;
	wait_valid_rtc();
	sys_outb(RTC_ADDR_REG, RTC_SECONDS_ADDR);
	sys_inb(RTC_DATA_REG, &seconds);
	return seconds;
}

unsigned long convertBCD(unsigned long seconds)
{
	seconds = (((seconds & 0xF0) >> 4) * 10 + (seconds & 0x0F));
	return seconds;
}
