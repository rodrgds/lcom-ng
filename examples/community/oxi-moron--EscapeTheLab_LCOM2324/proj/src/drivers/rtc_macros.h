#ifndef G6_RTC_MACROS_H
#define G6_RTC_MACROS_H

#include <lcom/lcf.h>

// Address Macros
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

// IRQ Macros
#define RTC_IRQ_LINE 8
#define RTC_BIT_NO 8

// Register Macros
#define RTC_SECS          0
#define RTC_SECS_ALARM    1
#define RTC_MINS          2
#define RTC_MINS_ALARM    3
#define RTC_HOURS         4
#define RTC_HOURS_ALARM   5
#define RTC_WEEK_DAY      6
#define RTC_MONTH_DAY     7
#define RTC_MONTH         8
#define RTC_YEAR          9

#define RTC_REG_A         10
#define RTC_REG_B         11
#define RTC_REG_C         12
#define RTC_REG_D         13

// Register Specific Macros
#define RTC_UIP BIT(7)
#define RTC_24H BIT(1)
#define RTC_AIE BIT(5)
#define RTC_AID 0xDF
#define RTC_AF BIT(5)
#define RTC_DM BIT(2)
#define RTC_SET BIT(7)
#define RTC_RESET 0x7F

// Alarm Macros
#define RTC_DONT_CARE 0xC0

#endif //G6_RTC_MACROS_H
