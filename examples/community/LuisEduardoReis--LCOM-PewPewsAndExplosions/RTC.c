#include <minix/drivers.h>

#include "RTC.h"


int RTC_register_read(unsigned int reg, unsigned char* byte) {

	if (reg > 13) return 1; //Invalid register

	sys_outb(RTC_ADDR_REG, (unsigned long) reg);

	unsigned long in;
	sys_inb(RTC_DATA_REG,&in);

	*byte = (unsigned char) in;

	return 0;
}

int RTC_register_write(unsigned int reg, unsigned char byte) {

	if (reg > 13) return 1; //Invalid register

	sys_outb(RTC_ADDR_REG, (unsigned long) reg);

	sys_outb(RTC_DATA_REG, byte);

	return 0;
}


int enable_UpdateInt(void) {
	char word;
	RTC_register_read(RTC_REG_B, &word);
	word |= UIE_BIT;
	RTC_register_write(RTC_REG_B, word);

	return 0;
};
int disable_UpdateInt(void) {
	char word;
	RTC_register_read(RTC_REG_B, &word);
	word &= ~UIE_BIT;
	RTC_register_write(RTC_REG_B, word);

	return 0;
};


int enable_RTC_IRQ(void) {
	char word;
	RTC_register_read(RTC_REG_C, &word);
	word |= IRQF_BIT;
	RTC_register_write(RTC_REG_C, word);

	return 0;
}
int disable_RTC_IRQ(void) {
	char word;
	RTC_register_read(RTC_REG_C, &word);
	word &= ~(IRQF_BIT);
	RTC_register_write(RTC_REG_C, word);

	return 0;
}


int RTC_subscribe_int() {

	RTC_HOOK_ID = 5;

	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &RTC_HOOK_ID) != OK
			|| sys_irqenable(&RTC_HOOK_ID) != OK)
		return -1;

	return (1<<5); //Returns bit order in interrupt mask
}

int RTC_unsubscribe_int() {
	if (sys_irqrmpolicy(&RTC_HOOK_ID) == OK)
		return 0;

	return 1;
}

int handle_RTC_int2() {
	unsigned char word;
	RTC_register_read(RTC_REG_C, &word);
	printf("Interrupt. C: 0x%x\n",word);

	if (word & AF_BIT) return 1;
	else return 0;
};



int RTC_read_Date(Date* d) {
	unsigned char in;

	Date date;

	RTC_register_read(RTC_SECONDS, &in);
	date.seconds = in;

	RTC_register_read(RTC_MINUTES, &in);
	date.minutes = in;

	RTC_register_read(RTC_HOURS, &in);
	date.hours = in;

	RTC_register_read(RTC_WEEKDAY, &in);
	date.weekday = in;

	RTC_register_read(RTC_DAY, &in);
	date.day = in;

	RTC_register_read(RTC_MONTH, &in);
	date.month = in;

	RTC_register_read(RTC_YEAR, &in);
	date.year = in;

	*d = date;

	return 0;
};



int printDate(Date date) {
	//"00:00:00 - YYYY/MM/DD- Week Day"
	char weekday[10];

	switch(date.weekday) {
	case 4: strcpy(weekday,"Domingo"); break;
	case 5: strcpy(weekday,"Segunda"); break;
	case 6: strcpy(weekday,"Ter�a"); break;
	case 7: strcpy(weekday,"Quarta"); break;
	case 1: strcpy(weekday,"Quinta"); break;
	case 2: strcpy(weekday,"Sexta"); break;
	case 3: strcpy(weekday,"Sabado"); break;
	}


	printf("%x:%x:%x - %x/%x/%x - %s",
			date.hours,date.minutes,date.seconds,
			date.year,date.month,date.day,
			weekday
			);

	return 0;
};

unsigned char bcd_to_char(unsigned char bcd) {
	return ((bcd & 0xF0) >> 4)*10 + (bcd & 0x0F);
}
unsigned char char_to_bcd(unsigned char byte) {
	return (byte % 10) + ((byte / 10) << 4);
}
