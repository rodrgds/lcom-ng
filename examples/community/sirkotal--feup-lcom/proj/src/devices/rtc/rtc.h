#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>
#include <stdint.h>

#define IRQ_RTC 8 /**< @brief RTC IRQ line*/

#define RTC_ADDR_REG 0x70 /**< @brief RTC register that is loaded with the address of the RTC register to be accessed */
#define RTC_DATA_REG 0x71 /**< @brief RTC register that is used to transfer the data to/from the RTC’s accessed register */

#define RTC_SECONDS_REG 0        /**< @brief RTC's Seconds */
#define RTC_SECONDS_ALARM_REG 1  /**< @brief RTC's Seconds Alarm */
#define RTC_MINUTES_REG 2        /**< @brief RTC's Minutes */
#define RTC_MINUTES_ALARM_REG 3  /**< @brief RTC's Minutes Alarm */
#define RTC_HOURS_REG 4          /**< @brief RTC's Hours */
#define RTC_HOURS_ALARM_REG 5    /**< @brief RTC's Hours Alarm */
#define RTC_WEEK_DAY 6           /**< @brief RTC's Day of the Week */
#define RTC_MONTH_DATE 7         /**< @brief RTC's Date of the Month */
#define RTC_MONTH 8              /**< @brief RTC's Month */
#define RTC_YEAR 9               /**< @brief RTC's Year */

#define RTC_REG_A 10 /**< @brief RTC's Register A */
#define RTC_REG_B 11 /**< @brief RTC's Register B */
#define RTC_REG_C 12 /**< @brief RTC's Register C */
#define RTC_REG_D 13 /**< @brief RTC's Register D */

#define BINARY BIT(2) /**< @brief Binary mode is enabled */

#define IRQF BIT(7)    /**< @brief RTC IRQ line active */
#define PF   BIT(6)    /**< @brief RTC Periodic interrupt pending */
#define AF   BIT(5)    /**< @brief RTC Alarm interrupt pending */
#define UF   BIT(4)    /**< @brief RTC Update interrupt pending */

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} rtc_time_info; /**< @brief Structure that stores the RTC's time data */

/**
 * @brief Subscribes and enables RTC interrupts
 * 
 * @param bit_no Bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes RTC interrupts
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (rtc_unsubscribe_int)();

/**
 * @brief RTC interrupt handler
 */
void (rtc_ih)();

/**
 * @brief Sends a command to the RTC address register and reads the corresponding data from the RTC data register
 * 
 * @param command Command to write
 * @param output Variable to store the output
 * @return 0 on success, 1 otherwise
 */
int (rtc_output)(uint8_t command, uint8_t *output);

/**
 * @brief Checks if the RTC's counters are being incremented in binary mode
 * 
 * @return Return the result of the bitwise AND operation between the retrieved value and BIT(2) if successful; otherwise, it returns 1
 */
int (rtc_check_binary)();

/**
 * @brief Checks if the RTC isn't updating any values
 * 
 * @return Return the result of the bitwise AND operation between the retrieved value and BIT(7) if successful; otherwise, it returns 1
 */
int (rtc_is_updating());

/**
 * @brief Updates the RTC's time values
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (update_rtc_time());

#endif

