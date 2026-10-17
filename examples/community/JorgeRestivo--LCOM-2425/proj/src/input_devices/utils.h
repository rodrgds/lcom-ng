/**
 * @file utils.h
 * @brief Utility functions for LCOM input devices.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <lcom/lcf.h>

/**
 * @brief Gets the least significant byte of a 16-bit value.
 * @param val 16-bit value.
 * @param lsb Pointer to store the least significant byte.
 * @return 0 on success, non-zero otherwise.
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Gets the most significant byte of a 16-bit value.
 * @param val 16-bit value.
 * @param msb Pointer to store the most significant byte.
 * @return 0 on success, non-zero otherwise.
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Reads a byte from the specified port.
 * @param port Port to read from.
 * @param value Pointer to store the read byte.
 * @return 0 on success, non-zero otherwise.
 */
int (util_sys_inb)(int port, uint8_t *value);

#endif
