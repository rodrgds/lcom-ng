#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_IRQ 8

#define RTC_SECONDS 0
#define RTC_MINUTES 2
#define RTC_HOURS 4
#define RTC_DAY 7
#define RTC_MONTH 8
#define RTC_YEAR 9

#define RTC_A 10
#define RTC_B 11
#define RTC_C 12
#define RTC_D 13

#define RTC_A_UIP BIT(7)
#define RTC_B_SET BIT(7)
#define RTC_B_UIE BIT(4)
#define RTC_C_UF BIT(4)

/** @defgroup rtc rtc
 * @brief Functions for interacting with the RTC
*/

/**
 * @brief Subscribes and enables RTC interrupts
 * @param bit_no Pointer to the variable where the bit mask will be stored
 * @return 0 upon success, 1 otherwise
*/
int (rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes RTC interrupts
 * @return 0 upon success, 1 otherwise
*/
int (rtc_unsubscribe_int)();

/**
 * @brief Checks if the RTC is updating
 * @return 1 if the RTC is updating, 0 otherwise
*/
int (can_read_datetime)();

/**
 * @brief Reads the date and time from the RTC
 * @param reg The date/time unit to read
 * @return 0 upon success, 1 otherwise
*/
int (read_datetime)(uint8_t reg);

/**
 * @brief RTC interrupt handler
*/
int (rtc_ih)();

/**
 * @brief Enables or disables RTC update interrupts
 * @param enable True to enable, false to disable
 * @return 0 upon success, 1 otherwise
*/
int (enable_update_interrupts)(bool enable);


#endif 
