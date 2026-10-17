#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "../controller/video.h"

/** @defgroup sprite sprite
 * @{
 *
 * Functions related to the sprites
 */

/**
 * @brief Struct for the sprite
 */
typedef struct {
  uint16_t height;     /*!< height of the sprite */
  uint16_t width;      /*!< width of the sprite */
  uint32_t *colors;    /*!< array with colors of the sprite */
  uint32_t background; /*!< background color of the sprite */
} Sprite;

/**
 * @brief Sprite constructor
 *
 * @param xpm XPM from where the sprite should be constructed
 * @param background background color of the sprite
 * @return Constructed sprite on success, NULL otherwise
 */
Sprite *construct_sprite(xpm_map_t xpm, uint32_t background);

/**
 * @brief Sprite destructor
 *
 * @param sprite pointer to the sprite to be destroyed
 */
void destroy_sprite(Sprite *sprite);

/**
 * @brief Numbers (and times symbol) constructor
 *
 * Initializes variables with the sprites.
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int construct_numbers();

/**
 * @brief Numbers (and times symbol) destructor
 *
 * Destroys the sprites.
 */
void destroy_numbers();

/**@}*/

#endif /* _SPRITE_H_ */
