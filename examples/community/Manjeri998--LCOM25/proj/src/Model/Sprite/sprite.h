#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdio.h>
#include <stdint.h>

typedef struct Sprite{
    uint32_t *color_map;
    uint16_t h, w;
} Sprite; 

Sprite* generate_Sprite(xpm_map_t xpm);
void freeSprite(Sprite * sprite);
void generate_all_sprites();
void freeAllSprites();

#endif
