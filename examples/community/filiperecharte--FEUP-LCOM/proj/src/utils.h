#pragma once
#include <lcom/lcf.h>

#include <stdint.h>
#include "rtc.h"
/** @defgroup utils utils
 * @{
 *
 * LCOM's utility functions
 */

#include <stdint.h>

#ifndef BIT
#  define BIT(n) (1 << (n))
#endif

 /**
  * @brief Returns the LSB of a 2 byte integer
  *
  * @param val input 2 byte integer
  * @param lsb address of memory location to be updated with val's LSB
  * @return Return 0 upon success and non-zero otherwise
  */
int (util_get_LSB)(uint16_t val, uint8_t* lsb);

/**
 * @brief Returns the MSB of a 2 byte integer
 *
 * @param val input 2 byte integer
 * @param msb address of memory location to be updated with val's LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_get_MSB)(uint16_t val, uint8_t* msb);

/**
 * @brief Invokes sys_inb() system call but reads the value into a uint8_t variable.
 *
 * @param port the input port that is to be read
 * @param value address of 8-bit variable to be update with the value read
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_sys_inb)(int port, uint8_t* value);
/**
 * @brief Invokes sys_inb() system call but reads the value into a uint8_t variable and increments a counter.
 *
 * @param port the input port that is to be read
 * @param value address of 8-bit variable to be update with the value read
 * @return Return 0 upon success and non-zero otherwise
 */
int util_sys_inb_counter(int port, uint32_t* value);
/**
* @brief Converts BCD to Decimal
*
* Converts a given bcd number to decimal representation
*
* @param n - value to increment
* @return Returns nothing
*/
int bcd_to_decimal(uint8_t n);
/**
* @brief Gets date
*
* Retrieves a the date to the given string
*
* @param date - string to be written with date
* @return Returns nothing
*/
void get_date(char *date);
