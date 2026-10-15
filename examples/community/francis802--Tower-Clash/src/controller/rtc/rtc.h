/**
 * @file rtc.h
 * 
 * @brief Provides macros and functions to use the RTC
*/

#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

// Macros
#define RTC_MASK            5
#define IRQ_RTC             8
#define REGISTER_INPUT      0x70
#define REGISTER_OUTPUT     0x71

#define REGISTER_UPDATING   10
#define REGISTER_COUNTING   11

#define SECONDS    0
#define MINUTES    2
#define HOURS      4   
#define DAY        7
#define MONTH      8
#define YEAR       9

#define BINARY              BIT(2)
#define UPDATING            BIT(7)

/**
 * @brief Struct that stores the real time information
*/
typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} real_time_info;

/**
 * @brief This function check the RTC mode and updates the real time information
*/
void rtc_setup();

/**
 * @brief Subscribes the RTC interrupts
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int rtc_subscribe_interrupts();

/**
 * @brief Unsubscribes the RTC interrupts
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int rtc_unsubscribe_interrupts();

/**
 * @brief This function writes a command to the RTC and reads the output
 * and stores it in the output variable. 
 * 
 * @param command Command to be outputted
 * @param output Pointer to the variable where the output will be stored
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int rtc_output(uint8_t command, uint8_t *output);

/**
 * @brief Checks if the RTC is updating
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int rtc_is_updating();

/**
 * @brief Checks if the RTC is in binary mode
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int rtc_is_binary();

/**
 * @brief Checks if the RTC is in BCD mode
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int rtc_is_bcd();

/**
 * @brief This function updates the real time information by reading the RTC
 * output using the rtc_output() function.
 * 
 * @return int 0 if no errors occurred, 1 otherwise
*/
int rtc_update_time();

/**
 * @brief Auxiliary function to convert a 8-bit BCD number to an 8-bit binary number.
 * 
 * @param bcd_number BCD number to be converted
 * 
 * @return uint8_t Converted number
*/
uint8_t to_binary(uint8_t bcd_number);

#endif
