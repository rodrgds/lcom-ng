/**
 * @file rtc
 * @brief Functions for using the real time clock
 */

#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

/** @defgroup RTC RTC
 * @{
 *
 * Functions for using the real time clock
 */

#include "rtc_info.h"

typedef struct {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} date_time_info;

/**
 * @brief Reads the data from rtc
 *
 * @param address The address location of the rtc register
 * @param data The data obtained from the rtc register
 * @return Return 0 upon success and 1 otherwise
 */
int (rtc_read_data)(uint8_t address, uint8_t *data);
/**
 * @brief Writes the data in rtc
 *
 * @param address The address location of the rtc register
 * @param data The data to write in the rtc register
 * @return Return 0 upon success and 1 otherwise
 */
int (rtc_write_data)(uint8_t address, uint8_t data);

/**
 * @brief Subscribes rtc interrupt
 *
 * @param bit_no Bit number of rtc IRQ
 * @return Return 0 upon success and 1 otherwise
 */
int (rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes rtc interrupt
 * @return Return 0 upon success and 1 otherwise
 */
int (rtc_unsubscribe_int)();

/**
 * @brief Check if rtc is updating
 * @return Return 0 if false, 1 if true and 2 if an error occurred
 */
int (rtc_is_updating)();

/**
 * @brief Check if rtc data form is binary or BCD
 * @return Return 0 if BCD, 1 if binary and 2 if an error occurred
 */
int (rtc_is_binary)();

/**
 * @brief Get the information about time and date
 * @param info A struct that contain information about time and date
 * @return Return 0 upon success and 1 otherwise
 */
int (get_time_date_info)(date_time_info *info);

/**
 * @brief Convert BCD data format to Binary data format
 * @param bcd The bcd data to be converted
 * @return Return the converted binary data
 */
uint8_t (bcd_to_binary)(uint8_t bcd);

/**@}*/

#endif
