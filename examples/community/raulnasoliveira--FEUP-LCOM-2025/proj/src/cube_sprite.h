#ifndef CUBE_SPRITE_H
#define CUBE_SPRITE_H

#include <stdint.h>
#include "Sprite.h"

/**
 * @file cube_sprite.h
 * @brief  Functions for creating, updating, and drawing rotating cube and letter sprites.
 */

/**
 * @brief   Creates a new cube sprite.
 * @param   x               Initial X coordinate of the cube’s center.
 * @param   y               Initial Y coordinate of the cube’s center.
 * @param   xspeed          Horizontal speed in pixels per frame.
 * @param   yspeed          Vertical speed in pixels per frame.
 * @param   rotation_speed  Rotation speed in radians per frame.
 * @return  Pointer to an allocated Sprite object, or NULL on failure.
 */
Sprite *create_cube_sprite(int x, int y, int xspeed, int yspeed, double rotation_speed);

/**
 * @brief   Advances the cube sprite’s state by one frame.
 *          Updates position based on speed and applies rotation.
 * @param   sp  Pointer to the cube Sprite to update.
 */
void update_cube_sprite(Sprite *sp);

/**
 * @brief   Draws the cube sprite into the specified buffer.
 * @param   sp      Pointer to the cube Sprite to draw.
 * @param   buffer  Target pixel buffer (e.g., double_buffer).
 */
void draw_cube_sprite(Sprite *sp, uint8_t *buffer);

/**
 * @brief   Frees all memory associated with a sprite.
 * @param   sp  Pointer to the Sprite to destroy.
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief   Draws a single XPM-based letter image into the buffer.
 * @param   xpm     XPM map for the letter.
 * @param   x       Top-left X coordinate.
 * @param   y       Top-left Y coordinate.
 * @param   buffer  Target pixel buffer.
 */
void draw_xpm_letter(const char **xpm, int x, int y, uint8_t *buffer);

/**
 * @brief   Creates a sprite for a single letter from its XPM map.
 * @param   letter_xpm  XPM map for the letter.
 * @param   x           Initial X coordinate.
 * @param   y           Initial Y coordinate.
 * @return  Pointer to an allocated Sprite object for the letter, or NULL on failure.
 */
Sprite *create_letter_sprite(const char **letter_xpm, int x, int y);

#endif // CUBE_SPRITE_H


