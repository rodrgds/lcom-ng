#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>

/**
 * @brief Sends a command to the RTC and reads the output.
 * @details This function sends a specified command to the RTC (Real-Time Clock) and reads the 
 * resulting output.
 * @param command The command to send to the RTC.
 * @param output Pointer to store the output read from the RTC.
 * @return 0 on success, 1 otherwise.
 */
int(rtc_output)(uint8_t command, uint8_t *output);

/**
 * @brief Gets the current time from the RTC.
 * @details This function retrieves the current time from the RTC and adjusts the hour value to 
 * account for time zones.
 * @return 0 on success, 1 otherwise.
 */
int(rtc_getTime)();

/**
 * @brief Converts a BCD (Binary-Coded Decimal) value to a binary value.
 * @details This function converts a given BCD value to its equivalent binary representation.
 * @param num The BCD value to convert.
 * @return The binary equivalent of the BCD value.
 */
uint8_t(bcd_to_binary)(uint8_t num);

#endif
