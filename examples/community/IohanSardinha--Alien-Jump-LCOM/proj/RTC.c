#include <lcom/lcf.h>
#include <stdio.h>

#include "RTC.h"

int rtc_read(uint32_t addr, uint32_t* config)
{
	int ret = sys_outb(RTC_ADDR_REG, addr); 
	ret |= sys_inb(RTC_DATA_REG, config);
	return ret;
}
int rtc_write(uint32_t addr, uint32_t config)
{
	int ret = sys_outb(RTC_ADDR_REG, addr); 
	ret |= sys_outb(RTC_DATA_REG, config);
	return ret;
}

int rtc_get_date(uint32_t config[10])
{
	tickdelay(micros_to_ticks(DELAY_US));

	for(int i = 0; i < 10; i++)
		if(rtc_read(i, &config[i]) != 0)
			return -1;

	config[RTC_MIN] += 2;
	config[RTC_MIN] %= 60;

	config[RTC_H] += 11;
	config[RTC_H] %= 24;

	config[RTC_SEC] %= 60;

	return 0;
}

char* get_day_week(uint32_t dayWeek)
{
	if(dayWeek == 1) return "Mon";
	if(dayWeek == 2) return "Tue";
	if(dayWeek == 3) return "Wed";
	if(dayWeek == 4) return "Thu";
	if(dayWeek == 5)return "Fri";
	if(dayWeek == 6)return "Sat";
	return "Sun";
}

char* get_month(uint32_t month)
{

	if(month == 1) return "Jan";
	if(month == 2) return "Feb";
	if(month == 3) return "Mar";
	if(month == 4) return "Apr";
	if(month == 5) return "May";
	if(month == 6) return "Jun";
	if(month == 7) return "Jul";
	if(month == 8) return "Aug";
	if(month == 9) return "Sep";
	if(month == 10) return "Oct";
	if(month == 11) return "Nov";
	if(month == 12) return "Dec";
	if(month == 0x10) return "Oct";
	if(month == 0x11) return "Nov";
	return "Dec";
}

char* get_year(uint32_t year)
{
	char* ret = (char*)malloc(5*sizeof(char));
	sprintf(ret,"20%x",year);
	return ret;
} 

char* get_formated_date()
{
	rtc_clear_reg_C();

    uint32_t rtc_time[10];
    rtc_get_date(rtc_time);

	char* ret = (char*)malloc(15*sizeof(char));
	sprintf(ret,"%s %d %s %s", get_day_week(rtc_time[RTC_DW]), rtc_time[RTC_DM], get_month(rtc_time[RTC_MONTH]), get_year(rtc_time[RTC_YEAR]));
	return ret;	
}

int rtc_clear_reg_C()
{
	return rtc_write(RTC_REG_C, 0);
}
