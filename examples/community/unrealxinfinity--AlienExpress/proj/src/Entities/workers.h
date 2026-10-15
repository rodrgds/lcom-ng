#ifndef WORKERS_H
#define WORKERS_H

#include "../macros/includes.h"
#include "../load/view.h"
#include <math.h>

/**
 * @brief Moves the workers enemies and calls the drawing function to draw them
 */
void move_workers();

/**
 * @brief Calls drawing function to draw a worker, taking into account the direction he faces
 * @param worker Worker image to draw
 */
void draw_workers(img_t worker);

#endif


