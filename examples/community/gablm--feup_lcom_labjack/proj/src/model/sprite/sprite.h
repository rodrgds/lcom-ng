#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include <lcom/lcf.h>
#include "../../drivers/drivers.h"

/**
 * @defgroup sprite Sprite
 * @ingroup models
 * @{
 * 
 * @brief Sprite module for handling sprite-related operations.
 */

/**
 * @struct sprite_t
 * @brief Structure to represent a sprite.
 */
typedef struct {
	uint32_t x;         /**< X coordinate of the sprite. */
	uint32_t y;         /**< Y coordinate of the sprite. */
	xpm_image_t img;    /**< XPM image of the sprite. */
	uint8_t *map;       /**< Pixel map of the sprite. */
} sprite_t;

/**
 * @brief Creates a sprite from an XPM map.
 *
 * @param map XPM map to create the sprite from.
 * @return Pointer to the created sprite, or NULL if creation fails.
 */
sprite_t *sprite_create(xpm_map_t map);

/**
 * @brief Destroys a sprite, freeing all allocated resources.
 *
 * @param sprite Pointer to the sprite to be destroyed.
 */
void sprite_destroy(sprite_t *sprite);

/**
 * @brief Destroys a sprite from a queue.
 *
 * @param sprite Pointer to the sprite to be destroyed.
 */
void sprite_queue_destroy(void *sprite);

/**
 * @brief Draws the sprite on the screen.
 *
 * @param sprite Pointer to the sprite to be drawn.
 * @return 0 on success, 1 on failure.
 */
int sprite_draw(sprite_t *sprite);

/**
 * @brief Draws the sprite on the screen with forced color.
 *
 * @param sprite Pointer to the sprite to be drawn.
 * @param color Color to force on the sprite.
 * @return 0 on success, 1 on failure.
 */
int sprite_draw_force_color(sprite_t *sprite, uint32_t color);

/**
 * @brief Draws the sprite on the screen with rotation.
 *
 * @param sprite Pointer to the sprite to be drawn.
 * @return 0 on success, 1 on failure.
 */
int sprite_draw_rotate(sprite_t *sprite);

/**
 * @brief Moves the sprite to a new position.
 *
 * @param sprite Pointer to the sprite to be moved.
 * @param x New x coordinate for the sprite.
 * @param y New y coordinate for the sprite.
 * @return 0 on success, 1 on failure.
 */
int sprite_move(sprite_t *sprite, uint32_t x, uint32_t y);

/**
 * @brief Checks if two sprites collide.
 *
 * @param sprite1 Pointer to the first sprite.
 * @param sprite2 Pointer to the second sprite.
 * @return 1 if the sprites collide, 0 otherwise.
 */
int sprite_colides(sprite_t *sprite1, sprite_t *sprite2);

/** @} */

#endif /* _LCOM_SPRITE_H_ */
