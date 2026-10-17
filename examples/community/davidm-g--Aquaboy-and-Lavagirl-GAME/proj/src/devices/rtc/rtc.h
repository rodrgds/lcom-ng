#ifndef __RTC_H
#define __RTC_H
#include "../utils.h"
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#define RTC_CTL_REGISTER 0x70
#define RTC_OUT_REGISTER 0x71
#define RTC_IRQ 8
#define RTC_UPDATING BIT(7)
#define RTC_BINARY BIT(2)
#define RTC_UPDATE_REG 10
#define RTC_COUNTING_REG 11
#define READ_SECONDS 0
#define READ_MINUTES 2
#define READ_HOURS 4
#define READ_DAYS 7
#define READ_MONTHS 8
#define READ_YEARS 9

typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} rtc_values;
/**
 * @brief Reads the output of the Real-Time Clock (RTC) using the specified command byte.
 *
 * This function reads the output of the RTC using the specified command byte and stores the result in the provided pointer.
 *
 * @param cmdbyte The command byte to send to the RTC.
 * @param result Pointer to store the result of the RTC output.
 * @return 0 on success, non-zero otherwise.
 */
int read_rtc_output(uint8_t cmdbyte, uint8_t *result);

/**
 * @brief Subscribes to RTC interrupts.
 *
 * This function subscribes to RTC interrupts and enables the specified bit number.
 *
 * @param bit_no Pointer to store the bit number associated with the RTC interrupt.
 * @return 0 on success, non-zero otherwise.
 */
int (rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from RTC interrupts.
 *
 * This function unsubscribes from RTC interrupts and disables the previously subscribed bit number.
 *
 * @return 0 on success, non-zero otherwise.
 */
int (rtc_unsubscribe_int)();

/**
 * @brief Checks if the RTC is currently updating.
 *
 * This function checks if the RTC is currently updating its values.
 *
 * @return 1 if the RTC is updating, 0 otherwise.
 */
int is_updating_rtc();

/**
 * @brief Checks if the RTC is in binary mode.
 *
 * This function checks if the RTC is in binary mode.
 *
 * @return true if the RTC is in binary mode, false otherwise.
 */
bool is_binary_rtc();

/**
 * @brief Converts a Binary-Coded Decimal (BCD) number to binary.
 *
 * This function converts a BCD number to binary.
 *
 * @param bcd_number The BCD number to convert.
 * @return The binary representation of the BCD number.
 */
uint8_t to_binary(uint8_t bcd_number);

/**
 * @brief Updates the values of the RTC.
 *
 * This function updates the values of the RTC, including the date and time.
 *
 * @return 0 on success, non-zero otherwise.
 */
int rtc_update_values();

/**
 * @brief Starts the RTC.
 *
 * This function starts the RTC and initializes its settings.
 */
void start_rtc();
#endif
