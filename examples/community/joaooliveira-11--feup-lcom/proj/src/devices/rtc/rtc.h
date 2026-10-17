#ifndef _RTC_PROJECT
#define _RTC_PROJECT

#include <minix/sysutil.h>
#include <lcom/lcf.h>

///@brief Set up the RTC.
///@note This function sets up the RTC by updating the time information and determining the RTC mode.
void (rtc_start)();

///@brief Output a command to the RTC.
///@note This function outputs a command to the RTC by writing to the appropriate register and retrieves the output from the RTC.
///@param command The command to be sent to the RTC.
///@param output Pointer to store the output received from the RTC.
///@return 0 if the output is successful, otherwise returns 1.
int (rtc_writeRead)(uint8_t rtc_command, uint8_t *rtc_output);

///@brief Check if RTC is in BCD or BIN mode.
///@note This function checks if the RTC is in BCD or BIN mode.
///@return 1 if RTC is in BCD mode, 0 if in BIN.
int (check_rtc_mode)();

///@brief Update the RTC time information.
///@note This function updates the time information by reading the appropriate RTC registers and storingthe information in the time_info structure.
///@return 0 if the update is successful, otherwise returns 1.
int (rtc_update_time)();

///@brief Convert BCD number to binary.
///@note This function converts a BCD number to binary format.
///@param number The BCD number to be converted.
///@return The converted binary number.
uint8_t (bcd_to_bin)(uint8_t bcd_number);

/// @note auxiliar functions used on rtc_update_time()
int (rtc_update_seconds)();

/// @note auxiliar functions used on rtc_update_time()
int (rtc_update_minutes)();

/// @note auxiliar functions used on rtc_update_time()
int (rtc_update_hours)();

/// @note auxiliar functions used on rtc_update_time()
int (rtc_update_year)();

/// @note auxiliar functions used on rtc_update_time()
int (rtc_update_month)();

/// @note auxiliar functions used on rtc_update_time()
int (rtc_update_day)();

#endif
