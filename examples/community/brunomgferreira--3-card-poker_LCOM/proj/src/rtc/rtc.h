#ifndef __RTC_H_
#define __RTC_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>

#define RTC_IRQ 8 /**< RTC interrupt line */
#define RTC_HOOK_BIT 5 /**< RTC hook bit for interrupts */

#define SELECT_REG 0x70 /**< RTC select register port */
#define REG_OUTPUT 0x71 /**< RTC data register port */

#define STAT_REG_A 0X0A /**< RTC status register A */
#define STAT_REG_B 0X0B /**< RTC status register B */
#define STAT_REG_C 0X0C /**< RTC status register C */
#define STAT_REG_D 0X0D /**< RTC status register D */

#define REGISTER_UPDATING 10 /**< RTC updating state */
#define REGISTER_COUNTING 11 /**< RTC counting state */

#define SECONDS 0x00 /**< RTC seconds register */
#define MINUTES 0x02 /**< RTC minutes register */
#define HOURS 0x04 /**< RTC hours register */
#define DAY 0x07 /**< RTC day register */
#define MONTH 0x08 /**< RTC month register */
#define YEAR 0x09 /**< RTC year register */

#define BINARY BIT(2) /**< Binary mode flag in RTC */
#define UIP BIT(7) /**< Update in progress flag in RTC */

/**
 * @brief Structure that holds time information
 */
typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
} time_info;

/**
 * @brief Initialize the RTC
 */
void (rtc_setup)();

/**
 * @brief Subscribe to RTC interrupts
 * @return 0 on success, non-zero otherwise
 */
int (rtc_subscribe_int)();

/**
 * @brief Unsubscribe from RTC interrupts
 * @return 0 on success, non-zero otherwise
 */
int (rtc_unsubscribe_int)();

/**
 * @brief Read a register from the RTC
 * @param port RTC register port
 * @param data Pointer to store the read data
 * @return 0 on success, non-zero otherwise
 */
int (rtc_read_register)(uint8_t port, uint8_t *data);

/**
 * @brief Read the current time from the RTC
 * @return 0 on success, non-zero otherwise
 */
int (rtc_read_time)();

/**
 * @brief Convert a BCD number to binary
 * @param bcd_number BCD number to convert
 * @return Binary equivalent
 */
uint8_t (to_binary)(uint8_t bcd_number);

/**
 * @brief Check if a given year is a leap year
 * @param year Year to check
 * @return true if leap year, false otherwise
 */
bool (is_leap_year)(uint16_t year);

/**
 * @brief Get the number of days in a month for a given year
 * @param month Month (1-12)
 * @param year Year
 * @return Number of days in the month
 */
uint8_t (get_days_in_month)(uint8_t month, uint16_t year);

/**
 * @brief Increment time by one second
 */
void (rtc_increment_time_by_one_second)();

/**
 * @brief Increment time by one day
 * @param date_time Pointer to time_info structure to update
 */
void (increment_time_by_one_day)(time_info *date_time);

/**
 * @brief Calculate the time left to reach a target time
 * @param target_time Target time
 * @return String representation of time left
 */
char* (time_left_to_target_time)(time_info target_time);

#endif /* __RTC_H */
