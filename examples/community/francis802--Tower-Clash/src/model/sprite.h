/**
 * @file sprite.h
 * 
 * @brief This file contains the definition of the Sprite struct and the
 * functions to create and destroy sprites.
*/

#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include "controller/video/graphics.h"

/**
 * @brief Struct that represents a sprite
 * 
*/
typedef struct {
    uint16_t height;
    uint16_t width;
    uint32_t *colors;
    uint32_t color;
    uint8_t pressed;
    uint16_t x_pos;
    uint16_t y_pos;
} Sprite; 

/**
 * @brief This function creates a sprite from a xpm map, allocating the necessary
 * memory for it.
 * 
 * @param sprite Xpm map of the sprite
 * @return Pointer to the created sprite on success, NULL otherwise
*/
Sprite *create_sprite_xpm(xpm_map_t sprite);

/**
 * @brief This function destroys a sprite, liberating the memory allocated for it.
 * 
 * @param sprite Sprite to be destroyed
*/
void destroy_sprite(Sprite *sprite);

#endif
