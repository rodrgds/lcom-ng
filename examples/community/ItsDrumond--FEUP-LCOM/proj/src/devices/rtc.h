/**
 * @file rtc.h
 * 
 */

#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "rtc_Macros.h"

// RTC Data

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} Rtc;

// Methods

/**
 * @brief This method outputs a command to the RTC
 * 
 * @param command Command to send to the RTC
 * @param output Pointer to the variable where the output will be stored
 * @return Returns an int value of zero upon success and one otherwise
 */
int (rtc_output)(uint8_t command, uint8_t *output);

/**
 * @brief This method checks if the RTC is available to be read
 * 
 * @return Returns an int value of zero if the RTC is available to be read and one otherwise
 */
int (rtc_available)();

/**
 * @brief This method checks if the RTC is in binary mode
 * 
 * @return Returns an int value of one if there is an error otherwise returns zero if is in BCD mode or a non-zero value if is in binary mode
 */
int (rtc_in_binary_mode)();

/**
 * @brief This method converts a BCD value to binary
 * 
 * @param bcd BCD value to convert
 * @return Returns the binary equivalent of the BCD value
 */
uint8_t  (rtc_convert_to_binary)(uint8_t bcd);

/**
 * @brief This method updates the RTC values
 * 
 * @return Returns an int value of zero upon success and one otherwise
 */
int (rtc_update)();


#endif
