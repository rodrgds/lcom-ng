#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include "controller/video/graphics.h"

/** @defgroup sprite sprite
 * @{
 *
 * Functions for using sprites
 */


/**
 * @brief Structure representing a sprite.
 *
 * The Sprite structure represents a sprite, which is a graphical object with a specific height, width, and color properties.
 * It contains the following members:
 * - height: Height of the sprite in pixels.
 * - width: Width of the sprite in pixels.
 * - colors: Pointer to an array of color values representing the pixels of the sprite.
 * - color: Color value representing the default color of the sprite.
 * - pressed: Flag indicating whether the sprite is pressed or not.
 */

typedef struct {
    uint16_t height;
    uint16_t width;
    uint32_t *colors;
    uint32_t color;
    uint8_t pressed;
} Sprite; 

/**
 * @brief Creates a sprite from an XPM image.
 * @param sprite XPM map representing the sprite image.
 * @return Pointer to the created Sprite structure if successful, NULL otherwise.
 */

Sprite *create_sprite_xpm(xpm_map_t sprite);

/**
 * @brief Creates a button sprite with a solid color.
 * @param width Width of the button sprite.
 * @param height Height of the button sprite.
 * @param color Color of the button sprite.
 * @return Pointer to the created Sprite structure if successful, NULL otherwise.
 */

Sprite *create_sprite_button(uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Destroys a sprite.
 * @param sprite Pointer to the sprite to be destroyed.
 */

void destroy_sprite(Sprite *sprite);

#endif
