#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "controller/video/graphics.h"

/** @defgroup sprite sprite
 * @{
  */
/**
 * @brief Structure representing a sprite.
 */
typedef struct {
    int x;                      /**< X coordinate of the sprite */
    int y;                      /**< Y coordinate of the sprite */
    int *width;                 /**< Array of sprite widths for each frame */
    int *height;                /**< Array of sprite heights for each frame */
    int xspeed;                 /**< X speed of the sprite */
    int yspeed;                 /**< Y speed of the sprite */
    int num_pixmaps;            /**< Number of sprite frames */
    int i;                      /**< Current frame index */
    uint32_t **pixmap_array;    /**< Array of sprite frame pixmaps */
} Sprite; 

/**
 * @brief Creates a sprite from XPM sprites.
 *
 * @param sprites Array of XPM sprites
 * @param num_sprites Number of XPM sprites
 * @param x Initial X coordinate of the sprite
 * @param y Initial Y coordinate of the sprite
 * @return Pointer to the created sprite, or NULL on failure
 */
Sprite *create_sprite_xpm(xpm_map_t *sprites, int num_sprites, int x, int y);

/**
 * @brief Destroys a sprite and frees its resources.
 *
 * @param sprite Pointer to the sprite to be destroyed
 */
void destroy_sprite(Sprite *sprite);

/**
 * @brief Sets the position of a sprite.
 *
 * @param sprite Pointer to the sprite
 * @param x New X coordinate of the sprite
 * @param y New Y coordinate of the sprite
 */
void setPos(Sprite* sprite, int x, int y);

#endif /* _SPRITE_H_ */
