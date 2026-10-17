#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>


#define RTC_IRQ 8
#define RTC_DELAY 20


#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_MIN_YEAR 2000
#define RTC_DONT_CARE_VALUE BIT(7) | BIT(6)

/* Registers reserved for time-related functionality */
#define RTC_SECONDS 0
#define RTC_SECONDS_ALARM 1
#define RTC_MINUTES 2
#define RTC_MINUTES_ALARM 3
#define RTC_HOUR 4
#define RTC_HOUR_ALARM 5
#define RTC_DAY_OF_WEEK 6
#define RTC_DAY_OF_MONTH 7
#define RTC_MONTH 8
#define RTC_YEAR 9

/* Registers reserved for control of the RTC */
#define RTC_A 10
#define RTC_B 11
#define RTC_C 12
#define RTC_D 13

/* Register A control */
#define RTC_REG_A_UIP BIT(7)
#define RTC_REG_A_RS3 BIT(3) // periodic int rate selector bit 2
#define RTC_REG_A_RS2 BIT(2) // periodic int rate selector bit 1
#define RTC_REG_A_RS1 BIT(1) // periodic int rate selector bit 0

/* Register B control */
#define RTC_REG_B_SET BIT(7)
#define RTC_REG_B_PIE BIT(6)
#define RTC_REG_B_AIE BIT(5)
#define RTC_REG_B_UIE BIT(4)
#define RTC_REG_B_SQWE BIT(3)
#define RTC_REG_B_DM BIT(2)

/* Register C control */
#define RTC_REG_C_IRQF BIT(7)
#define RTC_REG_C_PF BIT(6)
#define RTC_REG_C_AF BIT(5)
#define RTC_REG_C_UF BIT(4)

/* Register D control */
#define RTC_REG_D_VRT BIT(7)

/**
 * @brief Returns the current ongoing game time
 * @return int The current game time
*/
int (get_game_time)();

/**
 * @brief Sets the current ongoing game time
 * @param time The time to be set
*/
void (set_game_time)(int time);

/**
 * @brief Returns the current round time
 * @return int The current round time
*/
int (get_round_time)();

/**
 * @brief Sets the current round time
 * @param time The time to be set
*/
void (set_round_time)(int time);

/**
 * @brief Returns the current time
 * @return char* The current time
*/
char* (get_current_time)();

/**
 * @brief Subscribes and enables RTC interrupts
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return int Return 0 upon success and non-zero otherwise
*/
int(rtc_subscribe)(uint8_t *bit_no);

/**
 * @brief Unsubscribes RTC interrupts
 * 
 * @return int Return 0 upon success and non-zero otherwise
*/
int(rtc_unsubscribe)();

/**
 * @brief RTC interrupt handler.
 *
 * This function is the interrupt handler for the Real-Time Clock (RTC).
 * It reads the status from the RTC's register C and handles the appropriate 
 * interrupt based on the flags set in the register.
 *
 * The function handles two types of RTC interrupts:
 * 1. Alarm interrupts (RTC_REG_C_AF): When an alarm occurs, it calls the 
 *    rtc_alarm_ih() function.
 * 2. Update interrupts (RTC_REG_C_UF): When the RTC updates its current time, 
 *    it calls the rtc_current_time_ih() function.
 */
void(rtc_ih)();



#endif

