#include "rtc.h"
#include <string.h>

int hookidrtc = 15;



int BCDToDecimal (unsigned long bcdByte)
{
  return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}


int rtc_subscribe_int() {
	int temp = hookidrtc; //hookid temporario para não alterar o valor original
	if(sys_irqsetpolicy(8, IRQ_REENABLE|IRQ_EXCLUSIVE,&temp)!=OK){
		return -1;
	}
	if(sys_irqenable(&temp)!=OK){
		return -2;
	}
	return BIT(hookidrtc); //return dos bits da interrupt mask
}


int rtc_unsubscribe_int() {
	int hookidrtctemp = hookidrtc;
	if(sys_irqrmpolicy(&hookidrtctemp)!=OK){
		return 1;
	}
	if(sys_irqenable(&hookidrtctemp)!=OK){
		return 1;
	}
	return 0;
}

void set_rtc() {
	//enable periodic interrupts
	unsigned long regB = BIT(4) | BIT(1); //set 24h format
	sys_outb(RTC_ADDR_REG, REG_B);
	sys_outb(RTC_DATA_REG, regB);

}

void clean_buffer(){
	unsigned long regC;
	sys_outb(RTC_ADDR_REG, REG_C);
	sys_inb(RTC_DATA_REG, &regC);
}


int get_seconds(){
	unsigned long value = 0;
	sys_outb(RTC_ADDR_REG, SECONDS);
	sys_inb(RTC_DATA_REG, &value);
	return BCDToDecimal(value);
}

int get_day() {
	unsigned long value = 0;
	sys_outb(RTC_ADDR_REG, DAY);
	sys_inb(RTC_DATA_REG, &value);
	return BCDToDecimal(value);
}

int get_month() {
	unsigned long value = 0;
	sys_outb(RTC_ADDR_REG, MONTH);
	sys_inb(RTC_DATA_REG, &value);
	return BCDToDecimal(value);
}


int get_year() {
	unsigned long value = 0;
	sys_outb(RTC_ADDR_REG, YEAR);
	sys_inb(RTC_DATA_REG, &value);
	return BCDToDecimal(value);
}


int get_hour() {
	unsigned long value = 0;
	sys_outb(RTC_ADDR_REG, HOUR);
	sys_inb(RTC_DATA_REG, &value);
	return BCDToDecimal(value);
}


int get_minutes() {
	unsigned long value = 0;
	sys_outb(RTC_ADDR_REG, MINUTES);
	sys_inb(RTC_DATA_REG, &value);
	return BCDToDecimal(value);
}
