#ifndef WOLF_H
#define WOLF_H

#include "../macros/includes.h"
#include "../load/view.h"
#include <math.h>

/**
 * @brief Moves the wolf and calls the drawing function to draw it
 */
void move_wolf();

/**
 * @brief Calls drawing function to draw the attacking wolf, taking into account the direction he faces
 * @param wolf Wolf image to draw
 */
void draw_wolf(img_t wolf);
/**
 * @brief Calls drawing function to draw a friendly wolf, taking into account the direction he faces
 * @param wolf Wolf image to draw
 */
void draw_friendly_wolf(img_t wolf);
#endif
