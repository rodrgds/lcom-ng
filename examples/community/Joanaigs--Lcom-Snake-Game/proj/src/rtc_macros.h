#ifndef _LCOM_RTC_MACROS_H_
#define _LCOM_RTC_MACROS_H_

#define IRQ_RTC 8
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71 

#define SECONDS 0
#define SECONDS_ALARM 1
#define MINUTES 2
#define MINUTES_ALARM 3
#define HOURS_MODE 4
#define HOURS_ALARM 5
#define DAY_OF_WEEK 6
#define DAY 7
#define MONTH 8
#define YEAR 9

// Register A
#define RTC_A 10
#define UIP BIT(7)
#define DV2 BIT(6)
#define DV1 BIT(5)
#define DV0 BIT(4)
#define RS3 BIT(3)
#define RS2 BIT(2)
#define RS1 BIT(1)
#define RS0 BIT(0)

// Register B
#define RTC_B 11
#define SET BIT(7)
#define PIE BIT(6)
#define AIE BIT(5)
#define UIE BIT(4)
#define SQWE BIT(3)
#define DM BIT(2)
#define HOUR BIT(1)
#define DSE BIT(0)

// Register C
#define RTC_C 12
#define IRQF    BIT(7)
#define PF      BIT(6)
#define AF      BIT(5)
#define UF      BIT(4)  

// Register D
#define RTC_D 13
#define VRT     BIT(7) 



#endif /*_LCOM_RTC_MACROS_H_ */

