#pragma once
#include <lcom/lcf.h>

#include "src/Model/Position.h"

/**
 * @file Sprite.h
 * @brief Defines the sprite_t structure used for 2D image representation.
 * 
 * Structure is used to represent graphical sprites in the game.
 */

/**
 * @struct sprite_t
 * @brief Represents a graphical sprite used for rendering on screen.
 *
 * Contains the raw image data (video map), its position on the screen,
 * size, and color depth information.
 */

typedef struct sprite_t
{
    uint8_t *video_map;     
    Position position;
    uint16_t width;         
    uint16_t height;        
    uint8_t bits_per_pixel;
} sprite_t;
