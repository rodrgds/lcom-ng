#ifndef SPRITE_H
#define SPRITE_H

#include <lcom/lcf.h>

typedef struct s_sprite {
    int x;
    int y;

    int width;
    int height;

    uint8_t *colors;
    xpm_image_t img;
} sprite;

sprite *create_sprite(xpm_map_t xpm, int x, int y);
void destroy_sprite(sprite **sp);
void move_sprite(sprite *sp, int dx, int dy);

#endif
