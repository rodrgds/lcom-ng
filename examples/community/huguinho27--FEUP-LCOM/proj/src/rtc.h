#ifndef RTC_H
#define RTC_H
#include <minix/drivers.h>
#include "utils.h"

/** @defgroup RTC RTC
 * @{
 * Functions for manipulating rtc commands
 */

#define RTC_ADDR_REG 	0x70
#define RTC_DATA_REG 	0x71
#define RTC_REG_A 		10
#define RTC_UIP			BIT(7)
#define RTC_SECONDS_ADDR	0
#define RTC_MINUTES_ADDR	2
#define RTC_HOURS_ADDR		4

/**
 * @brief Waits until we are able
 * to read information from RTC
 */
void wait_valid_rtc();

/**
 * @brief Reads the seconds from RTC peripheral
 * @return Seconds read from RTC
 */
unsigned long readSecondsFromRTC();

/**
 * @brief Converts Binary Coded Decimal to binary
 * @return The binary unsigned long result
 */
unsigned long convertBCD(unsigned long seconds);

#endif /**@}*/
