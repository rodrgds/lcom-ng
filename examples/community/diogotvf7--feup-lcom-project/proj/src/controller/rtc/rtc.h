#pragma once
#include <lcom/lcf.h>
#include "../utils.h"

#define IRQ_RTC 8
#define REGISTER_COUNTING 11
#define IS_BINARY BIT(2)
#define UPDATING BIT(7)
#define REG_INPUT 0x70
#define REG_OUTPUT 0x71
#define REGISTER_UPDATING   10
#define SECONDS    0
#define MINUTES    2
#define HOURS      4   
#define DAY        7
#define MONTH      8
#define YEAR       9

/**
 * @brief Structure representing the real-time values (year, month, day, hour, minute, second).
 */
typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} real_time;

/**
 * @brief Initializes the RTC (Real-Time Clock).
 */
void rtc_init();

/**
 * @brief Subscribes to RTC interrupts.
 *
 * @return Bitmask of the RTC's IRQ line.
 */
int rtc_subscribe_int();

/**
 * @brief Unsubscribes from RTC interrupts.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int rtc_unsubscribe_int();

/**
 * @brief Checks if the RTC is using binary format.
 *
 * @return 1 if binary format is used, 0 otherwise.
 */
int is_binary();

/**
 * @brief Reads the output of the RTC.
 *
 * @param command The command to send to the RTC.
 * @param output Pointer to store the output value.
 * @return 0 upon success, non-zero otherwise.
 */
int read_rtc_output(uint8_t command, uint8_t *output);

/**
 * @brief Checks if the RTC is updating.
 *
 * @return 1 if the RTC is updating, 0 otherwise.
 */
int rtc_is_updating();

/**
 * @brief Updates the time values (year, month, day, hour, minute, second) from the RTC.
 *
 * @return 0 upon success, non-zero otherwise.
 */
int rtc_update_time();


