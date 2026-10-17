#ifndef _RTC_H_
#define _RTC_H_

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


/** @defgroup RTC RTC
 * @{
 *
 * Functions and constant of the RTC
 */

/*MACROS*/
#define RTC_IRQ         8               /**< @brief RTC IRQ line*/

#define RTC_ADDR_REG    0x70            /**< @brief RTC Address register */
#define RTC_DATA_REG    0x71            /**< @brief RTC Data register*/

#define RTC_REG_A       10              /**< @brief RTC Register A*/
#define RTC_REG_B       11              /**< @brief RTC Register B*/
#define RTC_REG_C       12              /**< @brief RTC Register C*/
#define RTC_REG_D       13              /**< @brief RTC Register D*/

#define RTC_UF          BIT(4)          /**< @brief Update interrupts source*/
#define RTC_AF          BIT(5)          /**< @brief Alarm interrupts source*/
#define RTC_PF          BIT(6)          /**< @brief Periodic interrupts source*/

#define RTC_UIE         BIT(4)          /**< @brief Update interrupts subscripton*/
#define RTC_AIE         BIT(5)          /**< @brief Alarm interrupts subscription*/
#define RTC_PIE         BIT(6)          /**< @brief Periodic interrupts subscripton*/

//Rate Selectors
#define RTC_RS0         BIT(0)          /**< @brief Rate Selector 0*/
#define RTC_RS1         BIT(1)          /**< @brief Rate Selector 1*/
#define RTC_RS2         BIT(2)          /**< @brief Rate Selector 2*/
#define RTC_RS3         BIT(3)          /**< @brief Rate Selector 3*/


/*Functions*/

/**
 * @brief Handles the periodic interrupts of the rtc
 * 
 */
void rtc_ih();

/**
 * @brief Subscribes the periodic interrupts of the RTC
 * 
 * @param bit_no        Bit number of the IRQ line
 * @return              1 on failure
 */
int (rtc_subscribe_int)(int *bit_no);

/**
 * @brief Unsubscribes the RTC periodic interruptions
 * 
 * @return              1 on failure
 */
int (rtc_unsubscribe_int)();

/**
 * @brief Writes data data to the reg Register of the kbc
 * 
 * @param reg           Register to write the data to
 * @param data          Data to write
 * @return              1 on failure  
 */
int rtc_write_data(int reg, uint8_t data);

/**
 * @brief Reads data from the reg Register of the kbc
 * 
 * @param reg           Register to read the data from
 * @param data          Return value
 * @return              1 on failure  
 */
int rtc_read_data(int reg,uint8_t * data);

/**
 * @brief Test function for the RTC
 * 
 * @return              1 on failure
 */
int test_rtc();

/**@}*/

#endif
