#ifndef __Sprite_h
#define __Sprite_h


#include "../devices/graphic.h"
#include "devices/rtc.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

real_time realTime;

typedef struct {
    int x, y; // current position
    int width, height; // dimensions
    uint32_t *map;
} Sprite;

void drawSprite(Sprite *sprite);

Sprite *create_sprite(xpm_map_t xpm, int x, int y);

void changeSprite(Sprite *sp, xpm_map_t xpm);

void drawMultipleSprite(int index, int wXpm, int hXpm, Sprite *sp);



#endif

