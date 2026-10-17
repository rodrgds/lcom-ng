#ifndef RTC_H
#define RTC_H

#include <stdbool.h>
#include <stdint.h>

#define RTC_ADDR_REG 0x70 /** RTC address register */
#define RTC_DATA_REG 0X71 /** RTC data register */

#define RTC_IRQ 8 /** RTC IRQ line */

#define HOURS 4 /** Hours register */
#define MINUTES 2 /** Minutes register */
#define SECONDS 0 /** Seconds register */
#define RTC_REG_A 0x0A /** Register A */
#define RTC_REG_B 0x0B /** Register B */
#define RTC_REG_C 0x0C /** Register C */
#define RTC_REG_D 0x0D /** Register D */
#define BLOCKED_BIT BIT(7) /**< Blocked bit */
#define BINARY_BIT BIT(2) /**< Binary bit */

/**
 * @struct rtc_time
 * @brief Structure to hold the time information.
 */
typedef struct {
  uint8_t hours;    /** Hours value */
  uint8_t minutes;  /** Minutes value */
  uint8_t seconds;  /** Seconds value */
} rtc_time;

/**
 * @brief Subscribes RTC interrupts.
 *
 * @param bit_no Pointer to the variable that will store the bit mask for the RTC interrupt.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes RTC interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int (rtc_unsubscribe_int)();

/**
 * @brief Reads the value of a time register.
 *
 * @param reg The register to read from.
 * @param output Pointer to the variable that will store the register value.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (read_time_register)(uint8_t reg, uint8_t* output);

/**
 * @brief Checks if the RTC is blocked.
 *
 * @return Return 1 if the RTC is blocked, 0 otherwise.
 */
int (rtc_is_blocked)();

/**
 * @brief Checks if the RTC is in binary mode.
 *
 * @return Return 1 if the RTC is in binary mode, 0 otherwise.
 */
int (rtc_is_binary)();

/**
 * @brief Reads the current time from the RTC.
 *
 * @param time Pointer to the rtc_time structure that will store the current time.
 * @return Return 0 upon success, non-zero otherwise.
 */
int (rtc_read_time)(rtc_time *time);

/**
 * @brief Updates the time in the RTC, uses timer interrupts to keep track of time.
 *
 * @param time Pointer to the rtc_time structure that contains the new time.
 */
void (rtc_update_time)(rtc_time *time);

/**
 * @brief Converts a BCD (Binary-Coded Decimal) value to decimal.
 *
 * @param bcd The BCD value to convert.
 * @return The decimal value.
 */
uint8_t (convert_bcd_to_decimal)(uint8_t *bcd);

#endif /* RTC_H */
