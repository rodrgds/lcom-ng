#ifndef _LCOM_DRIVERS_RTC_H_
#define _LCOM_DRIVERS_RTC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/**
 * @defgroup rtc RTC
 * @ingroup drivers
 * @{
*/

/** 
 * @defgroup irtc iRTC
 * @ingroup rtc
 * @{
 */

#define RTC_IRQ      					8
#define RTC_DELAY							20000
#define RTC_IS_UPDATING 			BIT(7)

#define RTC_DONT_CARE   			(BIT(7) | BIT(6))
#define RTC_REG_A							10
#define RTC_REG_A_UPDATE	  	BIT(7)
#define RTC_REG_B             11
#define RTC_REG_B_IS_BIN      BIT(2)
#define RTC_REG_B_ALARM       BIT(5)
#define RTC_REG_B_DONT_UPDATE BIT(7)
#define RTC_REG_C             12
#define RTC_REG_C_ALARM_FLAG  BIT(5)
#define RTC_REG_C_UPDATE_FLAG BIT(4)
#define RTC_INPUT_REG         0x70
#define RTC_OUTPUT_REG        0x71
#define RTC_REG_B_PERIODIC    BIT(6) 

#define RTC_H             4
#define RTC_H_ALARM       5
#define RTC_MIN           2
#define RTC_MIN_ALARM     3
#define RTC_S             0
#define RTC_S_ALARM       1
#define RTC_Y             9
#define RTC_M             8
#define RTC_D             7

/** @} */


/**
 * @brief Structure to store date and time.
 */
typedef struct {
    uint8_t hours;    /**< Hours */
    uint8_t minutes;  /**< Minutes */
    uint8_t seconds;  /**< Seconds */
    uint8_t year;     /**< Year */
    uint8_t month;    /**< Month */
    uint8_t day;      /**< Day */
} datetime_t;

/**
 * @brief Subscribes to RTC interrupts.
 *
 * @param bit_no Pointer where the bit number signaling the interruption will be saved
 * @return 0 if successful, 1 otherwise.
 */
int (rtc_subscribe_int)(uint8_t* bit_no);

/**
 * @brief Unsubscribes RTC interrupts.
 * 
 * @return 0 if sucessful, 1 otherwise.
*/
int (rtc_unsubscribe_int)();

/**
 * @brief Sets up RTC.
 * 
 * @return 0 if sucessful, 1 otherwise.
*/
int rtc_setup();

/**
 * @brief Converts a BCD value to binary.
 * 
 * @param inbcd BCD value to convert.
 * @return uint8_t Binary representation of the input BCD value.
 */

uint8_t rtc_to_bin(uint8_t inbcd);

/**
 * @brief Reads the current date and time from the RTC.
 * 
 * @return 0 if sucessful, 1 otherwise.
 */
int rtc_get_time();

/**
 * @brief Sets the RTC alarm.
 */
int rtc_set_alarm();

/**
 * @brief Reads a value from an RTC register.
 * 
 * @param cmd Command to specify the register to read.
 * @param output Pointer to store the read value.
 * @return 0 if sucessful, 1 otherwise.
 */
int rtc_read(uint8_t cmd, uint8_t *output);

/**
 * @brief Writes a value to an RTC register.
 * 
 * @param cmd Command to specify the register to write.
 * @param content Value to write to the register.
 * @return 0 if sucessful, 1 otherwise.
 */
int rtc_write(uint8_t cmd, uint8_t content);

/**
 * @brief Waits for the RTC to be ready.
 */
void rtc_wait();

/**
 * @brief RTC interrupt handler.
 * 
 * @return 0 if sucessful, 1 otherwise.
 */
int (rtc_ih)();

/**
 * @brief Sets the periodic interrupt of the RTC.
 * 
 * @param enable Boolean to enable or disable the periodic interrupt.
 * @return 0 if sucessful, 1 otherwise.
 */
int rtc_set_periodic_int(bool enable);

/**
 * @brief Sets the update interrupt of the RTC.
 * 
 * @param enable Boolean to enable or disable the update interrupt.
 * @return 0 if sucessful, 1 otherwise.
 */
int rtc_set_update_int(bool enable);

/**
 * @brief Sets the alarm interrupt of the RTC.
 * 
 * @param enable Boolean to enable or disable the alarm interrupt.
 * @return 0 if sucessful, 1 otherwise.
 */
int rtc_set_alarm_int(bool enable);

/** @} */

#endif
