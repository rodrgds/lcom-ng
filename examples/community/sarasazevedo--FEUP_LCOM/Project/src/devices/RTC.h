/**
 * @file RTC.h
 * 
 * @brief Data structures and functions for using the Real Time Clock
*/

#ifndef _RTC_H_
#define _RTC_H_

#include <lcom/lcf.h>

/* Ports */
#define RTC_ADDR_REG_PORT 0x70
#define RTC_DATA_REG_PORT 0x71
#define RTC_IRQ_LINE 8

/* One-byte registers */
#define SECONDS 0   //0-59
#define MINUTES 2   //0-59
#define HOURS 4     //0-24
#define WEEKDAY 6   //1-7, Sunday=1
#define MONTHDAY 7  //1-31
#define MONTH 8     //1-12
#define YEAR 9      //0-99

#define SECONDS_ALARM 1
#define MINUTES_ALARM 3
#define HOURS_ALARM 5

#define REGISTER_A 10
#define REGISTER_B 11
#define REGISTER_C 12
#define REGISTER_D 13

/** @defgroup RTC RTC
 * @{
 *
 * @brief  RTC related functions
 * 
 */

/**
 * @brief Struct to hold the seconds, minutes and hours of the current time of the day
*/
typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
} timeType;


/**
 * @brief Struct to hold the weekday, monthday, month and year of the current day date
*/
typedef struct {
    uint8_t weekday;
    uint8_t monthday;
    uint8_t month;
    uint8_t year;
} dateType;



/**
 * @brief Reads a byte from a RTC register, after informing the address of the desidered register
 * 
 * @param address The address of the register we want to read
 * @param byte The read byte
 * @return Return 0 upon success and non-zero otherwise
*/
int rtc_read_byte(uint8_t address, uint8_t *byte);


/**
 * @brief Update the current day date by reading RTC registers
 * @return Return 0 upon success and non-zero otherwise
*/
int rtc_update_date();


/**
 * @brief Update the current day time by reading RTC registers
 * @return Return 0 upon success and non-zero otherwise
*/
int rtc_update_time();


/**
 * @brief Converts a byte from BCD form into binary. If it is already in binary, doesn't change it
 * 
 * @param byte The byte to be converted
 * @param binary The binary form of the byte
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int get_binary(uint8_t byte, uint8_t *binary);


/**
 * @brief Subscribes RTC interrupts
 *
 * @param irq_set_mask  Mask with the rtc_hook_id bit set
 * @return Return 0 upon success and non-zero otherwise
 */
int rtc_subscribe_int(uint8_t *irq_set_mask);

/**
 * @brief Unsubscribes RTC interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int rtc_unsubscribe_int();


/**
 * @brief Updates the current date and time of the day upon receiving a RTC interrupt
*/
int rtc_ih();

/** @} end of RTC */


#endif /* _RTC_H_ */
