#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "rtc.h"

#define RTC_ADDR_REG 	0x70  /**< RTC Addres Register */
#define RTC_DATA_REG 	0x71  /**< RTC Data Register */
#define RTC_REG_A		0x0A  /**< RTC Register A*/
#define RTC_REG_B 		0x0B  /**< RTC Register B*/
#define RTC_REG_C 		0x0C  /**< RTC Register C*/
#define RTC_REG_D 		0x0D  /**< RTC Register D*/
#define RTC_IRQF 		8	  /**< RTC Interrupt Request */
#define RTC_HOOK_BIT 	2  	  /**< RTC Hook Bit*/

#define RTC_YEAR 		0x09  /**< RTC Year Register*/
#define RTC_MONTH 		0x08  /**< RTC Month Register*/
#define RTC_DAY 		0x07  /**< RTC Day Register*/
#define RTC_HOUR 		0x04  /**< RTC Hour Register*/
#define RTC_MIN 		0x02  /**< RTC Minute Register*/
#define RTC_SEC 		0x00  /**< RTC Second Register*/

#define MAX_ATTEMPTS 20       /**< RTC Max Attempts*/

int (isUpdating)() {
	uint32_t read;
	for(size_t i = 0; i < MAX_ATTEMPTS; i++) {
			sys_outb(0x70, RTC_REG_A);
			sys_inb(0x71, &read);
			if (read & BIT(7)) return 0;
	}
	return 1;
}

/*
int rtc_subscribe_int(){
	if(sys_irqsetpolicy(IRQ_RTC, IRQ_EXCLUSIVE | IRQ_REENABLE, &hookid_rtc) != OK)
		return 1;

	if(sys_irqenable(&hookid_rtc) != OK)
		return 1;

	return 0;
}

int rtc_unsubscribe_int(){
	if(sys_irqdisable(&hookid_rtc) != OK)
		return 1;

	if(sys_irqrmpolicy(&hookid_rtc) != OK)
		return 1;

	return 0;
}
*/

int isBCD() {
	uint32_t RegB = 0;

	sys_outb(RTC_ADDR_REG, RTC_REG_B);
	sys_inb(RTC_DATA_REG, &RegB);

	if (RegB & BIT(2))
		return 0;
	else
		return 1;
}

uint32_t convertDecimal(uint32_t* bcd) {
	uint32_t bi;

	bi = (((*bcd) & 0xF0) >> 4) * 10 + ((*bcd) & 0x0F);

	return bi;
}

int getDate(uint32_t *day, uint32_t *month, uint32_t *year, uint32_t *hour, uint32_t *min,uint32_t *sec) {
	//Read Day
	sys_outb(RTC_ADDR_REG, RTC_DAY);
	sys_inb(RTC_DATA_REG, day);

	//Read Month
	sys_outb(RTC_ADDR_REG, RTC_MONTH);
	sys_inb(RTC_DATA_REG, month);

	//Read Year
	sys_outb(RTC_ADDR_REG, RTC_YEAR);
	sys_inb(RTC_DATA_REG, year);

	//Read Hour
	sys_outb(RTC_ADDR_REG, RTC_HOUR);
	sys_inb(RTC_DATA_REG, hour);

	//Read Min
	sys_outb(RTC_ADDR_REG, RTC_MIN);
	sys_inb(RTC_DATA_REG, min);

	//Read Sec
	sys_outb(RTC_ADDR_REG, RTC_SEC);
	sys_inb(RTC_DATA_REG, sec);

	if (isBCD()) {
		(*day) = convertDecimal(day);
		(*month) = convertDecimal(month);
		(*year) = convertDecimal(year);
		(*hour) = convertDecimal(hour);
		(*min) = convertDecimal(min);
		(*sec) = convertDecimal(sec);
	}

	return 0;
}
