#ifndef G6_RTC_H
#define G6_RTC_H

#include <lcom/lcf.h>

/**
 * @brief Sets up an alarm in the RTC after a certain time.
 * @param alarm_hours Hours to elapse until alarm.
 * @param alarm_minutes Minutes to elapse until alarm.
 * @param alarm_seconds Seconds to elapse until alarm.
 * @return 0 if successful, non-zero otherwise.
 */
int (rtc_set_alarm) (uint8_t alarm_hours, uint8_t alarm_minutes, uint8_t alarm_seconds);

/**
 * @brief Processes RTC interrupts by setting a flag if an alarm was generated.
 */
void (rtc_ih) ();

/**
 * @brief Disables the RTC alarm interrupts.
 * @return 0 if successful, non-zero otherwise.
 */
int (rtc_disable_alarm) ();

/**
 * @brief Subscribes to RTC interrupts using the IRQ_REENABLE policy.
 * @param bit_no Bit number to use in the RTC hook ID.
 * @return 0 if successful, non-zero otherwise.
 */
int (rtc_subscribe_int) (uint8_t* bit_no);

/**
 * @brief Unsubscribes to RTC interrupts.
 * @return 0 if successful, non-zero otherwise.
 */
int (rtc_unsubscribe_int) ();

/**
 * @brief Returns the current time in the RTC registers.
 * @param hours Address where hours will be stored.
 * @param minutes Address where the minutes will be stored.
 * @param seconds Address where the seconds will be stored.
 * @return 0 if successful, non-zero otherwise.
 */
int (rtc_get_time) (uint8_t* hours, uint8_t* minutes, uint8_t* seconds);

/**
 * @brief Reads a register from the RTC.
 * @param reg Address of the register to be read.
 * @param data Address where the register data will be stored.
 * @return 0 if successful, non-zero otherwise.
 */
int (rtc_read_from_reg) (uint32_t reg, uint8_t* data);

/**
 * @brief Writes a byte to a RTC register.
 * @param reg Address of the register to write to.
 * @param data Byte to be written.
 * @return 0 if successful, non-zero otherwise.
 */
int (rtc_write_to_reg) (uint32_t reg, uint8_t data);

/**
 * @brief Converts a BCD number to binary format.
 * @param bcd Number to be converted.
 * @return The corresponding binary format number.
 */
uint8_t (to_binary) (uint8_t bcd);

/**
 * @brief Converts a binary number to BCD format.
 * @param binary Number to be converted.
 * @return The corresponding BCD format number.
 */
uint8_t (to_bcd) (uint8_t binary);

#endif //G6_RTC_H
