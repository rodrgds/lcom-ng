/**
 * @file font8x8_basic.h
 * @brief 8x8 monochrome bitmap font for ASCII characters 0-127.
 *
 * Provides a font array for rendering text in graphics mode.
 */

#ifndef _LCOM_FONT8X8_BASIC_H_
#define _LCOM_FONT8X8_BASIC_H_

#include <stdint.h>

/**
 * @brief 8x8 monochrome font for the first 128 ASCII characters.
 *
 * This array contains 128 entries, one for each ASCII character (from 0 to 127).
 * Each character is represented by 8 bytes, with each byte encoding one horizontal row
 * of the character's bitmap. The LSB represents the leftmost pixel.
 *
 */
extern uint8_t font8x8_basic[128][8];

#endif /* _LCOM_FONT8X8_BASIC_H_ */
