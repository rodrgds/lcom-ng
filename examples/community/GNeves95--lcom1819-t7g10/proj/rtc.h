#ifndef _RTC_H
#define _RTC_H

#define BIT(n)			(0x01<<(n))

#define RTC_ADDR_REG	0x70
#define	RTC_DATA_REG	0x71

#define RTC_SEC			0
#define	RTC_SEC_ALARM	1
#define RTC_MIN			2
#define RTC_MIN_ALARM	3
#define RTC_HOUR		4
#define RTC_HOUR_ALARM	5
#define	RTC_WEEK_DAY	6
#define	RTC_MONTH_DAY	7
#define RTC_MONTH		8
#define RTC_YEAR		9

#define RTC_REG_A		0x0A
#define RTC_REG_B		0x0B
#define RTC_REG_C		0x0C
#define RTC_REG_D		0x0D

#define RTC_A_UIP		BIT(7)
#define RTC_A_DV2		BIT(6)
#define RTC_A_DV1		BIT(5)
#define RTC_A_DV0		BIT(4)
#define RTC_A_RS3		BIT(3)
#define	RTC_A_RS2		BIT(2)
#define	RTC_A_RS1		BIT(1)
#define	RTC_A_RS0		BIT(0)

#define RTC_B_SET		BIT(7)
#define RTC_B_PIE		BIT(6)
#define RTC_B_AIE		BIT(5)
#define RTC_B_UIE		BIT(4)
#define RTC_B_SQWE		BIT(3)
#define	RTC_B_DM		BIT(2)
#define	RTC_B_24_12		BIT(1)
#define	RTC_B_DSE		BIT(0)

#define RTC_C_IRQF		BIT(7)
#define RTC_C_PF		BIT(6)
#define RTC_C_AF		BIT(5)
#define RTC_C_UF		BIT(4)

#define RTC_D_VIRT		BIT(7)

#define RTC_IRQ			8

static const char *dow[] = {"N/A","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
static const char *Month[] = {"N/A","January","February","March","April","May","June","July","August","September","October","November","December"};

typedef struct timedate{int year, month, day, hours, minutes, seconds;const char *dweek, *mon;} timedate;

typedef struct rtc_int_flags{bool alarm_flag, update_flag, periodic_flag;} rtc_int_flags;

void wait_valid_rtc(void);

void enable_rtc_int();

void disable_rtc_int();

int rtc_subscribe_int(uint8_t *bit_no);

int rtc_unsubscribe_int(void);

int rtc_bcd();

uint8_t rtc_bcd_to_bin(uint8_t bcd_num);

uint8_t rtc_bin_to_bcd(uint8_t bin_num);

timedate get_date();

void rtc_ih(void);

void handle_update_int();

void handle_alarm_int();

void handle_periodic_int();

void set_alarm(uint16_t time);

void disable_per_rtc_int();

rtc_int_flags rtc_flags_status;

#endif
