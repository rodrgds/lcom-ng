#ifndef __ASTRO_H
#define __ASTRO_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "video.h"

//sprite from https://opengameart.org/content/brown-asteroid
#include "xpm/asteroid.xpm"

/**
 * @defgroup astro astro
 * @{
 * 
 * Functions regarding the initialization and movement of the astros
 */

/**
 * @brief Struct type for storing and organizing information relative to an astro
 * 
 */
struct astro {
    uint16_t x, y;
    xpm_image_t img;
    uint8_t *sprite;
    int8_t speed;
};

/**
 * @brief Initializes the astros that are included in the array of astros, more specifically
 * their initial position and speed, sprite and xpm image
 * 
 * @param astros Array of astros of the game
 * @return Return 0 upon success and non-zero otherwise 
 */
int (initialize_astros)(struct astro astros[]);

/**
 * @brief Moves all of the astros horizontally according to their speed
 * 
 * @param astros Array of astros of the game
 */
void (move_astros)(struct astro astros[]);

/**
 * @}
 */

#endif /* __ASTRO_H */
