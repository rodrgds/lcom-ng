#ifndef PACKAGE_H
#define PACKAGE_H

#include "../macros/includes.h"
#include "../load/view.h"
#include "pizza.h"
#include "ammo.h"
#include "pistol.h"
#include "boots.h"
#include "backpack.h"
#include "bone.h"
#include "grenade.h"
#include "watch.h"

/**
 * @brief Calls drawing function to draw packages
 */
void draw_packages();

/**
 * @brief Calls functions to use item being currently help
 * @param index Index position of inventory of item to use
 */
void use_item(int index);

/**
 * @brief Randomizes the item to be received after opening a package
 * @param index inventory index position on which the item will be put
 * @param rate the randomizing rate for the chosen item
 * @return Returns 1 if item was found and 0 if otherwise
 */
int get_item(int index, int rate);


#endif

