#ifndef WALL_VIEW_H
#define WALL_VIEW_H

#include "../../model/include/wall_model.h"
#include "../../../drivers/graphic/include/graphic.h"

/**
 * @brief Draws all the walls of the map on the screen.
 * 
 * @param wall Structure that contains the positions of the first wall;
 * @return 0 if it succeded, otherwise it returns 1;
 */
int wall_draw(wall_t* wall);

#endif
