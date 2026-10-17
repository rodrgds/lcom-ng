#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include <lcom/lcf.h>

/**
 * @brief Struct to represent date and time.
 */
typedef struct{
    uint8_t seconds;    /**< Seconds (0-59) */
    uint8_t minutes;    /**< Minutes (0-59) */
    uint8_t hours;      /**< Hours (0-23) */
    uint8_t day;        /**< Day of the month (1-31) */
    uint8_t month;      /**< Month (1-12) */
    uint8_t year;       /**< Year (0-99) */
    uint8_t day_week;   /**< Day of the week (1-7) */
} date_time;

/**
 * @brief Subscribe to RTC interrupts.
 *
 * @param bit_no Bit number that will be activated when an interrupt is generated
 * @return Return 0 upon success, non-zero otherwise.
 */
int (rtc_subscribe_int) (uint8_t *bit_no);

/**
 * @brief Unsubscribe from RTC interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (rtc_unsubscribe_int) ();

/**
 * @brief Read value from a specific register of the RTC.
 *
 * @param reg Register to read from.
 * @param value Pointer to store the read value.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (read_register) (uint32_t reg, uint8_t* value);

/**
 * @brief Write value to a specific register of the RTC.
 *
 * @param reg Register to write to.
 * @param value Value to write.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (write_register) (uint32_t reg, uint32_t value);

/**
 * @brief Read RTC config.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (rtc_read_conf) ();

/**
 * @brief Convert BBCD to Decimal.
 *
 * @param value Pointer to the BCD value to convert.
 */
void (convert_bcd_decimal) (uint8_t* value);

/**
 * @brief Convert Decimal to BCD.
 *
 * @param value Pointer to the decimal value to convert.
 */
void (convert_decimal_bcd) (uint8_t* value);

/**
 * @brief Get current date and time from RTC.
 *
 * @param dt Pointer to store the retrieved date and time.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (rtc_get_datetime) (date_time* dt);

/**
 * @brief Display the date.
 *
 * @param dt Pointer to the date and time structure.
 */
void (show_date) (date_time* dt);

/**
 * @brief Display the time.
 *
 * @param dt Pointer to the date and time structure.
 */
void (show_time) (date_time* dt);

/**
 * @brief Adjust date and time values from BCD to Decimal.
 *
 * @param dt Pointer to the date and time structure to adjust.
 */
void (adjust_dt_values) (date_time* dt);

/**
 * @brief Set an alarm for a specific time.
 *
 * @param sec Seconds offset to set the alarm. At most 1 day in seconds
 * @return Return 0 upon success, non-zero otherwise.
 */
int (set_alarm) (uint32_t sec);

/**
 * @brief Activate the alarm interrupt.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (active_alarm_int) ();

/**
 * @brief Disables the alarm interrupts on the RTC (Real-Time Clock).
 *
 * This function configures the RTC to disable any alarm interrupts that might be set.
 *
 * @return int 0 on success, non-zero on failure.
 */
int (disable_alarm_int) ();

/**
 * @brief RTC interrupt handler.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (rtc_ih) ();

#endif /* RTC_H */
