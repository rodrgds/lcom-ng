#ifndef _COLOR_H
#define _COLOR_H

#include <lcom/lcf.h>

#include "Sprite.h"
#include "macros.h"

#include "img/xpms/colors/color_black.xpm"
#include "img/xpms/colors/color_white.xpm"
#include "img/xpms/colors/color_red.xpm"
#include "img/xpms/colors/color_green.xpm"
#include "img/xpms/colors/color_orange.xpm"
#include "img/xpms/colors/color_violet.xpm"
#include "img/xpms/colors/color_grey.xpm"
#include "img/xpms/colors/color_blue.xpm"
#include "img/xpms/eraser.xpm"

#define NO_COLORS 7

/** @defgroup color
 * @{
 *
 *
 *
 * Functions and data structures that deal with all the color operations needed for the game
 */

/* Data structure used to store a color */
typedef struct {
    uint32_t color; 	        /**<@brief Color rgb value */
    Sprite *sp;                 /**<@brief Color visual representation */
} Color;

/**
 * @brief  Creates the colors, i.e, loads their xpm into memory and assigns them a value
 *
 */
void (load_colors)();

/**
 * @brief Free memory allocated to the colors
 *
 */
void(destroy_colors)();

/**
 * @brief Draws all the colors in the correct screen coordinates
 *
 */
void(draw_colors)();

/**
 * @brief Getter method: Given an index, returns the corresponding color rgb value
 * @param index Position in the array
 * @return RGB value of the desired color
 */
uint32_t(get_color_rgb)(int index);


/** @}*/

#endif /* _COLOR_H */


