/** @defgroup rtc rtc
 * @{
 *
 * utils for rtc
 */
#ifndef RTC_H
#define RTC_H

#include "realtimeclock.h"

typedef struct {
  uint32_t year;
  uint32_t month;
  uint32_t day;
  uint32_t hour;
  uint32_t minute;
  uint32_t second;
} Date_and_Time;

/**
 * @brief Subscribes RTC interrupts
 * @return Returns 0 if succesful, not -1 otherwise
 */
int rtc_subscribe_int();

/**
 * @brief Unsubscribes RTC interrupts
 * @return Returns 0 if successful, not -1 otherwise
 */
int rtc_unsubscribe_int();

/**
 * @brief RTC Interrupt Handler
 * @return Returns 0 if successful, not -1 otherwise
 */
int rtc_ih();

/**
 * @brief Enables RTC Update Interrupts
 * @return Returns 0 if successful, not -1 otherwise
 */
int rtc_enable_update_interrupts();

/**
 * @brief Disables RTC Update Interrupts
 * @return Returns 0 if successful, not -1 otherwise
 */
int rtc_disable_update_interrupts();

/**
 * @brief Updates the currently date when  an update interrupt is recieved
 * @return Returns 0 if successful, not -1 otherwise
 */
int update_RTC_date();

/**
 * @brief Return a string with current the date and time
 * @return Returns
 */
char * get_current_date_and_time(bool to_string);

#endif

/**@}*/
