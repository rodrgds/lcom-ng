#ifndef _RTC_H
#define _RTC_H
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <minix/com.h>

/** @defgroup rtc rtc
 * @{
 *
 * Functions that handle the rtc part of the project
 */

/**
 * @brief Struct that keeps information about the current date
 */
typedef struct {
    unsigned long day;
    unsigned long month;
    unsigned long year;
    unsigned long seconds;
    unsigned long minutes;
    unsigned long hours;
} Date;

#define BIT(n) (0x01<<(n)) /**< @brief put the chosen bit as 1 */

#define RTC_IRQ 8 /** < @brief Real time clock IRQ */

#define RTC_ADDR_REG 0x70 /** < @brief RTC internal address register*/
#define RTC_RW_REG 0x71 /** < @brief RTC internal data register*/


#define SEC 0 /** < @brief seconds*/
#define SECA 1 /** < @brief alarm seconds*/
#define MINU 2 /** < @brief minutes*/
#define MINUA 3 /** < @brief alarm minutes*/
#define HOUR 4 /** < @brief hour */
#define HOURA 5 /** < @brief alarm hour*/
#define DOW 6 /** < @brief day of the week*/
#define DAY 7 /** < @brief day of the month*/
#define MONTH 8 /** < @brief month*/
#define YEAR 9 /** < @brief year*/
#define REGA 10 /** < @brief Register A*/
#define REGB 11 /** < @brief Register B*/
#define REGC 12 /** < @brief Register C*/
#define REGD 13 /** < @brief Register D*/

#define PIE 6 /** < @brief periodic interrupt*/
#define AIE 5 /** < @brief alarm interrupt*/
#define UIE 4 /** < @brief update interrupt*/

/**
 * @brief Converts a decimal number to it's binary equivalent
 * @param bcd is the number to be converted
 * @return number converted to binary
 */
static unsigned char bcd_to_bin(unsigned char bcd);
/**
 * @brief Disables interrupts
 */
void rtc_disable_int();
/**
 * @brief Enables interrupts
 */
void rtc_enable_int();
/**
 * @brief Gets the current date and hours from the RTC and returns it
 *
 * Disables interrupts, writes to the RTC the instructions needed to get the current date and hours, and returns them
 *
 * @return struct Date with the current date and hour
 */
Date *getDateOnly();
/**
 * @brief Gets the current date from the RTC and shows the date
 *
 * Disables interrupts, writes to the RTC the instructions needed to get the current date, and call the function
 * to show it
 *
 * @see show_date
 *
 * @return 0  if success, 1 if not success
 */
int getDate();
/**
 * @brief Gets the current date and hours from the RTC and shows them
 *
 * Disables interrupts, writes to the RTC the instructions needed to get the current date and hours, and call the function
 * to show them
 *
 * @see show_date_and_hours
 *
 * @return 0  if success, 1 if not success
 */
int getDateandHours();
/**
 * @brief Sets the alarm for the next two seconds
 *
 * Gets the current hour and adds 2 more seconds to it, making it the hour for the alarm
 */
void rtc_set_alarm();
/**
 * @brief Reads the interruption from the RTC
 * @return 0 if success
 */
int rtc_handler();

#endif
