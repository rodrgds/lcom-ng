#ifndef __RTC_H_
#define __RTC_H_


#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71


#define RTC_SECONDS 		0
#define RTC_SECONDS_ALARM 	1
#define RTC_MINUTES 		2
#define RTC_MINUTES_ALARM 	3
#define RTC_HOURS 			4
#define RTC_HOURS_ALARM 	5
#define RTC_WEEKDAY 		6
#define RTC_DAY 			7
#define RTC_MONTH 			8
#define RTC_YEAR 			9

#define RTC_REG_A 10
#define RTC_REG_B 11
#define RTC_REG_C 12
#define RTC_REG_D 13


#define UIE_BIT 0x10
#define AIE_BIT 0x20

#define IRQF_BIT 0x80
#define AF_BIT 0x20
#define UPF_BIT 0x40

#define RTC_IRQ 8

static int RTC_HOOK_ID;


typedef struct {
	unsigned char seconds, minutes, hours, weekday, day, month, year;
} Date;



int RTC_register_read(unsigned int reg, unsigned char* byte);
int RTC_register_write(unsigned int reg, unsigned char byte);

int RTC_read_Date(Date* date);

int printDate(Date date);

int enable_UpdateInt(void);
int disable_UpdateInt(void);

int enable_RTC_IRQ(void);
int disable_RTC_IRQ(void);

int RTC_subscribe_int();
int RTC_unsubscribe_int();


int handle_RTC_int2();

unsigned char bcd_to_char(unsigned char bcd);
unsigned char char_to_bcd(unsigned char byte);

#endif
