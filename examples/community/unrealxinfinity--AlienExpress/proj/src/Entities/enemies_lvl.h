#ifndef ENEMIES_LVL_H
#define ENEMIES_LVL_H

#include "../macros/includes.h"
#include "../load/view.h"
#include <math.h>

/**
 * @brief Moves the zombie enemies and calls the drawing function to draw them
 */
void move_enemies_lvl();

/**
 * @brief Calls drawing function to draw a zombie, taking into account the direction he faces
 * @param enemy Zombie image to draw
 */
void draw_enemies_lvl(img_t enemy);


#endif

