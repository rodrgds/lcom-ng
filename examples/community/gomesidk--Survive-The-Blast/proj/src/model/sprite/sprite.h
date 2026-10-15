#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <lcom/lcf.h>
#include <stddef.h>
#include <stdlib.h>
#include "graphics.h"

typedef enum {
    FULL_IMAGE,
    BY_LINE,
    BY_PIXEL
} SpriteType;

/** @file sprite.h
 * @brief Sprite management for the game
 * This module provides functions to create, destroy, and draw sprites.
 * 
 * It also defines the sprite type, which can be FULL_IMAGE, BY_LINE, or BY_PIXEL.
 */

struct sprite_imp;
typedef struct sprite_imp Sprite;

/**
 * @brief Returns the width of the sprite
 * 
 * @param sp Pointer to the sprite
 * @return The width of the sprite
 */
uint16_t get_sprite_width(Sprite *sp);

/**
 * @brief Returns the height of the sprite
 * 
 * @param sp Pointer to the sprite
 * @return The height of the sprite
 */
uint16_t get_sprite_height(Sprite *sp);

/** 
 * @brief Creates a new sprite from XPM "pic"
 * This function allocates memory for the sprite and loads the XPM image.
 * It does not draw the sprite on the screen.
 * 
 * @param pic The XPM image to be loaded
 * @return Pointer to the created sprite, or NULL if the pixmap is invalid
*/
Sprite *create_sprite(const xpm_map_t pic, SpriteType type);

/**
 * @brief Frees the memory allocated for the sprite
 * 
 * @param sp Pointer to the sprite to be destroyed
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Draws the sprite on the screen at the specified coordinates
 * 
 * @param sp Pointer to the sprite to be drawn
 * @param x The x-coordinate where the sprite will be drawn
 * @param y The y-coordinate where the sprite will be drawn
 */
void draw_sprite(Sprite *sp, uint16_t x, uint16_t y);

/** @} end of sprite */

#endif

