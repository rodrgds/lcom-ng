#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>

bool is_rtc_in_binary_store;

/**
 * @brief reads the rtc register corresponding to the command and stores it in place_to_store  
 * @param cmd command to be sent to the rtc
 * @param place_to_store pointer to the place where the output will be stored 
 * @return int 0 if success, 1 otherwise
 */
int rtc_input(uint8_t cmd, uint *place_to_store);

/**
 * @brief writes the output in the rtc register corresponding to the command
 * 
 * @param cmd command to be sent to the rtc
 * @param output content to be written in the rtc register
 * @return int 0 if success, 1 otherwise
 */
int rtc_output(uint8_t cmd, uint8_t output);

/**
 * @brief checks if rtc is updating
 * 
 * @return false if rtc is updating, true otherwise 
 */
bool is_rtc_updating();

/**
 * @brief updates the time of the RTC
 * 
 * @param hours hours to be updated
 * @param minutes minutes to be updated
 * @return int 0 if success, 1 otherwise
 */
int rtc_update_time(uint *hours, uint *minutes);

/**
 * @brief confirms if the rtc is in binary mode
 * 
 * @return true if rtc is in binary mode, false otherwise
 */
bool is_rtc_in_binary();

/**
 * @brief converts a bcd number to binary
 * 
 * @param bcd number to be converted
 * @return int binary number corresponding to the bcd number
 */
int bcd_to_binary(uint8_t bcd);

#endif
