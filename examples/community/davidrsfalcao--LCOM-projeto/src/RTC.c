#include "RTC.h"
#include <string.h>

int hook_id_rtc = 15;

int decimal (unsigned long bcd)
{
  return (((bcd & 0xF0) >> 4) * 10) + (bcd & 0x0F);
}


int RTC_subscribe_int() {
	int temp = hook_id_rtc;
	if(sys_irqsetpolicy(8, IRQ_REENABLE|IRQ_EXCLUSIVE,&temp)!=OK){
		return -1;
	}
	if(sys_irqenable(&temp)!=OK){
		return -2;
	}
	return BIT(hook_id_rtc);
}


int RTC_unsubscribe_int() {
	int hook_id_rtc_temp = hook_id_rtc;
	if(sys_irqrmpolicy(&hook_id_rtc_temp)!=OK){
		return 1;
	}
	if(sys_irqenable(&hook_id_rtc_temp)!=OK){
		return 1;
	}
	return 0;
}

void Set_RTC() {
	unsigned long regB = BIT(4) | BIT(1); //set 24h format
	sys_outb(RTC_ADDR_REG, REG_B);
	sys_outb(RTC_DATA_REG, regB);

}

void clean_buffer(){
	unsigned long regC;
	sys_outb(RTC_ADDR_REG, REG_C);
	sys_inb(RTC_DATA_REG, &regC);
}


int segundos(){
	unsigned long v = 0;
	sys_outb(RTC_ADDR_REG, SECONDS);
	sys_inb(RTC_DATA_REG, &v);
	return decimal(v);
}

int dia() {
	unsigned long v = 0;
	sys_outb(RTC_ADDR_REG, DAY);
	sys_inb(RTC_DATA_REG, &v);
	return decimal(v);
}

int mes() {
	unsigned long v = 0;
	sys_outb(RTC_ADDR_REG, MONTH);
	sys_inb(RTC_DATA_REG, &v);
	return decimal(v);
}


int ano() {
	unsigned long v = 0;
	sys_outb(RTC_ADDR_REG, YEAR);
	sys_inb(RTC_DATA_REG, &v);
	return decimal(v);
}


int horas() {
	unsigned long v = 0;
	sys_outb(RTC_ADDR_REG, HOUR);
	sys_inb(RTC_DATA_REG, &v);
	return decimal(v);
}


int minutos() {
	unsigned long v = 0;
	sys_outb(RTC_ADDR_REG, MINUTES);
	sys_inb(RTC_DATA_REG, &v);
	return decimal(v);
}
