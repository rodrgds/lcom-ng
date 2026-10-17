#include "i8042.h"

#define RTC_IRQL			8
#define RTC_ADDR_REG		0x70
#define RTC_DATA_REG		0x71

#define RTC_SEC				0
#define RTC_SECA			1
#define RTC_MIN				2
#define RTC_MINA			3
#define RTC_H				4
#define RTC_HA				5
#define RTC_DW				6
#define RTC_DM				7
#define RTC_MONTH			8
#define RTC_YEAR			9
#define RTC_REG_A			10
#define RTC_REG_B			11
#define RTC_REG_C			12
#define RTC_REG_D			13

int rtc_read(uint32_t addr, uint32_t* config);
int rtc_write(uint32_t addr, uint32_t config);
int rtc_get_date(uint32_t config[10]);
int rtc_clear_reg_C();
char* get_day_week(uint32_t dayWeek);
char* get_month(uint32_t month);
char* get_year(uint32_t year);
char* get_formated_date();
