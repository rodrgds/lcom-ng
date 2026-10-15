/**
 * @file utils.c
 * @brief Utility functions for byte manipulation and port I/O.
 *
 * This file provides helper functions for extracting LSB/MSB from 16-bit values
 * and for reading 8-bit values from hardware ports as implemented in the labs.
 */

#include <lcom/lcf.h>
#include <stdint.h>
#include "utils.h"

/**
 * @brief Gets the least significant byte (LSB) of a 16-bit value.
 *
 * @param val 16-bit value.
 * @param lsb Pointer to store the LSB.
 * @return 0 on success, 1 on failure (if lsb is NULL).
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (!lsb) return 1;
  *lsb = (uint8_t) (val & 0xFF);
  return 0;
}

/**
 * @brief Gets the most significant byte (MSB) of a 16-bit value.
 *
 * @param val 16-bit value.
 * @param msb Pointer to store the MSB.
 * @return 0 on success, 1 on failure (if msb is NULL).
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (!msb) return 1;
  *msb = (uint8_t) ((val >> 8) & 0xFF);
  return 0;
}

/**
 * @brief Reads an 8-bit value from the specified port.
 *
 * Uses sys_inb to read a 32-bit value and stores only the least significant byte.
 *
 * @param port I/O port to read from.
 * @param value Pointer to store the 8-bit result.
 * @return 0 on success, 1 on failure.
 */
int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t temp;
    
    if (sys_inb(port, &temp) != OK) {
        return 1; 
    }
  
    *value = (uint8_t) temp; 
    return 0;
}
