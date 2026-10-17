/**
 * @file font8x8.h
 * @brief 8x8 bitmap font for ASCII characters 32–90 (space through Z).
 */

#ifndef FONT8X8_H
#define FONT8X8_H

#include <stdint.h>

/**
 * @brief 8x8 bitmap font data for ASCII characters 32–90.
 *
 * Each character is represented by 8 bytes, one for each row.
 * Only ASCII codes 32 (space) through 90 (Z) are supported.
 */
extern const uint8_t font8x8[91][8];

#endif
