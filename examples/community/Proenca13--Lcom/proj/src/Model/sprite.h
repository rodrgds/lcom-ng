//
// Created by joao on 26-04-2024.
//

#ifndef SHARED_SPRITE_H
#define SHARED_SPRITE_H
#include "Controller/Video/graphic.h"

/**
 * @brief Struct representing a graphical sprite.
 *
 * This structure defines the properties of a graphical sprite, 
 * including its position, dimensions, speed, and pixel map. 
 * 
 */

typedef struct {
    int x, y; // current position
    int width, height; // dimensions
    int xspeed, yspeed; // current speed
    uint32_t *map; // the pixmap
} Sprite;

/**
 * @brief Creates a sprite from a given pixmap.
 * 
 * This function allocates memory for a Sprite structure and loads the pixmap
 * into it. It also sets the width and height of the sprite based on the pixmap.
 * 
 * @param pic The pixmap from which the sprite is to be created.
 * @return Sprite* Pointer to the created Sprite, or NULL if the creation failed.
 */

Sprite *create_sprite(xpm_map_t pic);

/**
 * @brief Destroys a sprite and frees allocated memory.
 * 
 * This function frees the memory allocated for a Sprite structure and its pixmap.
 * It sets the pointer to NULL to avoid dangling pointers, but note that the 
 * pointer is passed by value, so this change will not affect the caller's pointer.
 * 
 * @param sp Pointer to the Sprite to be destroyed.
 */

void destroy_sprite(Sprite *sp);
#endif //SHARED_SPRITE_H
