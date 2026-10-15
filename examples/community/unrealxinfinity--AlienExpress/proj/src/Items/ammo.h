#ifndef AMMO_H
#define AMMO_H

#include "../macros/includes.h"
#include "../load/view.h"

/**
 * @brief Calls drawing function to draw ammo
 * @param index Index position of ammo to draw
 */
void draw_ammo(int index);

/**
 * @brief Sets the current inventory position to ammo.
 * @param index Current inventory index position
 * 
 */
void get_ammo(int index);



#endif

