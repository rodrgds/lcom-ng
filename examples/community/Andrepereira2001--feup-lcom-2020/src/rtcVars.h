#ifndef _LCOM_RTCVARS_H
#define _LCOM_RTCVARS_H

#include <lcom/lcf.h>

/** @defgroup rtcVars
 * @{
 *
 * Constants for the . 
 */

#define SECONDS          0x00 /**< @brief rtc seconds register position */ 
#define MINUTES          0x02 /**< @brief rtc minutes register position */ 
#define HOURS            0x04 /**< @brief rtc hours register position */ 
#define MONTH_DAY        0x07 /**< @brief rtc month_day register position */ 
#define MONTH            0x08 /**< @brief rtc month register position */ 
#define YEAR             0x09 /**< @brief rtc year register position */ 
#define RTC_REG_A        0x0A /**< @brief rtc registerA register position */ 
#define RTC_REG_B        0x0B /**< @brief rtc registerB register position */ 
#define RTC_REG_C        0x0C /**< @brief rtc registerC register position */ 

#define RTC_ADDR_REG     0x70 /**< @brief rtc address register adrress */ 
#define RTC_DATA_REG     0x71 /**< @brief rtc data register adrress */ 

#define RTC_IRQ          8  /**< @brief rtc irq line */ 

#define RTC_P_INT_EN_B   BIT(6) /**< @brief periodic interrupt enable  registerB*/ 
#define RTC_P_INT_PEND_C BIT(6) /**< @brief periodic interrupt pending registerC  */ 
#define RTC_UPDATE_IN_P_A BIT(7) /**< @brief update in process */ 

#define INTERRUPT_INTERVAL (BIT(1) | BIT(2) | BIT(3)) /**< @brief Bits used to simbolize a interval between interrupts*/ 

#endif
