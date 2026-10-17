#ifndef SPRITE_H
#define SPRITE_H

typedef struct {
  int x, y;
  int xspeed, yspeed;
  xpm_image_t img;
  uint8_t *map;
} Sprite;


Sprite* create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed);

int draw_sprite(Sprite* sp);

void update_position(Sprite* sp, int new_x, int new_y);

int update_xpm(Sprite* sp, xpm_map_t xpm);

void update_sprite(Sprite* sp);

void release_sprite(Sprite *sp);

#endif

