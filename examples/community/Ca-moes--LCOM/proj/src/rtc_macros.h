#ifndef _LCOM_RTC_MACROS_H
#define _LCOM_RTC_MACROS_H

#include <lcom/lcf.h>

/** @defgroup rtc_macros rtc_macros
 * @{
 *
 * Constants for programming the RTC
 */

typedef struct realTime{
  
	uint32_t minutes;
	uint32_t hour;
	uint32_t day; 
	uint32_t month;
	uint32_t year;

} realTime;

#define RTC_IRQ         8 /**< @brief RTC IRQ line */

/* Registers/Ports */
#define RTC_ADDR_REG    0x70  /**<loaded with the address of the RTC register to be accessed*/
#define RTC_DATA_REG    0x71  /**<used to transfer the data to/from the RTC’s register accessed*/

#define RTC_SEC         0x00       /**<  @brief Seconds*/
#define RTC_SEC_ALARM   0x01      /**<  @brief Seconds Alarm*/
#define RTC_MIN         0x02       /**<  @brief Minutes*/
#define RTC_MIN_ALARM   0x03       /**<  @brief Minutes Alarm*/
#define RTC_HOUR        0x04      /**<  @brief Hours*/
#define RTC_HOUR_ALARM  0x05       /**<  @brief Hours Alarm*/
#define RTC_WEEK_DAY    0x06       /**<  @brief Week Day*/
#define RTC_DAY         0x07      /**<  @brief Month Day*/
#define RTC_MONTH       0x08       /**<  @brief Month*/
#define RTC_YEAR        0x09       /**<  @brief Year*/

/*Internal registers - Control/Status Registers*/
#define RTC_REG_A       10      /**<  @brief rtc register A*/
#define RTC_REG_B       11      /**<  @briefrtc register B*/
#define RTC_REG_C       12      /**<  @briefrtc register C*/
#define RTC_REG_D       13      /**<  @brief rtc register D*/

#define RTC_UIP         BIT(7)  /**<  @brief 1 - don't access time date registers*/
#define RTC_RATE_SEL    0xF     /**<  @brief show rate selector*/
#define RTC_SET         BIT(7)  /**<  @brief 1 - inhibit updates of time/date registers.*/
#define RTC_UIE         BIT(4)  /**<  @brief enable update interrupts*/
#define RTC_AIE         BIT(5)  /**<  @brief enable alarm interrupts*/
#define RTC_DM          BIT(2)  /**<  @brief 1 - set time, alarm and date registers in binary*/

/*Flags*/
#define RTC_UF          BIT(4)  /**<  @brief Update interrupt pending*/
#define RTC_AF          BIT(5)  /**<  @brief Alarm interrupt pending*/
#define RTC_PF          BIT(6)  /**<  @brief Periodic interrupt pending*/

#endif // _LCOM_RTC_MACROS_H
