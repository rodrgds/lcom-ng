#ifndef __RTC_H
#define __RTC_H
#include "sprite.h"
/** @defgroup RTC RTC

 * @{ Functions used to calculate the game duration
 */

static int hook_id_rtc = 8;  //!< bit order for RTC to be used in interupt mask

#define RTC_ADDR_REG 0x70   //!< must be loaded with the address of the RTC register to be accessed
#define RTC_DATA_REG 0x71   //!< Used to transfer data to/from the RTC's register accessed

//#define RTC_IRQ 8

#define RTC_REG_A 10 
#define RTC_REG_B 11
#define RTC_REG_C 12
#define RTC_REG_D 13

// Bit-meaning in RTC_REG_A

#define RTC_UIP (1 << 7)  //!<  1 -> update in progress,dont acess time/date registers || 0 -> data valid for next 244 microsec
#define RTC_DV2 (1 << 6)  //!< Divider
#define RTC_DV1 (1 << 5)  //!< Divider
#define RTC_DV0 (1 << 4)  //!< Divider
#define RTC_RS3 (1 << 3)  //!< Rate selector
#define RTC_RS2 (1 << 2)  //!< Rate selector
#define RTC_RS1 (1 << 1)  //!< Rate selector
#define RTC_RS0 (1 << 0) //!< Rate selector

// Bit-meaning in RTC_REG_B

#define RTC_SET  (1 << 7)  //!< 1-> Stop updating || 0 -> update normally
#define RTC_PIE  (1 << 6)  //!< 1-Periodic interrupt enable
#define RTC_AIE  (1 << 5)  //!< 1-Alarm interrupt enable
#define RTC_UIE  (1 << 4)  //!< 1-Update-ended interrupt enable
#define RTC_SQWE (1 << 3)  //!< 1-Square Wave Enable
#define RTC_DM   (1 << 2)  //!< Data Mode, 1-Binary, 0-BCD
#define RTC_12_24 (1 << 1)  //!< 1-24 hours, 0-12 hours
#define RTC_DSE  (1 << 0)  //!< 1-Day-light saving

// Bit-meaning in RTC_REG_C

#define RTC_IRQF (1 << 7)  //!< 1-An interrupt occurred, if PF=PIE=1 || AF=AIE=1 || UF=UIE=1
#define RTC_PF   (1 << 6)  //!< 1-Periodic interrupt occurred
#define RTC_AF   (1 << 5)  //!< 1-Alarm interrupt occurred
#define RTC_UF   (1 << 4)  //!< 1-Update interrupt occurred

// Bit-meaning in RTC_REG_D

#define RTC_VRT (1 << 7)  //!< 1-Valid RAM and time

//Auxiliar Functions

/**
 * @brief Waits for a valid RTC interuption using valid interupt bit to be caught and stores it in regA (as defined in function)
 *
 * @return void
 */
void wait_valid_rtc(void);

/**
 * @brief Waits for a RTC interuption using update bit to be caught and stores it in regA (as defined in function)
 *
 * @return void
 */
void wait_update_end(void);

/**
 * @brief reads RTC register information and from rtc_reg
 * @param rtc_reg - register to be read from
 * @return RTC register value
 */
unsigned long read_rtc(unsigned long rtc_reg);

/**
 * @brief Writes in value to rtc_reg register
 * @param rtc_reg - register to be written in
 * @param value - value to be written in rtc_reg
 * @return RTC register that was written in
 */
unsigned long write_rtc(unsigned long rtc_reg,unsigned long value);

/**
 * @brief Enable RTC interuptions, so that RTC is able to be updated
 *
 * @return void
 */
void enable();

/**
 * @brief Disables RTC interuptions, so that RTC is not updated
 *
 * @return 1 upon sucess
 */
unsigned long disable();

/**
 * @brief Enable update type interuptions 
 *
 * @return void
 */
void enable_UIE();

/**
 * @brief Enables alarm type interuptions
 * @return void
 */
void enable_AIE();

/**
 * @brief Converts hexdecimal value into decimal number
 *
 * @return integer contain decimal value
 */
unsigned long BCD_to_DEC(unsigned long bcdByte);

/**
 * @brief Converts decimal value into hexdecimal number
 *
 * @return integer contain hexdecimal value
 */
unsigned long DEC_to_BCD(unsigned long decimalByte);
/**
 * @brief Subscribes and enables RTC interrupts
 *
 * @return bit order in interrupt mask; negative value on failure
 */
int RTC_subscribe();

/**
 * @brief Unsubscribes RTC interrupts
 *
 * @return 0 upon success and non-zero otherwise
 */
int RTC_unsubscribe();

/**
 * @brief defines a Sprite given a number
 * @param value to be converted to a Sprite
 * @return Sprite pointer upon success and non-zero otherwise
 */
Sprite * draw_symbol(unsigned int value);

/**
 * @brief draws in graphic mode in a pre-defined position the time
 * @param hours to be converted and printed in Sprite form
 * @param minutes to be converted and printed in Sprite form
 * @param seconds to be converted and printed in Sprite form
 * @return void
 */
void draw_time(unsigned int hours, unsigned int min, unsigned int sec);

/**
 * @brief Gets current time from RTC registers and converts into seconds
 * 
 * @return time in seconds
 */
unsigned int getTime(); 

/**
 * @brief Calculates time diference and calls draw_time function 
 * @param start - start time in seconds (time at game start)
 * @param end - end time in seconds (time at game end)
 * @return void
 */
void print_game_duration(int start, int end);


#endif /* __RTC_H */
