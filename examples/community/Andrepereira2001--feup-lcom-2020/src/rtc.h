#pragma once
#include <lcom/lcf.h>

/** @defgroup rtcVars
 * @{
 *
 * Constants for programming the real time clock. 
 */

/**
 * @brief used to save the date 
 */
typedef struct {
    /** @brief  date seconds*/
    int seconds;
    /** @brief  date minutes*/
    int minutes;
    /** @brief  date hours*/
    int hours;
    /** @brief  date day*/
    int month_day;
    /** @brief  date month*/
    int month;
    /** @brief  date year*/
    int year;
} Date_t;

/**
 * @brief read date from the rtc registers
 * 
 * @param date  date that will be readed
 * @return returns 0 upon sucess
 */
int read_date(Date_t * date);

/**
 * @brief convert given bcd number to decimal number
 * 
 * @param bcd  number in bcd
 * @return number in decimal
 */
int convert_to_decimal(uint8_t bcd);

/**
 * @brief check if is valid read date
 * 
 * @return true if it is a valid read and false if it is not
 */
bool valid_read_date();

/**
 * @brief subscribes rtc interrupts
 * 
 * @param bit_no irq line of rtc
 * @return int 0 upon success
 */
int rtc_subscribe_int(uint8_t *bit_no);

/**
 * @brief unscribes rtc interrupts
 * 
 * @return int 0 upon sucess
 */
int rtc_unsubscribe_int();

/**
 * @brief enables periodic interrupts from rtc
 * 
 * @return int  0 upon sucess
 */
int enable_periodic_interrupts();

/**
 * @brief disables periodic interrupts from rtc
 * 
 * @return int  0 upon sucess
 */
int disable_periodic_interrupts();

/**
 * @brief reads register C from rtc
 * 
 * @param regC info readed
 * @return int 0 upon sucess
 */
int read_registerC(uint8_t * regC);
