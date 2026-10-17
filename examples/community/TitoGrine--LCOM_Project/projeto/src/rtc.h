#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "MACROS.h"

#define BIT(n) (0x01<<(n))

/**
 * @brief Struct used to store date and time information obtained from
 * RTC.
 * 
 */
typedef struct {
  uint32_t day;       /**< Day */
  uint32_t month;     /**< Month */
  uint32_t year;      /**< Year */
  uint32_t hours;     /**< Hours */
  uint32_t minutes;   /**< Minutes */
  uint32_t seconds;   /**< Seconds */
} TIME;

/**
 * @brief Subscribes to the RTC's interruptions
 * 
 * @param bit_no    Variable that will store the RTC hook id (hardcoded to be 4)
 */
int rtc_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes from the RTC's interruptions
 * 
 */
int rtc_unsubscribe_int();

/**
 * @brief Checks if there was an alarm interruption, returning 1 if so, 0 if there were no interruptions
 * or if the interruption is not an alarm one.
 * 
 * @return int    alue of 1 if it read an alarm interruption
 */
int rtc_handler();

/**
 * @brief Enables alarm interruptions from the RTC
 * 
 */
void rtc_enable_alarm();

/**
 * @brief Disables alarm interrutions by the RTC
 * 
 */
void rtc_disable_alarm();

/**
 * @brief Given a time interval in seconds, sets an alarm interruption
 * to go off at the current time plus the given seconds.
 * Number of seconds pass 59 seconds will be ignored.
 * 
 * @param interval    Time interval in seconds until wanted alarm
 */
void rtc_set_alarm(uint8_t interval);

/**
 * @brief Reads from the register A to check if UIP (bit 7) is active
 * meaning the RTC is in th middle of an update.
 * 
 * @return true       RTC is in the middle of an update
 * @return false      RTC is not updating
 */
bool rtc_is_updating();

/**
 * @brief Reads the register B to check if DM (bit 2) is active low, 
 * meaning the RTC is using BCD to represent date and time values.
 * 
 * @return true       RTC is using BCD to represent values
 * @return false      RTC is using binary to represent values
 */
bool is_BCD();

/**
 * @brief Converts a number in BCD to decimal and returns it.
 * 
 * @param bcd_number  Number in BCD to convert
 * 
 * @return uint32_t   Number given in decimal representation
 */
uint32_t convert_BCD(uint32_t bcd_number);

/**
 * @brief Converts a number in decimal to BCD and returns it.
 * 
 * @param decimal_number  Number in decimal to convert
 * 
 * @return uint32_t       Number given in BCD representation
 */
uint32_t convert_decimal(uint32_t decimal_number);

/**
 * @brief Changes the attributes day, month and year of struct t of    
 * type TIME to the current day, month and year read by from the RTC.
 * 
 * @param t           Struct that stores date and time information to change
 */
void get_date(TIME* t);

/**
 * @brief Changes the attributes seconds, minutes and hours of struct 
 * t of type TIME to the current seconds, minutes and hours read by 
 * from the RTC.
 * 
 * @param t           Struct that stores date and time information to change
 */
void get_time(TIME* t);

/**
 * @brief Reads the current date and time from the RTC, converts it to
 * a user friendly string dd/mm/yyyy hh:mm:ss in the char array passed
 * as argument.
 * 
 * @param t           C string that will store the date and time
 */
void get_time_string(char t[20]);

#endif
