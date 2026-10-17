#include "sprites.h"
#include "assets/pixmaps.h"

void (load_sprite) (uint16_t x, uint16_t y, uint16_t speed, uint16_t angle, sprite* sprite, xpm_map_t xpm){
    sprite->x = x;
    sprite->y = y;
    sprite->speed = speed;
    sprite->angle = angle;
    xpm_load(xpm, XPM_8_8_8_8, &sprite->info);
}
