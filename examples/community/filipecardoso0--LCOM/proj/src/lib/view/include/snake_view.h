#ifndef SNAKE_VIEW_H
#define SNAKE_VIEW_H

#include "../../model/include/snake_model.h"
#include "../../../drivers/graphic/include/graphic.h"


/**
 * @brief Draws a snake on the screen.
 * 
 * @param snake Structure that contains the positions of the snake's head and its tail, the snake's size and the diretion of its head as well.
 * @return 0 if succeded, otherwise it returns 1. 
 */
int snake_draw(snake_t* snake);

#endif
