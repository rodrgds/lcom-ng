#ifndef _SPRITE_VIEW_H_
#define _SPRITE_VIEW_H_

#include "../model/sprite.h"

/** @defgroup sprite_view sprite_view
 * @{
 *
 * Functions related to the sprite view
 */

/**
 * @brief Sprite drawer
 *
 * @param sprite pointer to the sprite to be drawn
 * @param x x-coordinate where to draw the sprite
 * @param y y-coordinate where to draw the sprite
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_sprite(Sprite *sprite, uint16_t x, uint16_t y);

/**
 * @brief Times symbol (cross - x) drawer
 *
 * @param x x-coordinate where to draw the times symbol
 * @param y y-coordinate where to draw the times symbol
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_times(uint16_t, uint16_t y);

/**
 * @brief Digit [0-9] drawer
 *
 * @param digit digit to be drawn
 * @param x x-coordinate where to draw the digt
 * @param y y-coordinate where to draw the digit
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_digit(uint8_t digit, uint16_t x, uint16_t y);

/**
 * @brief Number drawer
 *
 * @param number number to be drawn
 * @param x x-coordinate where to draw the number
 * @param y y-coordinate where to draw the number
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_number(uint32_t number, uint16_t x, uint16_t y);

/**@}*/

#endif /* _SPRITE_VIEW_H_ */
