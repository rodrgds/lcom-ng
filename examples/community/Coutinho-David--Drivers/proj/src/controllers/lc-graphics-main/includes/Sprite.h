#pragma once
#include <lcom/lcf.h>

typedef struct sprite_t
{
    uint8_t *video_map;     // pointer to the sprite map
    uint16_t x;             // x coordinate of the sprite
    uint16_t y;             // y coordinate of the sprite
    uint16_t width;         // width of the sprite
    uint16_t height;        // height of the sprite
    uint8_t bits_per_pixel; // bits per pixel
} sprite_t;
