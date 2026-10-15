#ifndef BOOTS_H
#define BOOTS_H


#include "../macros/includes.h"
#include "../load/view.h"

/**
 * @brief Calls drawing function to draw boots
 */
void draw_boots();

/**
 * @brief Sets the current inventory position to boots.
 * @param index Current inventory index position
 * 
 */
void get_boots(int index);

/**
 * @brief Equips boots by placing it in equipment instead of inventory
 * @param index Index position of inventory where boots is currently at
 */
void use_boots(int index);

#endif

