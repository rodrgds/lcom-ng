#ifndef INVENTORY_H
#define INVENTORY_H

#include "../macros/includes.h"
#include "../load/view.h"
#include "../Items/package.h"
/**
 * @brief Calls the drawing function to draw the inventory images and also the items inside the inventory
 */
void draw_inventory();
/**
 * @brief Removes item from equipment, placing it on the ground/board
 * @param index Index of position of equipment where item currently resides
 */
void remove_from_equipment(int index);

/**
 * @brief Counts the number of packages that is in the player's inventory
 * @return Returns the count of packages
 */
int count_packages();

/**
 * @brief Determines whether there is space on the inventory or not
 * @return Returns -1 if there isn't and if there is it returns the inventory index of the nearest free place
 */
int is_inventory_full();


#endif
