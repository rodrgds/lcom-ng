/**
 * This module contains all functions to manage Real Time Clock(RTC)
 * Jorge Vale: 50%
 * Telmo Barros: 50%
 * Relative Weight: 4%
 */
#ifndef __RTC_H
#define __RTC_H

/** @defgroup RTC RTC
 * @{
 * Functions to manage Real Time Clock(RTC)
 */

#define RTC_ADDR_REG 		0x70
#define RTC_DATA_REG 		0x71

#define RTC_SECONDS 		0
#define RTC_SECONDS_ALARM 	1
#define RTC_MINUTES			2
#define RTC_MINUTES_ALARM	3
#define RTC_HOURS 			4
#define RTC_HOURS_ALARM 	5
#define RTC_DAY_WEEK 		6
#define RTC_DAY_MONTH		7
#define RTC_MONTH			8
#define RTC_YEAR			9

#define RTC_REG_A 			10
#define RTC_REG_B  			11
#define RTC_REG_C  			12
#define RTC_REG_D  			13

#define HIGH_REG_1			14
#define HIGH_REG_2			15
#define HIGH_REG_3			16

#define RTC_IRQ				8

#define HIGHSCORES_MULTIPLE 10

/**
 * @brief Subscribes the rtc interrupts
 *
 * @return Return rtc ID to verify the interrupts and -1 upon failure
 */
int rtc_subscribe_int();

/**
 * @brief Unsubscribes the rtc interrupts
 *
 * @return Return 0 upon success and 1 upon failure
 */
int rtc_unsubscribe_int();

/**
 * @brief Gets the current time from RTC registers
 *
 * @param hour Variable to change to the value of hours
 * @param min Variable to change to the value of minutes
 * @param sec Variable to change to the value of seconds
 *
 * @return Return 0 upon success
 */
int rtc_current_time(unsigned long *hour, unsigned long *min, unsigned long *sec);

/**
 * @brief Draws the current time in the video buffer, in hh:mm:ss model
 *
 * @param x X coordinate to begin the drawing
 * @param y Y coordinate to begin the drawing
 */
void rtc_draw_current_time(unsigned int x, unsigned int y);

/**
 * @brief Gets the current values of highscores from the first three registers that we are able to use
 *
 * @param highscores[3] Array of highscores values to change to the values of registers
 */
void rtc_get_highscores(unsigned int highscores[3]);

/**
 * @brief Sets the first three registers that we are able to use TO the current values of highscores (update highscores)
 *
 * @param highscores[3] Array of highscores value to change the value of registers
 */
void rtc_set_highscores(unsigned int highscores[3]);

/**@}*/

#endif /* __RTC_H */
