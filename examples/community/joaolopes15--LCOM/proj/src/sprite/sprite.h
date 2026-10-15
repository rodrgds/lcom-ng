#ifndef SPRITE_H
#define SPRITE_H

#include <lcom/lcf.h>
#include "../drivers/video/video.h"

typedef struct {
  int x, y;
  int width, height;
  int xspeed, yspeed;
  uint32_t *map;
} Sprite;

Sprite *create_sprite(xpm_map_t xpm);

void destroy_sprite(Sprite *sprite);

int draw_sprite(Sprite *sprite, uint16_t x, uint16_t y);

int move_sprite_down(Sprite *sprite);
int move_sprite_up(Sprite *sprite);
int move_sprite_left(Sprite *sprite);
int move_sprite_right(Sprite *sprite);
void move_sprite_mouse(Sprite *sprite, int target_x, int target_y, int xspeed);

#endif
