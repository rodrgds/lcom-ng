#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>

/**
 * @file rtc.h
 * @brief Functions to interface with the PC's Real Time Clock
 * 
 * File containing the functions to subscribe and handle RTC's interrupts
*/

/** 
 * @defgroup RTC Macros
 * @{
 * 
 * Constants for programming the PC's Real Time Clock
 */

#define RTC_ADDR_REG            0x70 /**< @brief RTC Address Register */ 
#define RTC_DATA_REG            0x71 /**< @brief RTC Data Register */

#define RTC_SECONDS 		    0 /**< @brief Seconds Register Address Location */
#define RTC_SECONDS_ALARM 	    1 /**< @brief Seconds (Alarm) Register Address Location */
#define RTC_MINUTES			    2 /**< @brief Minutes Register Address Location */
#define RTC_MINUTES_ALARM	    3 /**< @brief Minutes (Alarm) Register Address Location */
#define RTC_HOURS 			    4 /**< @brief Hours Register Address Location */
#define RTC_HOURS_ALARM 	    5 /**< @brief Hours (Alarm) Register Address Location */
#define RTC_DAY_WEEK 		    6 /**< @brief Day of the week Register Address Location */
#define RTC_DAY_MONTH		    7 /**< @brief Day of the month Register Address Location */
#define RTC_MONTH			    8 /**< @brief Month Register Address Location */
#define RTC_YEAR			    9 /**< @brief Year Register Address Location */

#define RTC_CTRL_REG_A 			10 /**< @brief Control/Status Register A */
#define RTC_CTRL_REG_B  		11 /**< @brief Control/Status Register B */
#define RTC_CTRL_REG_C  		12 /**< @brief Control/Status Register C */
#define RTC_CTRL_REG_D  		13 /**< @brief Control/Status Register D */

#define RTC_IRQ                 8 /**< @brief RTC IRQ line*/

#define UIP                     0x80 /**< @brief Register A bit 7 (Set to 1 if update in progress) */
#define DM                      0x04 /**< @brief Register B bit 2 (Set to 1 to set time, alarm and date registers in binary. Set to 0, for BCD) */

/**@}*/

/**
 * @brief Subscribes RTC's interrupts.
 *  
 * Subscribes RTC's interrupt notifications using sys_irqsetpolicy().
 * 
 * @param bit_no pointer to the hook_id returned by the kernel in sys_irqsetpolicy().
 * @return 0 if the operation was successful, 1 otherwise
 */
int subscribe_rtc(uint8_t *bit_no);

/**
 * @brief Unsubscribes RTC's interrupts.
 * 
 * Unsubscribes RTC's interrupt notifications using sys_irqrmpolicy(). 
 * 
 * @return 0 if the operation was successful, 1 otherwise.
 */
int unsubscribe_rtc();

/**
 * @brief Creates a seed to generate random numbers
 * 
 * Reads the hours, minutes and seconds from the RTC and adds the three values to global variable rand_seed,
 * checking register A to see if there is an update in progress.
 */
void rtc_generate_seed();
#endif
