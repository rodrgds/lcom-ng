#ifndef FRUIT_VIEW_H
#define FRUIT_VIEW_H

#include "../../model/include/fruit_model.h"
#include "../../../drivers/graphic/include/graphic.h"

/**
 * @brief Draws a fruit on the screen.
 * 
 * @param fruit Structure that contains the fruit's position.
 * @return 0 if it succeded, otherwise it returns 1.
 */
int fruit_draw(fruit_t* fruit);

#endif
