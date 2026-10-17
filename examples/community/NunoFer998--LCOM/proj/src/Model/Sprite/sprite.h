/**
 * @file sprite.h
 * @brief Sprite structure and function declarations
 * 
 * This header file defines the Sprite structure and declares functions
 * for creating, managing, and destroying sprites used in the game.
 */

#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdio.h>
#include <stdint.h>

/**
 * @brief Sprite structure for storing image data
 * 
 * This structure holds the color map and dimensions of a sprite
 * used for rendering graphics in the game.
 */
typedef struct Sprite{
    uint32_t *color_map;  // Array of pixel colors in RGBA format
    uint16_t h, w;        // Height and width of the sprite
} Sprite; 

/**
 * @brief Generates a sprite from an XPM map
 * 
 * @param xpm XPM map containing sprite data
 * @return Pointer to the generated sprite
 */
Sprite* generate_Sprite(xpm_map_t xpm);

/**
 * @brief Frees memory allocated for a sprite
 * 
 * @param sprite Pointer to the sprite to be freed
 */
void freeSprite(Sprite * sprite);

/**
 * @brief Generates all sprites used in the game
 */
void generate_all_sprites();

/**
 * @brief Frees all sprites used in the game
 */
void freeAllSprites();

#endif
