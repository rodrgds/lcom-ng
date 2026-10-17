#ifndef RTC_H
#define RTC_H

/** @defgroup RTC RTC
 * @{
 * Functions and constants to make RTC work
 */



#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "i8254.h"



#define RTC_ADDR_REG 0x70	/**< @brief Control Word for ADDR_REG */
#define RTC_DATA_REG 0x71	/**< @brief Control word for DATA_REG*/
#define SECONDS 0x00	/**< @brief Access to seconds */
#define MINUTES 0x02	/**< @brief Access to minutes*/
#define HOUR 0x04	/**< @brief Access to hour */
#define DAY 0x07	/**< @brief Access to days*/
#define MONTH 0x08	/**< @brief Access to months */
#define YEAR 0x09	/**< @brief Access to year */
#define REG_A 0x0A	/**< @brief Control Word to REG_A*/
#define REG_B 0x0B	/**< @brief Control Word to REG_B */
#define REG_C 0x0C	/**< @brief Control WOrd to REG_C*/

/**
 * @brief Convert BCD to integer
 * @param bcdByte BCD number
 * @return Return integer converted from BCD
 */
int BCDToDecimal (unsigned long bcdByte);

/**
 * @brief Subscribes rtc
 *
 * @return Return irq Bit mask and negative value on failure
 */
int rtc_subscribe_int();

/**
 * @brief Unsubscribes rtc interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int rtc_unsubscribe_int();

/**
 * @brief Set rtc, enables interrupts and 24 h format
 */
void set_rtc();

/**
 * @brief Cleans buffer from REG_C
 */
void clean_buffer();

/**
 * @brief Retuns seconds in real time
 *
 * @return Return seconds
 */
int get_seconds();

/**
 * @brief Retuns days in real time
 *
 * @return Return days
 */
int get_day();

/**
 * @brief Retuns months in real time
 *
 * @return Return month
 */
int get_month();

/**
 * @brief Retuns years in real time
 *
 * @return Return years
 */
int get_year();

/**
 * @brief Retuns hours in real time
 *
 * @return Return hours
 */

int get_hour();

/**
 * @brief Retuns minutes in real time
 *
 * @return Return minutes
 */
int get_minutes();


#endif
