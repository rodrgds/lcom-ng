/**
 * @file rtc.h
 * @author Bruno Pinto and Vitor Magalhaes
 * @brief Contains rtc functions
 */

#ifndef __RTC_H
#define __RTC_H

/** @defgroup rtc rtc
 * @{
 *
 * Functions related to RTC
 */

/**
 * @brief Subscribes and enables Real-Time Clock
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int rtc_subscribe();

/**
 * @brief Unsubscribes Real-Time Clock
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int rtc_unsubscribe();

/**
 * @brief Reads content on register
 *
 * @param info register to read
 *
 * @return Returns content on register
 */
unsigned long read_reg(unsigned long info);

/**
 * @brief Returns the day on the Real-Time Clock
 *
 */
unsigned long getDay();

/**
 * @brief Returns the month on the Real-Time Clock
 *
 */
unsigned long getMonth();

/**
 * @brief Returns the year on the Real-Time Clock
 *
 */
unsigned long getYear();

/**
 * @brief Returns the seconds on the Real-Time Clock
 *
 */
unsigned long getSec();

/**
 * @brief Returns the minutes on the Real-Time Clock
 *
 */
unsigned long getMin();

/**
 * @brief Returns the hour on the Real-Time Clock
 *
 */
unsigned long getHour();

/** @} end of rtc */

#endif


