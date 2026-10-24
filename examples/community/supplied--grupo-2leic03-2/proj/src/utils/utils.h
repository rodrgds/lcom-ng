/**
 * @file utils.h
 * @brief Utility functions.
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

/**
 * @brief Invokes sys_inb and reads an 8-bit value.
 * @param port The port to read from.
 * @param value Pointer to store the read value.
 * @return 0 upon success, non-zero otherwise.
 */
int util_sys_inb(int port, uint8_t *value);

/**
 * @brief Clamps an integer value within a specific range.
 * @param value The value to clamp.
 * @param min The minimum allowed value.
 * @param max The maximum allowed value.
 * @return The clamped value.
 */
int clamp_int(int value, int min, int max);

#endif
