/**
 * @file rtc.h
 * 
 */

#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <lcom/lcf.h>

// MACROS //
#define RTC_REG_UPDATING 10
#define RTC_REG_COUNTING 11
#define RTC_REG_INPUT 0x70
#define RTC_REG_OUTPUT 0x71
#define RTC_BIN_MASK BIT(2)

// COMMANDS //
#define RTC_SECONDS 0
#define RTC_MINUTES 2
#define RTC_HOURS 4   
#define RTC_DAY 7
#define RTC_MONTH 8
#define RTC_YEAR 9 

/**
 * @brief through its variables, it reads the timer's output
 * 
 * @param cmd the command to output to RTC
 * @param output reference to the value the RTC outputs
 * @return int is 0 if the function went smoothly, will not be 0 if any errors occured
 */
int getRTCOutput(uint8_t cmd, uint8_t *output);

/**
 * @brief checks if the RTC is in binary mode
 * 
 * @return true if it is
 * @return false if it is not
 */
bool isBinary();

/**
 * @brief function that converts a RTC output not in binary to its binary counterpart
 * 
 * @param num the number to be converted
 * @return uint8_t the conversion's result
 */
uint8_t toBinary(uint8_t num);

/**
 * @brief function that requests the RTC for any information regarding the time
 * 
 * @param what what the code wants to be returned, using the macros defined at the top of the document.
 * @param result a reference to the variable where the result should be assigned
 * @return int is 0 if the function went smoothly, will not be 0 if any errors occured
 */
int readRTC(uint8_t what, uint8_t* result);

#endif
