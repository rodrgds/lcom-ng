#include "rtc.h"
#include "Utilities.h"
#include <minix/bitmap.h>
#include <minix/drivers.h>

int rtc_test_conf(void) {
	/* To be completed */
}

int rtc_read_reg(unsigned long reg, unsigned long* value)
{
	if (sys_outb(RTC_ADDR_REG,reg)!=OK) {
		DEBUG_STR("rtc_read_reg:failed to write") ;
		return -1;}

	if (sys_inb(RTC_DATA_REG,value)!=OK) {
		DEBUG_STR("rtc_read_reg:failed to read") ;
		return -1;}

	if (value==NULL) return -1;

	return 0;
}


unsigned int rtc_generate_seed(void) {

	unsigned long control=0;
	const unsigned long maxControl=9999999;
	unsigned long regA;

do{
	sys_outb(RTC_ADDR_REG,RTC_REG_A);
	if (sys_inb(RTC_DATA_REG,&regA)!=OK)
	{
		DEBUG_STR("rtc_test_date:TIMEOUT") ; return -1;
	}

}while(bit_isset(regA,RTC_UIP_BIT) && control<maxControl);
if (control>=maxControl) { DEBUG_STR("rtc_test_date:TIMEOUT") ; return -1;}

unsigned long seconds, minutes, hours, dayOfMonth , month, year;

rtc_read_reg(RTC_SECONDS,&seconds);
DEBUG_HEXADEC(seconds);
rtc_read_reg(RTC_MINUTES,&minutes);
DEBUG_HEXADEC(minutes);
rtc_read_reg(RTC_HOURS,&hours);
DEBUG_HEXADEC(hours);
rtc_read_reg(RTC_DAY_OF_MONTH,&dayOfMonth);
DEBUG_HEXADEC(dayOfMonth);
rtc_read_reg(RTC_MONTH,&month);
DEBUG_HEXADEC(month);
rtc_read_reg(RTC_YEAR,&year);
DEBUG_HEXADEC(year);

//using more bits  srand uses unsigned int so cant use all bits
unsigned int seed=
		(unsigned int) (seconds
		+ (minutes<<1)
		+ (hours<<2)
		+ (dayOfMonth<<4)
		+ (month<<5)
		+ (year<<7)
		);

if(   ((char)(seed&0x00FF))  == '.' ) seed++;

return seed;

/*return (unsigned long long) (seconds
		+ (minutes<<8)
		+ (hours<<16)
		+ (dayOfMonth<<24)
		+ (month<<32)
		+ (year<<40)
		);//6*8bits*/
}

