#ifndef __POINTS_H
#define __POINTS_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "video.h"
#include "xpm/0.xpm"
#include "xpm/1.xpm"
#include "xpm/2.xpm"
#include "xpm/3.xpm"
#include "xpm/4.xpm"
#include "xpm/5.xpm"
#include "xpm/6.xpm"
#include "xpm/7.xpm"
#include "xpm/8.xpm"
#include "xpm/9.xpm"
//sprites from http://pixelartmaker.com/art/112944c1e93844b

/**
 * @defgroup points points
 * @{
 * 
 * Functions regarding the initialization and drawing of the points
 */

#define POINTS_X 110 // Initial X coordinate of the points
#define POINTS_Y 550 // Initial Y coordinate of the points

uint8_t *maps[10];
xpm_image_t imgs[10];


/**
 * @brief Initializes the xpm maps and images of all 
 * of the numbers needed for drawing the points on screen,
 * as well as starting the points at '0' 
 * 
 */
int (initialize_points)();

/**
 * @brief Draws the points and updates the value shown on screen whenever a player gets a point
 * 
 * @param points Points of the player
 * @return Return 0 upon success and non-zero otherwise 
 */
int (draw_points)(int points);

/**
 * @}
 */

#endif /* __POINTS_H */
