#ifndef _RTC_H_
#define _RTC_H_

/** @defgroup rtc rtc
 * @{
 *
 * Functions to manage rtc abstraction (get curret time)
 */

#define BIT(n) (0x01<<(n))
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71


#define RTC_REG_A 10
#define RTC_REG_B 11
#define RTC_HOURS 4
#define RTC_MINUTES 2
#define RTC_SECONDS 0

#define REG_A_UIP BIT(7)


/**
 * @brief Get current hours from rtc
 *
 * @param hours - pointer to place where the result will be stored
 * @return 0
 */
int rtc_getHours(unsigned long *hours);
/**
 * @brief Get current minutes from rtc
 *
 * @param minutes - pointer to place where the result will be stored
 * @return 0
 */
int rtc_getMinutes(unsigned long *minutes);

/**
 * @brief Get current seconds from rtc
 *
 * @param seconds - pointer to place where the result will be stored
 * @return 0
 */
int rtc_getSeconds(unsigned long *seconds);

/**
 * @brief check if rtc is available to read
 *
 * @return 1 if rtc is available, 0 otherwise
 */
int rtc_available();

/**
 * @brief convert BCD value to binary
 *
 * @param data - BCD values
 * @return binary value
 */
unsigned long convertBCDtoBinary(unsigned long data);

#endif

/** @} end of rtc*/
