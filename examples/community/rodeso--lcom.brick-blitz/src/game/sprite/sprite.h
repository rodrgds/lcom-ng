/**
 * @file sprite.h
 * @brief Header file for sprite manipulation functions.
 */

#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include <lcom/lcf.h>
#include "dvcs/graphics/vbe.h"
#include "xpm/background.xpm"
#include "xpm/menu.xpm"
#include "xpm/lost.xpm"
#include "xpm/won.xpm"
#include "xpm/ball.xpm"
#include "xpm/ball2.xpm"
#include "xpm/brick.xpm"
#include "xpm/paddle.xpm"
#include "xpm/missile.xpm"
#include "xpm/missileIcon.xpm"
#include "xpm/life.xpm"
#include "xpm/explosion.xpm"
#include "xpm/marker.xpm"

/**
 * @struct Sprite
 * @brief Structure representing a sprite.
 * @var Sprite::height
 * Height of the sprite.
 * @var Sprite::width
 * Width of the sprite.
 * @var Sprite::map
 * Pointer to the sprite's pixel map.
 */
typedef struct {
    uint16_t height; /**< Height of the sprite */
    uint16_t width; /**< Width of the sprite */
    uint32_t *map; /**< Pointer to the sprite's pixel map */
} Sprite;

/**
 * @brief Creates a sprite based on the given XPM image.
 *
 * @param pic XPM image representing the sprite.
 * @return Pointer to the created sprite, or NULL on failure.
 */
Sprite *create_sprite(xpm_map_t pic);

/**
 * @brief Destroys a sprite, freeing the allocated memory.
 *
 * @param sp Pointer to the sprite to be destroyed.
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Draws a sprite at the specified position on the screen.
 *
 * @param sprite Pointer to the sprite to be drawn.
 * @param x X-coordinate of the top-left corner of the sprite.
 * @param y Y-coordinate of the top-left corner of the sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int drawSprite(Sprite *sprite, int x, int y);

/**
 * @brief Erases a sprite from the specified position on the screen.
 *
 * @param sprite Pointer to the sprite to be erased.
 * @param x X-coordinate of the top-left corner of the sprite.
 * @param y Y-coordinate of the top-left corner of the sprite.
 * @return 0 upon success, non-zero otherwise.
 */
int eraseSprite(Sprite *sprite, int x, int y);

#endif /* _LCOM_SPRITE_H_ */
