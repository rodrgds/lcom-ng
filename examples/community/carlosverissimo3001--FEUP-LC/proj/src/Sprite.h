#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "graphics.h"
#include "macros.h"




/** @defgroup sprite
 * @{
 *
 *
 *
 * Functions and data structures used to deal with sprites
 */

typedef struct {
  int x;                /**< current sprite x position */
  int y;               /**< current sprite y position */
  int xspeed;         /**< current speed in the x direction */
  int yspeed;         /**< current speed in the y direction */
  uint8_t *map;       /**< the sprite pixmap */
  xpm_image_t img;    /**< the sprite image */
} Sprite;

/** @brief Creates a new sprite with pixmap "xpm", with specified
* position (within the screen limits) and speed;
* @param xpm pixmap
* @param x x coordinate
* @param y y coordinate
* @param xspeed horizontal speed
* @param yspeed vertical speed
* @return NULL on invalid pixmap.
*/
Sprite*create_sprite(xpm_map_t xpm , int x, int y, int xspeed, int yspeed);

/**
 * @brief Free memory allocated to the sprite
 * @param sp
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Draw a sprite in the screen
 *
 * @param sp Sprite to be drawn
 * @param addToBackgroundFrame If true, add the to to the frame permanently (mouse purposes)
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_sprite(Sprite *sp, bool addToBackgroundFrame);

/** @} end of sprite */

#endif
