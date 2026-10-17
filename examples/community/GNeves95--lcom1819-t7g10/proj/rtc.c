#include <lcom/lcf.h>
#include <stdio.h>
#include "rtc.h"

int hook_rtc_id = 8;

uint8_t rtc_bcd_to_bin(uint8_t bcd_num){
	return ((bcd_num >> 4)*10) + (bcd_num & 0x0F);
}

uint8_t rtc_bin_to_bcd(uint8_t bin_num){
	return ((bin_num / 10)<<4) + (bin_num % 10);
}

int rtc_bcd(){
	uint32_t regB = 0;
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_inb(RTC_DATA_REG, &regB);
	
	return ((regB & RTC_B_DM) == 0);
}

void enable_rtc_int(){
	uint32_t regB = 0;
	
	rtc_flags_status.alarm_flag = false;
	rtc_flags_status.update_flag = false;
	rtc_flags_status.periodic_flag = false;
	
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_inb(RTC_DATA_REG, &regB);

	regB = regB | RTC_B_PIE | RTC_B_AIE | RTC_B_UIE;
	
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_outb(RTC_DATA_REG, regB);
}

void disable_per_rtc_int(){
	uint32_t regB = 0;
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_inb(RTC_DATA_REG, &regB);

	if(regB & RTC_B_PIE)
		regB = regB - RTC_B_PIE;
	if(regB & RTC_B_UIE)
		regB = regB - RTC_B_UIE;
	
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_outb(RTC_DATA_REG, regB);
}

void disable_rtc_int(){
	uint32_t regB = 0;
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_inb(RTC_DATA_REG, &regB);

	if(regB & RTC_B_PIE)
		regB = regB - RTC_B_PIE;
	if(regB & RTC_B_AIE)
		regB = regB - RTC_B_AIE;
	if(regB & RTC_B_UIE)
		regB = regB - RTC_B_UIE;
	
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_outb(RTC_DATA_REG, regB);
}

void wait_valid_rtc(void) {
	uint32_t regA = 0;
	do {
		disable_rtc_int();
		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		sys_inb(RTC_DATA_REG, &regA);
		enable_rtc_int();
	} while ( regA & RTC_A_UIP);
}

int rtc_subscribe_int(uint8_t *bit_no){
    *bit_no = hook_rtc_id;
	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_rtc_id) != 0)
        return 1;
    return 0;
}

int rtc_unsubscribe_int(void)
{
    if (sys_irqrmpolicy(&hook_rtc_id) != 0)
        return 1;
    return 0;
}

timedate get_date(){
	timedate ret;
	
	uint32_t regB = 0;
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_inb(RTC_DATA_REG, &regB);
	
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_outb(RTC_DATA_REG, regB | RTC_B_SET);
		
	for(int i=0; i < 10; i++){
		uint32_t data;
		sys_outb(RTC_ADDR_REG, i);
		sys_inb(RTC_DATA_REG, &data);
		switch(i){
			case 0:
				ret.seconds = rtc_bcd_to_bin(data);
				break;
			case 2:
				ret.minutes = rtc_bcd_to_bin(data);
				break;
			case 4:
				ret.hours = rtc_bcd_to_bin(data);
				break;
			case 6:
				ret.dweek = dow[rtc_bcd_to_bin(data)];
				break;
			case 7:
				ret.day = rtc_bcd_to_bin(data);
				break;
			case 8:
				ret.month = rtc_bcd_to_bin(data);
				ret.mon = Month[rtc_bcd_to_bin(data)];
				break;
			case 9:
				ret.year = 2000+rtc_bcd_to_bin(data);
				break;
		}
	}
	
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_inb(RTC_DATA_REG, &regB);
	
	if(regB & RTC_B_SET)
		regB = regB - RTC_B_SET;
	
	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_outb(RTC_DATA_REG, regB);
	
	return ret;
}

void handle_update_int(){
	rtc_flags_status.update_flag = true;
}

void handle_alarm_int(){
	rtc_flags_status.alarm_flag = true;
}

void handle_periodic_int(){
	rtc_flags_status.periodic_flag = true;
}

void rtc_ih(void) {
	uint32_t regC;
	
	sys_outb(RTC_ADDR_REG, RTC_REG_C);
	sys_inb(RTC_DATA_REG, &regC);
	
	if( regC & RTC_C_UF ){
		handle_update_int();
	}
	if( regC & RTC_C_AF ){
		handle_alarm_int();
		//return 1;
	}
	if( regC & RTC_C_PF ){
		handle_periodic_int();
	}
	//return 0;
}

void set_alarm(uint16_t time){
	timedate aux = get_date();
	uint32_t hour = 0, minutes = 0, seconds = 0;
	
	uint32_t res_time = aux.hours*60*60 + aux.minutes*60 + aux.seconds;
	
	res_time += time;
	
	hour += res_time / (60*60)%24;
	minutes += ((res_time)/ (60))%60;
	seconds += ((res_time)%60);
	//printf("%02d:%02d:%02d\n", hour,minutes,seconds);
	
	sys_outb(RTC_ADDR_REG, RTC_SEC_ALARM);
	sys_outb(RTC_DATA_REG, rtc_bin_to_bcd(seconds));
	
	sys_outb(RTC_ADDR_REG, RTC_MIN_ALARM);
	sys_outb(RTC_DATA_REG, rtc_bin_to_bcd(minutes));
	
	sys_outb(RTC_ADDR_REG, RTC_HOUR_ALARM);
	sys_outb(RTC_DATA_REG, rtc_bin_to_bcd(hour));
} 
