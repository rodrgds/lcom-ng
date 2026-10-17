#ifndef _MACRO_RTC_H_
#define _MACRO_RTC_H_

/**
 * @ingroup Device_Drivers
 * @{
 * 
 * @defgroup RTC RTC
 * 
 * @brief Device driver module for the real-time clock (RTC).
 * 
 * This module provides the necessary functionality to interact with the real-time clock.
 * It includes functions for initializing the RTC and working with alarm interrupts.
 * The module also contains definitions for registers related to the RTC.
 * 
 * @{
*/

#define LIGHTMODE_HOURS 0x07  /**< @brief Hour of the day at which lightmode is the theme */
#define DARKMODE_HOURS 0x20  /**< @brief Hour of the day at which darkmode is the theme */

#define RTC_IRQ 8           /**< @brief RTC IRQ line */

#define RTC_ADDR_REG 0x70   /**< @brief Port used to load the address of the RTC register to be accessed */
#define RTC_DATA_REG 0x71   /**< @brief Port used to transfer the data to/from the RTC's register accessed */

#define RTC_SECONDS 0       /**< @brief RTC seconds register */
#define RTC_SECONDS_ALARM 1 /**< @brief RTC seconds alarm register */
#define RTC_MINUTES 2       /**< @brief RTC minutes register */
#define RTC_MINUTES_ALARM 3 /**< @brief RTC minutes alarm register */
#define RTC_HOURS 4         /**< @brief RTC hours register */
#define RTC_HOURS_ALARM 5   /**< @brief RTC hours alarm register */
#define RTC_WEEK_DAY 6      /**< @brief RTC week DAY register */
#define RTC_DAY 7           /**< @brief RTC day register */
#define RTC_MONTH 8         /**< @brief RTC month register */
#define RTC_YEAR 9          /**< @brief RTC year register */

#define RTC_REG_A 0x0A      /**< @brief RTC register A */
#define RTC_UIP BIT(7)      /**< @brief Update in progress */

#define RTC_REG_B 0x0B      /**< @brief RTC register B */
#define RTC_SET BIT(7)      /**< @brief Inhibit updates of time/date registers */
#define RTC_PIE BIT(6)      /**< @brief Enable periodic interrupts */
#define RTC_AIE BIT(5)      /**< @brief Enable alarm interrupts */
#define RTC_UIE BIT(4)      /**< @brief Enable update interrupts */
#define RTC_BIN BIT(2)      /**< @brief Enable binary mode */

#define RTC_REG_C 0x0C      /**< @brief RTC register C */
#define RTC_PF BIT(6)       /**< @brief Periodic interrupt pending */
#define RTC_AF BIT(5)       /**< @brief Alarm interrupt pending */
#define RTC_UE BIT(4)       /**< @brief Update interrupt pending */

#define RTC_REG_D 0x0D      /**< @brief RTC register D */

/** @} 
 * }@
*/

#endif
