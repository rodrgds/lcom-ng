/**
 * @file rtc_info
 * @brief Constants for programming the RTC
 */

#ifndef _LCOM_RTC_INFO_H_
#define _LCOM_RTC_INFO_H_

/** @defgroup RTC_info RTC_info
 * @{
 *
 * Constants for programming the RTC
 */

#define RTC_IRQ 8 /**< @brief RTC IRQ line */

#define RTC_ADDR_REG 0x70 /**< @brief Port to load the address of the rtc register */
#define RTC_DATA_REG 0x71 /**< @brief Port to transfer data to/from the rtc register */

#define RTC_SECOND_ADDR_LOC 0 /**< @brief Address location for the second */
#define RTC_MINUTE_ADDR_LOC 2 /**< @brief Address location for the minute */
#define RTC_HOUR_ADDR_LOC   4 /**< @brief Address location for the hour */
#define RTC_DAY_ADDR_LOC    7 /**< @brief Address location for the day */
#define RTC_MONTH_ADDR_LOC  8 /**< @brief Address location for the month */
#define RTC_YEAR_ADDR_LOC   9 /**< @brief Address location for the year */

#define RTC_UPDATE_REG_A    10 /**< @brief Adress location of register A */
#define RTC_UPDATE          BIT(7) /**< @brief Bit indicates rtc is updating */

#define RTC_BINARY_REG_B    11 /**< @brief Adress location of register B */
#define RTC_BINARY          BIT(2) /**< @brief Bit indicates rtc data format is binary */

/**@}*/

#endif
