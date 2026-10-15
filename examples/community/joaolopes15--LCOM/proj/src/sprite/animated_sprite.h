#ifndef __ANIMATED_SPRITE_H__
#define __ANIMATED_SPRITE_H__

#include <lcom/lcf.h>
#include <stdint.h>
#include "sprite.h"

typedef struct {
  Sprite *sp;       // standard sprite
  int aspeed;       // no. frames per pixmap
  int cur_aspeed;   // no. frames left to next change
  int num_fig;      // number of pixmaps
  int cur_fig;      // current pixmap
  uint32_t **map;   // array of pointers to pixmaps
} AnimSprite;

AnimSprite *create_animated_sprite(xpm_map_t *xpm_array, uint8_t no_pic, int aspeed);

void destroy_animated_sprite(AnimSprite *as);

int draw_animated_sprite(AnimSprite *as, uint16_t x, uint16_t y);

void animate_sprite_once(AnimSprite *as);

#endif
