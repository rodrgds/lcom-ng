#include "rtc.h"
#include <minix/drivers.h>

static void choosePort(char port) {
	sys_outb(RTC_ADDR_REG,port);
}

static void readPort(unsigned long *stat) {
	sys_inb(RTC_DATA_REF,stat);
}

static void writePort(unsigned long stat) {
	sys_outb(RTC_DATA_REF,stat);
}

static void readTime(unsigned short *time, char port, unsigned long *stat) {
	choosePort(port);
	readPort(stat);
	*time=*stat;
}

int rtc_subscribe_int() {

	int rtc_hook=4;
	unsigned long stat;

	choosePort(REG_B);
	readPort(&stat);
	choosePort(REG_B);
	//Enables update and alarm interrupts
	writePort((stat&0x7F)|BIT(4)|BIT(5));

	sys_irqsetpolicy(RTC_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&rtc_hook);
	sys_irqenable(&rtc_hook);

	return 1;
}

int rtc_unsubscribe_int() {

	sys_irqrmpolicy(&rtc_hook);
	sys_irqdisable(&rtc_hook);

	return 1;
}

static int bcd_to_decimal(char bcd) {
	return (bcd>>4)*10+(bcd&0xF);
}

static char decimal_to_bcd(int decimal) {
	return (((decimal/10)<<4)|(decimal%10));
}

int rtc_int_handler(unsigned short readtime, unsigned short* timesalarm, int DELTA) {

	unsigned long stat;
	int i;

	choosePort(REG_C);
	readPort(&stat);

	if( (stat&BIT(4)) && !readtime) {

		//Reads timesalarm
		readTime(&timesalarm[2],HOURS,&stat);
		readTime(&timesalarm[1],MINUTES,&stat);
		readTime(&timesalarm[0],SECONDS,&stat);

		printf("Depois de ler: %x %x %x\n", timesalarm[2], timesalarm[1],timesalarm[0]);

		for(i=0; i<3; i++)
			timesalarm[i]=bcd_to_decimal(timesalarm[i]);

		timesalarm[0]+=DELTA;

		printf("Set1: %d %d %d\n", timesalarm[2], timesalarm[1],timesalarm[0]);

		for(i=0; i<3; i++)
			timesalarm[i]=decimal_to_bcd(timesalarm[i]);

		//Sets the alarm
		choosePort(SECONDS_ALARM);
		writePort(timesalarm[0]);
		choosePort(MINUTES_ALARM);
		writePort(timesalarm[1]);
		choosePort(HOURS_ALARM);
		writePort(timesalarm[2]);

		return 1;
	}

	return !(stat&BIT(5));
}
