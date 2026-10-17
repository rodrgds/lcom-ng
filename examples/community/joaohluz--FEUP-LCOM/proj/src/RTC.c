#include <lcom/lcf.h>
#include <machine/int86.h> // /usr/src/include/arch/i386
#include "RTC.h"

void wait_valid_rtc() 
{
	uint32_t regA = 0;
	do 
	{
		
		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		sys_inb(RTC_DATA_REG, &regA);
		
	} while ( regA & RTC_UIP);
}


uint32_t read_reg(int reg)
{
	uint32_t value;

	//wait_valid_rtc();

	if(sys_outb(RTC_ADDR_REG, reg) != OK)
	{
		printf("Error  in fucntion sys_outb(RTC_ADDR,reg)\n");
		return 1;
	}
	if(sys_inb(RTC_DATA_REG,&value) != OK)
	{
		printf("Error  in fucntion sys_inb(RTC_DATA,value)\n");
		return 1;
	}

	if((RTC_REG_B & BCD_MODE) == 0)
		value = ((value & 0xF0) >> 4) *10 + (value & 0x0F);
	
	return value;
}


uint32_t get_Seconds()
{
	return read_reg(RTC_SECONDS);
}

uint32_t get_Minute()
{
	return read_reg(RTC_MINUTES);
}

uint32_t get_Hour()
{
	return read_reg(RTC_HOURS);
}

uint32_t get_Day()
{
	return read_reg(RTC_DAYS);
}


uint32_t get_Month()
{
	return read_reg(RTC_MONTH);
}

uint32_t get_Year()
{
	return read_reg(RTC_YEAR);
}


