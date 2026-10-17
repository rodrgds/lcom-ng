/**
 * @file sprite.h
 * @brief Sprite management for game graphics, including creation, drawing and destruction.
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "assets/letters/a.xpm"
#include "assets/letters/b.xpm"
#include "assets/letters/c.xpm"
#include "assets/letters/d.xpm"
#include "assets/letters/e.xpm"
#include "assets/letters/f.xpm"
#include "assets/letters/g.xpm"
#include "assets/letters/h.xpm"
#include "assets/letters/i.xpm"
#include "assets/letters/j.xpm"
#include "assets/letters/k.xpm"
#include "assets/letters/l.xpm"
#include "assets/letters/m.xpm"
#include "assets/letters/n.xpm"
#include "assets/letters/o.xpm"
#include "assets/letters/p.xpm"
#include "assets/letters/q.xpm"
#include "assets/letters/r.xpm"
#include "assets/letters/s.xpm"
#include "assets/letters/t.xpm"
#include "assets/letters/u.xpm"
#include "assets/letters/v.xpm"
#include "assets/letters/w.xpm"
#include "assets/letters/x.xpm"
#include "assets/letters/y.xpm"
#include "assets/letters/z.xpm"

#include "assets/numbers/zero.xpm"
#include "assets/numbers/one.xpm"
#include "assets/numbers/two.xpm"
#include "assets/numbers/three.xpm"
#include "assets/numbers/four.xpm"
#include "assets/numbers/five.xpm"
#include "assets/numbers/six.xpm"
#include "assets/numbers/seven.xpm"
#include "assets/numbers/eight.xpm"
#include "assets/numbers/nine.xpm"

#include "assets/mouse/cursor.xpm"
#include "assets/mouse/mouse_cursor.xpm"

#include "assets/title.xpm"

#include "drivers/video/graphics.h"

#include <lcom/lcf.h>


#define NUM_LETTERS 26
#define NUM_NUMBERS 10

/**
 * @struct Sprite
 * @brief Structure representing a sprite with width, height and pixel map.
 */
typedef struct Sprite {
  uint16_t width;
  uint16_t height;
  uint32_t* map;
} Sprite;

extern Sprite* letter_sprites[NUM_LETTERS];
extern Sprite* number_sprites[NUM_NUMBERS]; 

extern Sprite* gameTitle;
extern Sprite* mouseCursor;

/**
 * @brief Creates a sprite from an XPM map.
 * 
 * @param xpm_map The XPM map to create the sprite.
 * @return Pointer to the created sprite, or NULL if creation failed.
 */
Sprite* create_sprite(xpm_map_t xpm_map);

/**
 * @brief Draws a sprite at the specified coordinates.
 * 
 * @param sprite Pointer to the sprite to draw.
 * @param x The x-coordinate where the sprite will be drawn.
 * @param y The y-coordinate where the sprite will be drawn.
 * @return 0 on success, non-zero on failure (e.g., if drawing a pixel fails).
 */
int draw_sprite(Sprite* sprite, uint16_t x, uint16_t y);

/**
 * @brief Frees the memory used by a sprite.
 * @param sprite Pointer to the sprite to destroy.
 */
void destroy_sprite(Sprite* sprite);

/**
 * @brief Loads all global sprites used in the game.
 */
void load_sprites();

/**
 * @brief Frees all global sprites used in the game.
 */
void unload_sprites();

#endif // _SPRITE_H_
