#ifndef PISTOL_H
#define PISTOL_H

#include "../macros/includes.h"
#include "../load/view.h"

/**
 * @brief Calls drawing function to draw pistol
 * @param index index position of pistol to draw
 */
void draw_pistol(int index);

/**
 * @brief Sets the current inventory position to a pistol.
 * @param index Current inventory index position
 * 
 */
void get_pistol(int index);

/**
 * @brief Uses all ammo available from inventory to reload the pistol that is being held
 * @param index Index position of pistol to use
 */
void use_pistol(int index);

#endif

