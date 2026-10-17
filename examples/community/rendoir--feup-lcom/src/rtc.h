/** @file rtc.h
    @author Daniel Marques
    @author João Almeida
    Prototype of functions used to read information from the RTC
*/

#ifndef RTC_H
#define RTC_H

/** @defgroup RTC RTC
 * @{
 *
 * Functions responsible for communicating with the RTC
 */

//TOOLS
#define BIT(n) (0x1<<(n))

//HARDWARE
#define RTC_IRQ 8
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

//REG ADDR
#define SECONDS 0x00
#define SECONDS_ALARM 0x01
#define MINUTES 0x02
#define MINUTES_ALARM 0x03
#define HOURS 0x04
#define HOURS_ALARM 0x05
#define DAY_OF_THE_WEEK 0x06
#define DAY_OF_THE_MONTH 0x07
#define MONTH 0x08
#define YEAR 0x09
#define REG_A 0x0A
#define REG_B 0x0B
#define REG_C 0x0C
#define REG_D 0x0D

//BITS
#define UIP BIT(7)
#define DM BIT(2)


//FUNCTIONS

/**
  @brief Subscribes the RTC Interrupts
  @param rtc_id To be ID of the RTC Interrupts
*/
void RTC_Subscribe(int rtc_id);

/**
  @brief Unsubscribes the RTC Interrupts
  @param rtc_id ID of the RTC Interrupts
*/
void RTC_Unsubscribe(int rtc_id);

/**
  @brief Enables interrupts from the RTC
*/
void RTC_EnableInt();

/**
  @brief Disables interrupts from the RTC
*/
void RTC_DisableInt();

/**
 	 @brief Sets the RTC mode to binary
 */
void RTC_SetBinary();

/**
  @brief Gets the time from the RTC
  @param hour To be hour received from the RTC
  @param minute To be minutes received from the RTC
  @param second To be seconds received from the RTC
*/
void RTC_GetTime(unsigned long int *hour, unsigned long int *minute, unsigned long int *second);

/**
  @brief Gets the time from the RTC
  @param day To be day received from the RTC
  @param month To be month received from the RTC
  @param year To be year received from the RTC
*/
void RTC_GetDate(unsigned long int *year, unsigned long int *month,unsigned long  int *day);


#endif
