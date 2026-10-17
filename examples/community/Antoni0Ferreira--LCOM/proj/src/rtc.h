#ifndef __RTC_H
#define __RTC_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @defgroup rtc rtc
 * @{
 * 
 * Module for the rtc
 */

#define RTC_ADDR_REG 0x70 // Register Address of the RTC
#define RTC_DATA_REG 0x71 // Data Address of the RTC

#define RTC_REGISTER_B 0x0B
#define RTC_REGISTER_C 0x0C
#define RTC_REGISTER_MINS 0x02
#define RTC_REGISTER_SECONDS 0x00

/**
 * @brief Subscribes rtc interrupts
 * 
 * @param bit_no bit where interrupts will be signaled
 * @return Return 0 upon success and non-zero otherwise 
 */
int rtc_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes rtc interrupts
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int rtc_unsubscribe_int();

/**
 * @brief Interrupt handler of the RTC
 * 
 */
void rtc_ih();

/**
 * @brief Reads the minutes from the RTC
 * 
 * @param minutes Minutes 
 * @return Return 0 upon success and non-zero otherwise 
 */
int get_mins(uint8_t *minutes);

/**
 * @brief Reads the seconds from the RTC
 * 
 * @param seconds Seconds
 * @return Return 0 upon success and non-zero otherwise 
 */
int get_secs(uint8_t *seconds);

/**
 * @}
 */

#endif /* __RTC_H */
