#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/**
 * @brief Struct that represents the necessary information of the RTC.
*/
typedef struct {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t day;
  uint8_t month;
  uint8_t year;
} rtc_info;

/**
 * @brief Function that initializes the RTC.
*/
void (rtc_initialize)();

/**
 * @brief Function that reads from the RTC.
 * @param command Command to read from the RTC.
 * @param output Variable to store the value read from the RTC.
 * @return 0 if successful, 1 otherwise.
*/
int (read_from_rtc)(uint8_t command, uint8_t *output);

/**
 * @brief Function that checks if the RTC is updating.
 * @return 0 if the RTC is not updating, 1 otherwise.
*/
int (check_update_status)();

/**
 * @brief Function that, first, checks if the RTC is in binary mode and, if so, gets the time in binary. If not, converts the time to binary.
 * @param selected_bit Bit to get the time in binary of a certain field (seconds, minutes, hours, day, month, year).
 * @return Time in binary.
*/
uint8_t (get_time_in_binary)(int selected_bit);

/**
 * @brief Function that updates the date of the RTC.
 * @return 0 if successful, 1 otherwise.
*/
int (update_date)();

#endif
