#ifndef _LCOM_RTC_MACROS_H_
#define _LCOM_RTC_MACROS_H_

#define BIT(n) (1 << (n))

// RTC IRQ Line
#define RTC_IRQ 8

// RTC Registers
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define SECONDS 0
#define RTC_ALARM_SECONDS 1
#define MINUTES 2
#define RTC_ALARM_MINUTES 3
#define HOURS 4
#define RTC_ALARM_HOURS 5
#define WEEK_DAY 6
#define MONTH_DAY 7
#define MONTH 8
#define YEAR 9
#define RTC_REG_A 10
#define RTC_REG_B 11
#define RTC_REG_C 12   

#define RTC_DC 0xC0

#define RTC_UIP BIT(7)
#define RTC_RS0 BIT(0)
#define RTC_RS1 BIT(1)
#define RTC_RS2 BIT(2)
#define RTC_RS3 BIT(3)
#define RTC_RS_CLEAR 0xf0

#define RTC_PIE BIT(6)
#define RTC_AIE BIT(5)
#define RTC_UIE BIT(4)

#define RTC_PF  BIT(6)
#define RTC_AF  BIT(5)
#define RTC_UF BIT(4)

#endif /* _LCOM_RTC_MACROS_H */
