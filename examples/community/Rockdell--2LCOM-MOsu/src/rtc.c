#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/sysutil.h>
#include "rtc.h"

//RTC's hook id
int rtc_hook_id = RTC_IRQ;

//Current RTC's state
rtc_t now;

int rtc_subscribe_int() {

	int rtc_id = rtc_hook_id;
	int valid = sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id);

	if(valid != OK)
		return -1;
	else
		return rtc_id;

}

int rtc_unsubscribe_int() {

	int valid = sys_irqrmpolicy(&rtc_hook_id);

	if(valid != OK)
		return 1;
	else
		return 0;

}

int rtc_int_handler() {

	unsigned long cause = rtc_readRegister(RTC_REG_C);

	if(cause & RTC_UF)
		handle_update_int();

	return 0;
}

void enable_update_int() {

	unsigned long regB = rtc_readRegister(RTC_REG_B);

	regB |= RTC_UF;

	rtc_writeRegister(RTC_REG_B, regB);
}

unsigned long rtc_readRegister(unsigned long reg) {

	sys_outb(RTC_ADDR_REG, reg);

	unsigned long response;
	sys_inb(RTC_DATA_REG, &response);

	return response;
}

void rtc_writeRegister(unsigned long reg, unsigned long param) {

	sys_outb(RTC_ADDR_REG, reg);
	sys_outb(RTC_DATA_REG, param);

}

void handle_update_int() {

	now.y = convertBinary(rtc_readRegister(RTC_REG_YEAR));
	now.m = convertBinary(rtc_readRegister(RTC_REG_MONTH));
	now.d = convertBinary(rtc_readRegister(RTC_REG_DAY));

	now.h = convertBinary(rtc_readRegister(RTC_REG_HOURS));
	now.min = convertBinary(rtc_readRegister(RTC_REG_MINUTES));
	now.s = convertBinary(rtc_readRegister(RTC_REG_SECONDS));
}

rtc_t checkClock() {
	return now;
}

unsigned long convertBinary(unsigned long bcd) {
	return bcd - 6 * (bcd >> 4);
}
