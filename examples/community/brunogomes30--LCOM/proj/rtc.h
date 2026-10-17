#ifndef RTC_H
#define RTC_H
#include <stdint.h>
#include <lcom/lcf.h>

/** @defgroup rtc 
 * @{
 *
 * Functions related to RTC 
 */

#define RTC_IRQ 8

#define RTC_SECONDS 0
#define RTC_SECONDS_ALARM 1
#define RTC_MINUTES 2
#define RTC_MINUTES_ALARM 3
#define RTC_HOURS 4
#define RTC_HOURS_ALARM 5
#define RTC_DAY_OF_WEEK 6
#define RTC_DAY_OF_MONTH 7
#define RTC_MONTH 8
#define RTC_YEAR 9
#define RTC_REGISTER_A 10
#define RTC_REGISTER_B 11
#define RTC_REGISTER_C 12
#define RTC_REGISTER_D 13

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

typedef struct{
    int day, month, year, hours, minutes, seconds;
}Date;

/**
 * @brief Subscribe to rtc's interrupts
 * 
 * @return int 
 */
int rtc_subscribe_int();

/**
 * @brief Unsubscribe rtc's interrupts
 * 
 * @return int 
 */
int rtc_unsubscribe_int();

/**
 * @brief Wait 
 * 
 */
void rtc_delay();

/**
 * @brief Active RTC periodic interrupts at te sample rate given
 * 
 * @param rate 
 * @return int 
 */
int rtc_act_periodic_interrupts(uint8_t rate);

/**
 * @brief Disable RTC periodic interrupts
 * 
 * @return int 
 */
int rtc_disable_periodic();

/**
 * @brief Add alarm
 * 
 * @return int 
 */
int rtc_create_alarm();

/**
 * @brief Write to RTC register
 * 
 * @param reg 
 * @param data 
 * @return int 
 */
int rtc_write(uint32_t reg, uint32_t data);

/**
 * @brief RTC interrupt handler
 * 
 * @return uint8_t 
 */
uint8_t rtc_ih();

/**
 * @brief Read from RTC register
 * 
 * @param reg 
 * @return uint32_t 
 */
uint32_t rtc_read(uint32_t reg);
/**
 * @brief Get current date
 * 
 * @return Date 
 */
Date getDate();


#endif
