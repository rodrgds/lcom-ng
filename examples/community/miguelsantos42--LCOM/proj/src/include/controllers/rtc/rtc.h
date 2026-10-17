#ifndef LCOM_RTC_H_
#define LCOM_RTC_H_

#include <lcom/lcf.h>
#include <stdint.h>

/*!
 * @def RTC_ADDR_REG
 * @brief RTC address register port.
 */
#define RTC_ADDR_REG 0x70

/*!
 * @def RTC_DATA_REG
 * @brief RTC data register port.
 */
#define RTC_DATA_REG 0x71

#define RTC_SECONDS 0
#define RTC_MINUTES 2
#define RTC_HOURS 4
#define RTC_DAY 7
#define RTC_MONTH 8
#define RTC_YEAR 9
#define RTC_UPDATE_STATUS 10
#define RTC_COUNT_STATUS 11
#define IRQ_RTC 8
#define RTC_UPDATE_IN_PROGRESS BIT(7)
#define RTC_BINARY_MODE BIT(2)

/*!
 * @struct rtc_time_t
 * @brief Structure representing the RTC time and date.
 */
typedef struct {
    uint8_t seconds; /*!< Seconds (0-59) */
    uint8_t minutes; /*!< Minutes (0-59) */
    uint8_t hours;   /*!< Hours (0-23) */
    uint8_t day;     /*!< Day of the month (1-31) */
    uint8_t month;   /*!< Month (1-12) */
    uint8_t year;    /*!< Year */
} rtc_time_t;

/*!
 * @brief Reads a value from the RTC register.
 *
 * @param[in] command RTC register to read from.
 * @param[out] output Pointer to store the read value.
 * @return 0 on success, non-zero otherwise.
 */
int rtc_output(uint8_t command, uint8_t *output);

/*!
 * @brief Checks if the RTC is currently updating its registers.
 *
 * @return 1 if updating, 0 if not updating, -1 on error.
 */
int rtc_is_updating();

/*!
 * @brief Checks if the RTC is in binary mode.
 *
 * @return 1 if in binary mode, 0 if in BCD mode, -1 on error.
 */
int rtc_is_binary();

/*!
 * @brief Converts a BCD number to binary.
 *
 * @param[in] bcd_number The BCD number to convert.
 * @return The binary representation of the BCD number.
 */
uint8_t rtc_bcd_to_binary(uint8_t bcd_number);

/*!
 * @brief Reads the current time and date from the RTC.
 *
 * @param[out] time Pointer to an rtc_time_t structure to store the result.
 * @return 0 on success, non-zero otherwise.
 */
int rtc_read_time(rtc_time_t *time);

/*!
 * @brief Subscribes RTC interrupts.
 *
 * @param[out] bit_no Pointer to variable to store the interrupt bit number.
 * @return 0 on success, non-zero otherwise.
 */
int rtc_subscribe_int(uint8_t *bit_no);

/*!
 * @brief Unsubscribes RTC interrupts.
 *
 * @return 0 on success, non-zero otherwise.
 */
int rtc_unsubscribe_int();

/*!
 * @brief RTC interrupt handler. Updates the internal current_time variable.
 */
void rtc_ih();

/*!
 * @brief Initializes the RTC by reading the current time.
 *
 * @return 0 on success, non-zero otherwise.
 */
int rtc_init();

/*!
 * @brief Gets the current RTC time.
 *
 * @param[out] time Pointer to an rtc_time_t structure to store the result.
 * @return 0 on success, non-zero otherwise.
 */
int rtc_get_time(rtc_time_t *time);

#endif
