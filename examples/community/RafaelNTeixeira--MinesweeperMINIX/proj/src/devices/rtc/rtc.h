#pragma once

#include "rtc_commands.h"
#include <lcom/lcf.h>
#include <math.h>


/**
 * @brief The Time structure represents a specific time and date.
*/
typedef struct {
  unsigned long hour;
  unsigned long minute;
  unsigned long second;

  unsigned long day;
  unsigned long month;
  unsigned long year;
} Time;

/**
 * @brief Initializes the Time struct.
 * @return timer
 */
Time* initTime();
/**
 * @brief Subscribes and enables the RTC interrupts.
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int rtc_subscribe_int();

/**
 * @brief Unsubscribes the RTC interrupts.
 * @return return 0 upon success and -1 otherwise
 */
int rtc_unsubscribe_int();

/**
 * @brief Deletes the time.
 * @param time
 */
void deleteTime(Time* time);

/**
 * @brief Get the current time and date.
  * @param time
 */
void getCurrentTime(Time* time);

/**
 * @brief Converts values from bcd to decimal.
 * @param bcd Binary time.
 * @return Decimal time.
 */
unsigned long bcdToDec(unsigned long bcd);
