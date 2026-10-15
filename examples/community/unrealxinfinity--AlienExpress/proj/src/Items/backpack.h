#ifndef BACKPACK_H
#define BACKPACK_H

#include "../macros/includes.h"
#include "../load/view.h"

/**
 * @brief Calls drawing function to draw backpack
 */
void draw_backpack();

/**
 * @brief Sets the current inventory position to backpack.
 * @param index Current inventory index position
 * 
 */
void get_backpack(int index);

/**
 * @brief Changes backpack from inventory to equipment
 * @param index Index position of iventory where backpack currently resides
 */
void use_backpack(int index);

#endif


