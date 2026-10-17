/** @file rtc.c
    @author Daniel Marques
    @author João Almeida
    Declaration of functions used to read information from the RTC
*/

#include "minix/syslib.h"
#include "minix/driver.h"
#include "rtc.h"

void RTC_Subscribe(int rtc_id) {
	sys_irqsetpolicy( RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_id);
	sys_irqenable(&rtc_id);
}

void RTC_Unsubscribe(int rtc_id) {
	sys_irqrmpolicy(&rtc_id);
	sys_irqdisable(&rtc_id);
}

void RTC_EnableInt() {
	asm("sti");
}

void RTC_DisableInt() {
	asm("cli");
}

void RTC_SetBinary() {
	RTC_DisableInt();

	unsigned long int data;

	sys_outb(RTC_ADDR_REG, REG_B);
	sys_inb(RTC_DATA_REG, &data);

	if (!(data & DM)) { //IF IT'S IN BCD CHANGE TO BIN
		sys_outb(RTC_ADDR_REG, REG_B);
		unsigned char cmd = (unsigned char) data | DM;
		sys_outb(RTC_DATA_REG, cmd);
	}

	RTC_EnableInt();
}

void RTC_GetTime(unsigned long int *hour, unsigned long int *minute,
		unsigned long int *second) {
	RTC_DisableInt();

	unsigned long int data;

	do {
		sys_outb(RTC_ADDR_REG, REG_A);
		sys_inb(RTC_DATA_REG, &data);
	} while (data & UIP);

	sys_outb(RTC_ADDR_REG, HOURS);
	sys_inb(RTC_DATA_REG, hour);

	do {
		sys_outb(RTC_ADDR_REG, REG_A);
		sys_inb(RTC_DATA_REG, &data);
	} while (data & UIP);

	sys_outb(RTC_ADDR_REG, MINUTES);
	sys_inb(RTC_DATA_REG, minute);

	do {
		sys_outb(RTC_ADDR_REG, REG_A);
		sys_inb(RTC_DATA_REG, &data);
	} while (data & UIP);

	sys_outb(RTC_ADDR_REG, SECONDS);
	sys_inb(RTC_DATA_REG, second);

	RTC_EnableInt();
}

void RTC_GetDate(unsigned long int *year, unsigned long int *month,
		unsigned long int *day) {
	RTC_DisableInt();

	unsigned long int data;

	sys_outb(RTC_ADDR_REG, DAY_OF_THE_MONTH);
	sys_inb(RTC_DATA_REG, day);

	sys_outb(RTC_ADDR_REG, MONTH);
	sys_inb(RTC_DATA_REG, month);

	sys_outb(RTC_ADDR_REG, YEAR);
	sys_inb(RTC_DATA_REG, year);

	RTC_DisableInt();
}
