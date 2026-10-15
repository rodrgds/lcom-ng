#ifndef SPRITE_H
#define SPRITE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "../../config.h"

/**
 * In LCOM, xpm_transparency_color(enum xpm_image_type) returns the exact 
 * uint32_t color that xpm_load maps "None" to.
 */
#define TRANSPARENT_COLOR xpm_transparency_color(GAME_XPM_TYPE)

typedef struct Sprite Sprite;

/**
 * @brief Gets the width of a Sprite.
 * @param sprite Sprite to inspect.
 * @return Width in pixels.
 */
uint16_t sprite_get_width(const Sprite *sprite);

/**
 * @brief Gets the height of a Sprite.
 * @param sprite Sprite to inspect.
 * @return Height in pixels.
 */
uint16_t sprite_get_height(const Sprite *sprite);

/**
 * @brief Gets the ARGB pixel array of a Sprite.
 * @param sprite Sprite to inspect.
 * @return Pointer to the array of colors.
 */
const uint32_t* sprite_get_colors(const Sprite *sprite);

/**
 * @brief Creates a Sprite from an XPM map, loading pixel colors in ARGB format.
 * @param xpm The XPM map array (e.g. declared as static char *my_xpm[])
 * @return Pointer to the allocated Sprite, or NULL on failure.
 */
Sprite* create_sprite_xpm(xpm_map_t xpm);

/**
 * @brief Frees all memory associated with a Sprite.
 */
void destroy_sprite(Sprite *sprite);

/**
 * @brief Creates an exact copy of a Sprite, but with the pixels flipped horizontally.
 * @param orig Pointer to the original Sprite.
 * @return Pointer to the new allocated Sprite, or NULL on error.
 */
Sprite* create_sprite_flipped_horizontal(const Sprite *orig);

#endif /* SPRITE_H */
