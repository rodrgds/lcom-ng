#include <lcom/lcf.h>
/** @defgroup rtc_macros rtc_macros.h
 * @{
 *
 * This file contains the macros used for programming the rtc.
 * 
 */

#define RTC_ADDR_REG        0x70
#define RTC_DATA_REG        0X71
// time data registers
#define SECONDS_ADDR        0X0
#define SECONDS_ALARM_ADDR  0X1
#define MINUTES_ADDR        0X2
#define MINUTES_ALARM_ADDR  0X3
#define HOURS_ADDR          0X4
#define HOURS_ALARM_ADDR    0X5
#define DAY_WEEK_ADDR       0X6
#define DAY_MONTH_ADDR      0X7
#define MONTH_ADDR          0X8
#define YEAR_ADDR           0X9
// control registers
#define REGISTER_A          0xa
#define REGISTER_B          0xb
#define REGISTER_C          0xc
#define REGISTER_D          0xd
#define RTC_IRQ_LINE        8 

// Register A bits:
#define REG_A_UIP           BIT(7)    // if bit is set to 1, update is in progress, do not access time registers


// Register B bits:
#define REG_B_SET           BIT(7)    // if bit is set to 1, updates of time/date registers are inhibited
#define REG_B_PIE           BIT(6)    // if bit is set to 1, enables periodic interrupts
#define REG_B_AIE           BIT(5)    // if bit is set to 1, enables alarm interrupts
#define REG_B_UIE           BIT(4)    // if bit is set to 1, enables update interrupts
#define REG_B_SQWE          BIT(3)    // if bit is set to 1, enables square-wave generation
#define REG_B_DM            BIT(2)    // if bit is set to 1, data is in binary    
#define REG_B_24_12         BIT(1)    // if bit is set to 1, hours range is 0-23, 0 for 0-12
#define REG_B_DSE           BIT(0)    // if bit is set to 1, Daylight savings time is enabled

// Register C bits
#define REG_C_IRQF          BIT(7)    // if bit is set to 1, IRQ line is active
#define REG_C_PF            BIT(6)    // if bit is set to 1, periodic interrupt is pending
#define REG_C_AF            BIT(5)    // if bit is set to 1, alarm interrupt is pending
#define REG_C_UF            BIT(4)    // if bit is set to 1, update interrupt is pending

// Register D bits
#define REG_D_VRT           BIT(7)    // Valid RAM/time – set to 0 when the internal lithium battery runs out  of energy – RTC readings are questionable
/**@}*/
