/**
 * @file timer.h
 * @brief ...
 * @author musta
 * @date 03/05/2023
 */

#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <stdint.h>
#include <stdio.h>

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define mask_rtc BIT(4)

typedef struct {
  uint8_t ano;
  uint8_t mes;
  uint8_t dia;
  uint8_t hora;
  uint8_t minuta;
  uint8_t segunda;
} time_info;

typedef struct {
  int months;
  int days;
  int hours;
  int minutes;
} time_difference;


/**
 @brief Reads data from the Real-Time Clock (RTC).
This function sends a command to the RTC and reads the response into the provided value pointer.
It uses IO ports 0x70 and 0x71 to communicate with the RTC.
 @param command The command to be sent to the RTC.
 @param value A pointer to the variable where the response from the RTC will be stored.
 @return 0 if the operation is successful, 1 otherwise.
*/
int read_rtc(uint8_t command, uint8_t * value);

/**
 @brief Converts BCD to binary or returns the value if it's already in binary.
This function converts the Binary-Coded Decimal (BCD) value to binary form if is_bcm is false.
If is_bcm is true, it returns the original value as it's assumed to be in binary form.
 @param value The value to be converted or returned.
 @param is_bcm A flag indicating whether the value is already in binary form (true) or in BCD form (false).
 @return The converted value in binary form or the original value if it's already in binary form.
*/
uint8_t result_time(uint8_t value, uint8_t is_bcm);

/**
 @brief Sets the system time by reading from the Real-Time Clock (RTC).
This function checks if the RTC is currently updating, and if not, it reads the time values
from the RTC and stores them in the data structure. It handles conversion from Binary-Coded Decimal
(BCD) to binary form if necessary. The time values include seconds, minutes, hours, days, months, and years.
 @return 0 if the operation is successful, 1 otherwise.
*/
int set_time();


/**
 @brief Determines if a year is a leap year.
This function checks if a year is a leap year according to the Gregorian calendar rules.
A year is a leap year if it is evenly divisible by 4, but not by 100, unless it is also evenly divisible by 400.
 @param year The year to check.
 @return Returns 1 if the year is a leap year, 0 otherwise.
*/
uint8_t is_leap_year(uint8_t year);

/**
 @brief Determines the number of days in a given month of a given year.
This function calculates the number of days in a specified month for a particular year.
It takes into account leap years for the month of February.
 @param month The month for which to determine the number of days (1-12).
 @param year The year, which is used to check if February is a leap year.
 @return The number of days in the specified month.
*/
uint8_t days_in_month(uint8_t month, uint8_t year);

/**
 @brief Handles Real-Time Clock (RTC) interrupts.
This function is an interrupt handler that is called when an RTC interrupt is received.
It increments the seconds, and if necessary, the minutes, hours, days, months, and years.
It takes into account the number of seconds in a minute, minutes in an hour, hours in a day, and days in a month, including leap years.
*/
void add_second();

/**
 @brief Determines if a given time is in the past relative to a current time.
This function compares two time_info objects: current_time and to_check.
It checks if the to_check time is earlier than the current_time by comparing the year, month, day, hour, and minute.
 @param current_time The reference time_info object against which to_check is compared.
 @param to_check The time_info object to check if it's in the past relative to current_time.
 @return true if to_check represents a time in the past relative to current_time, false otherwise.
*/
bool is_past(time_info current_time, time_info to_check);

/**
 @brief Calculates the difference between two time_info objects.
This function calculates the difference between two time_info objects: current_time and to_check.
The difference is calculated in terms of months, days, hours, and minutes.
If to_check represents a time in the past relative to current_time, the function returns a time_difference object with all fields set to -1.
 @param current_time The reference time_info object against which to_check is compared.
 @param to_check The time_info object for which the difference from current_time is calculated.
 @return A time_difference object representing the time difference between to_check and current_time.
*/
time_difference calculate_time_difference(time_info current_time, time_info to_check);

#endif
