#include <lcom/lcf.h>
#include "i8254.h"
#include "rtc.h"

static int hook_id = RTC_IRQ;

int rtc_subscribe_int(uint8_t *bit_no){
    *bit_no = hook_id;
	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE , &hook_id)!=OK) return SET_POLICY_FAIL;
	return OK;
}

int rtc_unsubscribe_int(){
    if (sys_irqrmpolicy(&hook_id)!=OK)	return RM_POLICY_FAIL;
    return OK;
}

uint32_t rtc_data(uint8_t port){
	uint32_t data;

	if(sys_irqdisable(&hook_id) != OK) return NOT_OK;
	if(sys_outb(RTC_ADDR_REG, port) != OK) return NOT_OK;
	if(sys_inb(RTC_DATA_REG, &data) != OK) return NOT_OK;
	if(sys_irqenable(&hook_id) != OK) return NOT_OK;

	return data;
}

bool check_bcd(){
	uint32_t Reg_B = 0;
	sys_outb(RTC_ADDR_REG, REG_B);
	sys_inb(RTC_DATA_REG, &Reg_B);
	if (!(Reg_B & REGBM))
		return true;
	else
		return false;
}

void bcd_to_bin(uint32_t* val){	*val = (((*val) & 0xF0) >> 4) * 10 + ((*val) & 0x0F); }

int isUpdating(){
	uint32_t Reg_A = 0;
	sys_outb(RTC_ADDR_REG, REG_A);
	sys_inb(RTC_DATA_REG, &Reg_A);

	if ((Reg_A & REGAP) != 0)
		return NOT_OK;

	return OK;
}

void get_time(uint32_t *sec,uint32_t *min, uint32_t *hour){

    while(isUpdating() != OK) {}
    *sec  = rtc_data(SEC);
    *min  = rtc_data(MINT);
    *hour = rtc_data(HOUR);

    if(check_bcd()) {
        bcd_to_bin(sec);
        bcd_to_bin(min);
        bcd_to_bin(hour);
    }
}

void get_date(uint32_t *day,uint32_t *month, uint32_t *year){
    
    while(isUpdating() != OK) {}
    *day   = rtc_data(DAY);
    *month = rtc_data(MONTH);
    *year  = rtc_data(YEAR);

    if(check_bcd()) {
        bcd_to_bin(day);
        bcd_to_bin(month);
        bcd_to_bin(year);
  }
}
