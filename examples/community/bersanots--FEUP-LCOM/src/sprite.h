/**
 * @file sprite.h
 * @author Joao Cardoso (jcard@fe.up.pt) ????
 * @brief Sprite related functions
 * @version 0.1
 * @date 2021-01-05
 * 
 * Added by pfs@fe.up.pt
 * Adapted by Bernardo Santos (bernardo.ots@fe.up.pt)
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "constants.h"

/** @defgroup sprite Sprite
 * @{
 *
 * Sprite related functions
 */

/**
 * @brief A Sprite is an "object" that contains all needed information to
 * create, animate, and destroy a pixmap.  The functions assume that
 * the background is BLACK and they take into account collision with
 * other graphical objects or the screen limits.
 */
typedef struct {
  int x, y;           /**< current sprite position */
  int width, height;  /**< sprite dimensions */
  int xspeed, yspeed; /**< current speeds in the x and y direction */
  unsigned char *map; /**< the sprite pixmap (use read_xpm()) */
} Sprite;

/**
 * @brief Create with speeds and position (within the screen limits) a new sprite with pixmap "xpm".
 * 
 * @param xpm pixmap
 * @param x horizontal coordinate
 * @param y vertical coordinate
 * @param xspeed value to add to the position on the x axis
 * @param yspeed value to add to the position on the y axis
 * @return the created sprite object; NULL on invalid pixmap 
 */
Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed);

/**
 * @brief Draw the sprite "sp" on the screen.
 * 
 * @param sp Pointer to the sprite object containing the necessary information for the drawing of the sprite's pixmap
 */
void draw_sprite(Sprite *sp);

/**
 * @brief Erase the sprite "sp" from the screen.
 * 
 * @param sp Pointer to the sprite object containing the necessary information for the erase of the sprite's pixmap
 */
void erase_sprite(Sprite *sp);

/**
 * @brief Animate the sprites according to their attributes in memory.
 * First erase them, then move them and finally redraw them.
 * 
 * @param sprites array of pointers to the sprites to animate
 */
void animate_sprites(Sprite **sprites);

/**
 * @brief Erase the sprite from memory and release used resources.
 * 
 * @param sp pointer to the sprite to be destroyed
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Change the sprite's xspeed or yspeed according to the given direction.
 * 
 * @param sp pointer to the sprite object that will change speed
 * @param dir new direction that the sprite will follow
 */
void set_direction(Sprite *sp, direction dir);

/**
 * @brief Clear the sprite's xspeed or yspeed according to the given direction.
 * 
 * @param sp pointer to the sprite object that will change speed
 * @param dir direction that the sprite was following
 */
void clear_direction(Sprite *sp, direction dir);

/** @} end of sprite */

#endif
