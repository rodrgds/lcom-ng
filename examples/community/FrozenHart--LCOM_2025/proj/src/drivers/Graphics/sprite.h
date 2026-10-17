#ifndef _LCOM_SPRITES_H
#define _LCOM_SPRITES_H

#include "graphics.h"
#include <lcom/lcf.h>
typedef struct {
  uint32_t *map;        // Sprite pixel data
  uint32_t *background; // Saved background pixels
  int width;
  int height;
  int last_x; // Last drawn position
  int last_y;
  bool has_background; // True if background is saved
} Sprite;

Sprite *create_sprite(const char *pic[]);
int draw_sprite(Sprite *sprite, int x, int y);
void destroy_sprite(Sprite *sp);
Sprite *create_sprite_neutral(const char *pic[]);
int restore_background(Sprite *sprite);
int draw_sprite_and_restore(Sprite *sprite, int x, int y);
Sprite *create_sprite_rectangle(int w, int h, uint32_t color);

#endif // SPRITES_H
