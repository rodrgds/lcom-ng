
#ifndef _SPRITE_H_
#define _SPRITE_H_

/** @defgroup sprite Sprite
 *  @{
 *
 *  Sprite related functions
 */

/** A Sprite is an "object" that contains all needed information to
 *  create, animate, and destroy a pixmap.  The functions assume that
 *  the background is BLACK and they take into account collision with
 *  other graphical objects or the screen limits.
 */
typedef struct {
  int x;				/**< Current X-position. */
  int y;				/**< Current Y-position. */
  int width;			/**< Sprite's width. */
  int height;			/**< Sprite's height. */
  int xspeed;			/**< Sprite's current X-speed. */
  int yspeed;			/**< Sprite's cuurrent Y-speed. */
  char *map;			/**< Sprite's pixmap (use read_xpm()). */
} Sprite;

/**
 *  @brief Creates with random speeds (not zero) and position (within the screen limits).
 *  @param pic Pixmap of the sprite.
 *  @param x1 X-position of the sprite.
 *  @param y1 Y-position of the sprite. *
 *  @return Returns pointer to sprite or NULL on invalid pixmap.
 */
Sprite * create_sprite(char *pic[], int x1, int y1);

/**
 *  @brief Draws a sprite on the screen.
 *  @param sp Pointer to the sprite to draw.
 */
void vg_drawSprite(Sprite *sp);

/**
 *  @brief Erases a sprite from the screen.
 *  @param sp Pointer to the sprite to erase.
 */
void vg_eraseSprite(Sprite *sp);

/**
 *  @brief The "sp" sprite is erased from memory whose address is "base" and used resources released.
 *  @param sp Pointer to sprite to destroy.
 */
void destroy_sprite(Sprite *sp);

/** @} end of sprite */

#endif
