#include <minix/sysutil.h>
#include <minix/drivers.h>
#include "rtc.h"
#include "i8042.h"
#include "i8254.h"

void readRegister(int reg, unsigned long* data) {
	sys_outb(RTC_ADDR_REG, reg);
	sys_inb(RTC_DATA_REG, data);
}

void BCDtoBinary(unsigned long* bcd) {

	*bcd = ((((*bcd) & 0xF0) >> 4) * 10 + ((*bcd) & 0x0F));

}

void getDate(unsigned long *day, unsigned long *month, unsigned long *year, unsigned long *hour, unsigned long* minute, unsigned long* seconds) {
	unsigned long regAByte;
	unsigned long regBByte;
	do{
		readRegister(REGA, &regAByte);
	} while(regAByte & RTC_REGA_UIP);
	readRegister(REGB, &regBByte);

	int bcdMode = ((regBByte & RTC_REGB_DATAMODE)==0);

	readRegister(RTC_SEC, seconds);
	readRegister(RTC_MIN, minute);
	readRegister(RTC_HOUR, hour);
	readRegister(RTC_DAY, day);
	readRegister(RTC_MONTH, month);
	readRegister(RTC_YEAR, year);

	if(bcdMode) {
		BCDtoBinary(seconds);
		BCDtoBinary(minute);
		BCDtoBinary(hour);
		BCDtoBinary(day);
		BCDtoBinary(month);
		BCDtoBinary(year);
	}
	*year += 2000;
}

