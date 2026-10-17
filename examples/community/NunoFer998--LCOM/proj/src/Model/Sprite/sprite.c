/**
 * @file sprite.c
 * @brief Sprite management implementation
 * 
 * This file contains the implementation of sprite-related functions
 * for loading, creating, and managing sprite objects from XPM files.
 */

#include "sprite.h"

#include <lcom/lcf.h>

/**
 * @brief Generates a sprite from an XPM map
 * 
 * This function creates a new sprite by loading an XPM image and
 * extracting its color data and dimensions.
 * 
 * @param xpm XPM map containing the sprite image data
 * @return Pointer to the generated sprite, or NULL on failure
 */
Sprite* generate_Sprite(xpm_map_t xpm) {
    Sprite *s = (Sprite*) malloc (sizeof(Sprite));
    if (s == NULL) return NULL;
    
    xpm_image_t x;
    // Load XPM data with 8_8_8_8 format (RGBA)
    s->color_map = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &x);
    s->h = x.height;  // Set sprite height
    s->w = x.width;   // Set sprite width

    // Check if color map was loaded successfully
    if( s->color_map == NULL ) {
      return NULL;
    }
    return s;
}

/**
 * @brief Frees memory allocated for a sprite
 * 
 * This function cleans up a sprite by freeing both the color map
 * and the sprite structure itself.
 * 
 * @param sprite Pointer to the sprite to be freed
 */
void freeSprite(Sprite * sprite) {
    free(sprite->color_map);  // Free the color map array
    free(sprite);             // Free the sprite structure
    sprite=NULL;              // Set pointer to NULL for safety
}

// Placeholder functions for future implementation
void generate_all_sprites();
void freeAllSprites();
