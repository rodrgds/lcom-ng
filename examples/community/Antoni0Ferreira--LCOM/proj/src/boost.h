#ifndef __BOOST_H
#define __BOOST_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "video.h"

//sprite  from http://pixelartmaker.com/art/c0e1e34c1b64249
#include "xpm/star.xpm"

/**
 * @defgroup boost boost 
 * @{
 * 
 * Functions regarding the initialization and drawing of a player boost
 */

/**
 * @brief Struct type for storing and organizing information relative to a boost
 * 
 */
struct boost {
    uint16_t x, y;
    xpm_image_t img;
    uint8_t *sprite;
};

/**
 * @brief Initializes the boost's position, xpm image and sprite
 * 
 * @param boost A player's boost 
 * @return Return 0 upon success and non-zero otherwise 
 */
int (initialize_boost)(struct boost *boost);
/**
 * @brief Draws the boost in its current position and determines
 * its next coordinates
 * 
 * @param boost A player's boost 
 * @param bgMap A xpm map of the game's background
 * @return Return 0 upon success and non-zero otherwise 
 */
int (draw_boost)(struct boost *boost, uint8_t *bgMap);

/**
 * @}
 */

#endif /* __BOOST_H */
