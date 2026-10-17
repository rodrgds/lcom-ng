#ifndef _RTC_H_
#define _RTC_H_

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include "macros/iRTC.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


/**
 * @brief checks if rtc is updating or not
 * @return return 0 if rtc is not updating and non-zero otherwise
 */
int isRTCUpdating();
/**
 * @brief Checks if the data read from RTC is in BCD
 * @return Return 0 if it's not in bcd form and non-zero otherwise
 */
int isBCD();
/**
 * @brief transforms the data that is in bcd form to binary one
 * @param bcd bcd a pointer to the byte in BCD form
 * @return the binary form of the given BCD byte
 */
unsigned long BCDtoBin(unsigned long* bcd);
/**
 * @brief reads the current date from RTC
 * @param day a pointer to a byte to store the day
 * @param month a pointer to a byte to store the month
 * @param year a pointer to a byte to store the year
 */
void getDate(unsigned long *day, unsigned long *month, unsigned long *year);
/**
 * @brief reads current hour from RTC
 * @param hour a pointer to a byte to store the hour
 * @param minutes a pointer to a byte to store the minutes
 * @param seconds a pointer to a byte to store the seconds
 */
void getHour(unsigned long *hour, unsigned long *minutes, unsigned long *seconds);
/**
 * @brief aux function for sys_inb in rtc
 * @param port register from where we're reading data
 * @param value a pointer to the variable to fill with data
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_sys_inb_RTC)(int port, unsigned long *value);

#endif
