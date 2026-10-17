/**
 * @file rtc.h
 * @brief Header file for the RTC controller module.
 */

#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define IRQ_RTC       8
#define RTC_ADDR_REG  0x70
#define RTC_DATA_REG  0x71

#define RTC_SECONDS   0
#define RTC_MINUTES   2
#define RTC_HOURS     4
#define RTC_DAY       7
#define RTC_MONTH     8
#define RTC_YEAR      9

#define RTC_REG_A     10
#define UIP           BIT(7)
#define DV2           BIT(6)
#define DV1           BIT(5)
#define DV0           BIT(4)
#define RS3           BIT(3)
#define RS2           BIT(2)
#define RS1           BIT(1)
#define RS0           BIT(0)

#define RTC_REG_B     11
#define SET           BIT(7)
#define PIE           BIT(6)
#define AIE           BIT(5)
#define UIE           BIT(4)
#define SQWE          BIT(3)
#define DM            BIT(2)
#define HOUR_MODE     BIT(1)
#define DSE           BIT(0)

#define RTC_REG_C     12
#define IRQF          BIT(7)
#define PF            BIT(6)
#define AF            BIT(5)
#define UF            BIT(4)

#define RTC_REG_D     13
#define VRT           BIT(7)

/**
 * @brief Struct to hold the RTC info
 */
typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} rtc_info;

/**
 * @brief Sets the Real-Time Clock (RTC) registers.
 */
void rtc_set();

/**
 * @brief Subscribes to RTC interrupts.
 *
 * @param bit_no Pointer to the variable that will store the bit mask for the RTC interrupt.
 * @return Return 0 upon success, non-zero otherwise.
 */
int rtc_subscribe(uint8_t *bit_no);

/**
 * @brief Unsubscribes from RTC interrupts.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int rtc_unsubscribe();

/**
 * @brief Sends a command to the RTC and retrieves the output.
 *
 * @param command The command to be sent to the RTC.
 * @param output Pointer to the variable that will store the output from the RTC.
 * @return Return 0 upon success, non-zero otherwise.
 */
int rtc_output(uint8_t command, uint8_t *output);

/**
 * @brief Retrieves the current time from the RTC in binary format.
 *
 * @return The current time in binary format.
 */
int rtc_get_bin();

/**
 * @brief Checks if the RTC is updating.
 *
 * @return Return 0 if the RTC is not updating, non-zero otherwise.
 */
int rtc_get_uip();

/**
 * @brief Converts a binary-coded decimal (BCD) value to binary format.
 *
 * @param bcd The BCD value to be converted.
 * @return The converted value in binary format.
 */
uint16_t bcd_to_bin(uint16_t bcd);

/**
 * @brief Updates the current time stored in the RTC.
 *
 * @return Return 0 upon success, non-zero otherwise.
 */
int rtc_update_time();

#endif
