#include <lcom/lcf.h>

#include "rtc.h"

int hookId = 8;

int rtc_subscribe_int(uint8_t *bit_no) {

	*bit_no = hookId;

	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookId) != OK) 
		return -1;
	

	if(rtc_enable_update_interrupts() != 0)
		return -1;
	
	return 0;
}

int rtc_unsubscribe_int() {

	if(sys_irqdisable(&hookId) != OK)
		return -1;
	

	if(sys_irqrmpolicy(&hookId) != OK)
		return -1;
	

	if(rtc_disable_update_interrupts() != 0) 
		return -1;
	
	return 0;
}

int rtc_ih() {
	uint32_t regC = 0;

	if(sys_outb(ADDRESS_REG, REG_C) != OK) 
		return -1;
	
	
	if (sys_inb(DATA_REG, &regC) != OK) 
		return -1;
	

	//Update interrupt
	if(regC & REG_C_UE) {
		if(update_RTC_date() != 0) 
			return -1;
	}
	

	return 0;
}

int rtc_enable_update_interrupts() {

 	uint32_t regB = 0;

  if(sys_outb(ADDRESS_REG, REG_B) != OK) {
		return -1;
  }

  if (sys_inb(DATA_REG, &regB) != OK) {
		return -1;
	}

	regB |= REG_B_UIE;
	regB &= ~REG_B_SET;

	if(sys_outb(ADDRESS_REG, REG_B) != OK) {
		return -1;
  }

	if(sys_outb(DATA_REG, regB) != OK) {
		return -1;
  }

  return 0;
}

int rtc_disable_update_interrupts() {

 	uint32_t regB = 0;

  if(sys_outb(ADDRESS_REG, REG_B) != OK) {
		return -1;
  }

  if (sys_inb(DATA_REG, &regB) != OK) {
		return -1;
	}

	regB &= ~BIT(4);

	if(sys_outb(ADDRESS_REG, REG_B) != OK) {
		return -1;
  }

	if(sys_outb(DATA_REG, regB) != OK) {
		return -1;
  }

  return 0;
}

uint32_t data_bcd_to_binary(uint32_t n){
	uint32_t bin = 0;
	uint32_t regB = 0;

	if(sys_outb(ADDRESS_REG, REG_B) != OK) {
		printf("");
	}
	if (sys_inb(DATA_REG, &regB) != OK) {
		printf("");
	}

	if (!(regB & REG_B_DM)){
		bin = ((n & 0xF0) >> 4) * 10 + (n & 0x0F);
		return bin;
	}
	return n;
}


Date_and_Time dt;

int update_RTC_date() {

	uint32_t temp_var = 0;

  if(sys_outb(ADDRESS_REG, REG_A_YEAR) != OK) {
		return -1;
  }
  if (sys_inb(DATA_REG, &temp_var) != OK) {
		return -1;
  }

	temp_var = data_bcd_to_binary(temp_var);
	dt.year = temp_var;


  if(sys_outb(ADDRESS_REG, REG_A_MONTH) != OK) {
		return -1;
  }
  if (sys_inb(DATA_REG, &temp_var) != OK) {
		return -1;
  }

	temp_var = data_bcd_to_binary(temp_var);
	dt.month = temp_var;

  if(sys_outb(ADDRESS_REG, REG_A_DAY) != OK) {
		return -1;
  }
  if (sys_inb(DATA_REG, &temp_var) != OK) {
		return -1;
  }

	temp_var = data_bcd_to_binary(temp_var);
	dt.day = temp_var;

  if(sys_outb(ADDRESS_REG, REG_A_HOUR) != OK) {
		return -1;
  }
  if (sys_inb(DATA_REG, &temp_var) != OK) {
		return -1;
  }

	temp_var = data_bcd_to_binary(temp_var);
	dt.hour = temp_var;

  if(sys_outb(ADDRESS_REG, REG_A_MINUTE) != OK) {
		return -1;
  }
  if (sys_inb(DATA_REG, &temp_var) != OK) {
		return -1;
  }

	temp_var = data_bcd_to_binary(temp_var);
	dt.minute = temp_var;

  if(sys_outb(ADDRESS_REG, REG_A_SECOND) != OK) {
		return -1;
  }
  if (sys_inb(DATA_REG, &temp_var) != OK) {
		return -1;
  }

	temp_var = data_bcd_to_binary(temp_var);
	dt.second = temp_var;

	return 0;
}

char * get_current_date_and_time(bool to_print){
	char * date_and_time = malloc(32 * sizeof *date_and_time);	
	snprintf(date_and_time, 32, "D20%02d/%02d/%02dT%02d:%02d:%02d\n", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
	
	if (to_print){
		int month = dt.month;
		switch (month)
	{
		case 1:
			snprintf(date_and_time, 32, "%02d JAN 20%02d %02d:%02d:%02d\n", dt.day,  dt.year, dt.hour, dt.minute, dt.second);
			break;
		case 2:
			snprintf(date_and_time, 32, "%02d FEB 20%02d %02d:%02d:%02d\n", dt.day,  dt.year, dt.hour, dt.minute, dt.second);
			break;
		case 3:
			snprintf(date_and_time, 32, "%02d MAR 20%02d %02d:%02d:%02d\n", dt.day,  dt.year, dt.hour, dt.minute, dt.second);
			break;
		case 4:
			snprintf(date_and_time, 32, "%02d APR 20%02d %02d:%02d:%02d\n", dt.day,  dt.year, dt.hour, dt.minute, dt.second);
			break;
		case 5:
			snprintf(date_and_time, 32, "%02d MAY 20%02d %02d:%02d:%02d\n", dt.day,  dt.year, dt.hour, dt.minute, dt.second);
			break;
		case 6:
			snprintf(date_and_time, 32, "%02d JUN 20%02d %02d:%02d:%02d\n", dt.day,  dt.year, dt.hour, dt.minute, dt.second);
			break;
		case 7:
			snprintf(date_and_time, 32, "%02d JUL 20%02d %02d:%02d:%02d\n", dt.day,  dt.year, dt.hour, dt.minute, dt.second);
			break;
		case 8:
			snprintf(date_and_time, 32, "%02d AUG 20%02d %02d:%02d:%02d\n", dt.day,  dt.year, dt.hour, dt.minute, dt.second);
			break;
		case 9:
			snprintf(date_and_time, 32, "%02d SEP 20%02d %02d:%02d:%02d\n", dt.day,  dt.year, dt.hour, dt.minute, dt.second);
			break;
		case 10:
			snprintf(date_and_time, 32, "%02d OCT 20%02d %02d:%02d:%02d\n", dt.day,  dt.year, dt.hour, dt.minute, dt.second);
			break;
		case 11:
			snprintf(date_and_time, 32, "%02d NOV 20%02d %02d:%02d:%02d\n", dt.day,  dt.year, dt.hour, dt.minute, dt.second);
			break;
		case 12:
			snprintf(date_and_time, 32, "%02d DEZ 20%02d %02d:%02d:%02d\n", dt.day,  dt.year, dt.hour, dt.minute, dt.second);
			break;
	
		default:
			break;
	}
	}
	return date_and_time;
}

