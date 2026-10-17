#include "rtc.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <minix/sysutil.h>
#include "macros.h"
#include "utilities.h"

int hook_id_rtc = 0x03;

int rtc_subscribe() {
	int ret = hook_id_rtc;

	if (sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_rtc) == OK)
		if (sys_irqenable(&hook_id_rtc) == OK)
			return ret;

	return -1;
}

int rtc_unsubscribe() {
	if (sys_irqdisable(&hook_id_rtc) == OK)
		if (sys_irqrmpolicy(&hook_id_rtc) == OK)
			return 0;

	return -1;
}

unsigned long read_reg(unsigned long info) {
	unsigned long res = 0;

	sys_outb(RTC_ADDR_REG, info);
	sys_inb(RTC_DATA_REG, &res);

	return res;
}

unsigned long getDay() {
	unsigned long day = read_reg(PC_DAY);

	day = convertBCDtoBinary(day);

	return day;
}

unsigned long getMonth() {
	unsigned long month = read_reg(PC_MONTH);

	month = convertBCDtoBinary(month);

	return month;
}

unsigned long getYear() {
	unsigned long year = read_reg(PC_YEAR);

	year = convertBCDtoBinary(year);

	return year;
}

unsigned long getSec() {
	unsigned long sec = read_reg(PC_SEC);

	sec = convertBCDtoBinary(sec);

	return sec;
}

unsigned long getMin() {
	unsigned long min = read_reg(PC_MIN);

	min = convertBCDtoBinary(min);

	return min;
}

unsigned long getHour() {
	unsigned long hour = read_reg(PC_HOUR);

	hour = convertBCDtoBinary(hour);

	return hour;
}
