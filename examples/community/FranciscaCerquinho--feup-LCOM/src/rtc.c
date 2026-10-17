#include "rtc.h"


char readDate(unsigned char address, long *result){
	char aux;

	sys_outb(RTC_ADDR_REG, address);
	sys_inb(RTC_DATA_REG, result);

	aux = (char) *result;
	return aux;

}


int WriteScore(int score) {
	long hour, minutes, seconds, day, month, year;

	Date date;

	date.day=readDate(DAY_REG_RTC, &day);
	date.month=readDate(MONTH_REG_RTC, &month);
	date.year=readDate(YEAR_REG_RTC, &year);

	date.hour=readDate(HOUR_REG_RTC, &hour);
	date.minutes = readDate(MINUTES_REG_RTC, &minutes);
	date.seconds= readDate(SECONDS_REG_RTC, &seconds);


	if(date.day<10)
		printf(" 0%x /",date.day);
	else printf(" %x /",date.day);

	if(date.month<10)
		printf(" 0%x /",date.month);
	else printf(" %x /",date.month);

	if(date.year<10)
		printf(" 200%x ",date.year);
	else printf(" 20%x ",date.year);

	if(date.hour<10)
		printf("0%x:",date.hour);
	else printf("%x:",date.hour);

	if(date.minutes<10)
		printf("0%x:",date.minutes);
	else printf("%x:",date.minutes);

	if(date.seconds<10)
		printf("0%x",date.seconds);
	else printf("%x",date.seconds);

	printf("       Score %d", score);
	return 0;
}

