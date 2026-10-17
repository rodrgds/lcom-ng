/**
 *  @author Joao Cardoso (jcard@fe.up.pt)
 *  Added by pfs@fe.up.pt
 *  Change by us to be able to accept 16 bit image using uint16_t pointer instead of char pointer
 *  Relative weight: 5 %
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <stdint.h>

/** @defgroup sprite Sprite
 * @{
 *
 * Sprite related functions
 */

/** A Sprite is an "object" that contains all needed information to
 * create, animate, and destroy a pixmap.  The functions assume that
 * the background is BLACK and they take into account collision with
 * other graphical objects or the screen limits. 
 */
typedef struct {
  int x,y;             /**< current sprite position */
  int width, height;   /**< sprite dimensions */
  int xspeed, yspeed;  /**< current speeds in the x and y direction */
  uint16_t *map;           /**< the sprite pixmap (use read_xpm()) */
} Sprite;

/** @} end of sprite */

#endif
