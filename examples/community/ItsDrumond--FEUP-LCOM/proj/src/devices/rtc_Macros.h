#ifndef _LCOM_RTC_MACROS_H_
#define _LCOM_RTC_MACROS_H_

#define RTC_INPUT_PORT 0x70
#define RTC_OUTPUT_PORT 0x71

#define RTC_AVAILABLE 10
#define RTC_COUNTING_MODE 11
#define RTC_CHECK_AVAILABLE_BIT BIT(7);
#define RTC_CHECK_MODE_BIT BIT(2);

#define RTC_INTERRUPT_RQ 8

#define SECONDS 0
#define MINUTES 2
#define HOURS 4   
#define DAY 7
#define MONTH 8
#define YEAR 9


#endif
