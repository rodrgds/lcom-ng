#ifndef _LCOM_RTC_H
#define _LCOM_RTC_H

#include <lcom/lcf.h>
#include "rtc_macros.h"

/**Enables interrupts from the RTC */
//int rtc_enable();

/**Disables interrupts from the RTC */
//int rtc_disable();

/**
 * @brief Subscribes interrupts to the RTC
 * 
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
//int rtc_subscribe_int(uint8_t * bit_no);

/**
 * @brief Unsubscribes RTC interrupts
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
//int rtc_unsubscribe_int();

/**
* @brief Reads date from the rtc only if rtc is not updating, also, if
* it is bcd converts to bcd
*/
realTime rtc_read_date();

/**
* @brief Reads from RTC_DATA_REG what is in port
* @param port One of the possible registers from rtc
*/
int rtc_read(uint32_t reg, uint32_t* data);

/**
* @brief Converts num to binary if and only if it is in bcd
* @param num number to be converted
*/
void bcd_to_binary(uint32_t * val);

/**
* @brief Checks if it is in bcd
*/
bool bcd();

/**
* @brief Checks if rtc is updating
*/
int isUpdating();
#endif // _LCOM_RTC_H
