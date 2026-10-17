#pragma once

#define RTC_IRQ         8
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_CONF BIT(4) | BIT(1) | BIT(2)
#define REG_A 10
#define REG_B 11
#define REG_C 12
#define RTC_REGB_DM BIT(2)

#define SECONDS 0
#define MINUTES 2
#define HOURS 4
#define DAY 7
#define MONTH 8
#define YEAR 9
