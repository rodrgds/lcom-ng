/**
 * @file utils
 * @brief This file has the implementation of the functions related to the uitls for drivers.
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <lcom/lcf.h>

/**
 * @brief Reads a byte from the specified port.
 * 
 * @param port The port to read from.
 * @param value A pointer to the variable that will hold the read byte.
 *
 * This function reads a byte from the specified port and stores it in the 
 * provided pointer value. It uses the sys_inb system call to read the byte 
 * and then type casts the result to a uint8_t.
 * 
 * @return Returns 0 upon success, and 1 upon failure.
 */
int (util_sys_inb)(int port, uint8_t* value);

/**
 * @brief Extracts the least significant byte from a word.
 * 
 * @param val The word to extract the LSB from.
 * @param lsb A pointer to the variable that will hold the extracted LSB.
 *
 * This function extracts the least significant byte (LSB) from a word (val) 
 * and stores it in the provided pointer (lsb). It returns an error if the 
 * pointer is null.
 * 
 * @return Returns 0 upon success, and 1 upon failure.
 */
int (util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Extracts the most significant byte from a word.
 * 
 * @param val The word to extract the MSB from.
 * @param msb A pointer to the variable that will hold the extracted MSB.
 *
 * This function extracts the most significant byte (MSB) from a word (val) 
 * and stores it in the provided pointer (msb). It returns an error if the 
 * pointer is null.
 * 
 * @return Returns 0 upon success, and 1 upon failure.
 */
int (util_get_MSB)(uint16_t val, uint8_t *msb);

#endif
