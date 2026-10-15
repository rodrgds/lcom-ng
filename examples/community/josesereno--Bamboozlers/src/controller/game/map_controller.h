#ifndef _MAP_CONTROLLER_H_
#define _MAP_CONTROLLER_H_

/**
 * @file map_controller.h
 *
 * @brief Header file containing functions for map management, collision detection, and game setup.
 */

#include "../KBC/KBC.h"
#include "../mouse/mouse.h"
#include "../timer/timer.h"
#include "../keyboard/keyboard.h"
#include "../video/graphics.h"
#include "model/sprite.h"
#include "model/config.h"

extern Sprite* panda;
extern Sprite* item;
extern Sprite* map;
extern Sprite* home;
extern Sprite* lava;
extern Sprite* bamboo;
extern Sprite* little_block;
extern Sprite* little_plank;
extern Sprite* big_block;
extern Sprite* big_plank;

int minutes;
int seconds;

extern int counter;
extern Panda_state panda_state;
extern bool isRightPressed;
extern bool isLeftPressed;

/** @defgroup map_controller map_controller
 * @{
 */

/**
 * @brief Checks if an item is within the map boundaries.
 *
 * This function checks if the specified item is within the boundaries of the map.
 *
 * @param item The item to check.
 * @return True if the item is within the map boundaries, false otherwise.
 */
bool (is_in_map)(Sprite* item);

/**
 * @brief Checks if two sprites collide.
 *
 * This function checks if two sprites collide by comparing their bounding boxes pixel by pixel.
 *
 * @param s1 The first sprite.
 * @param s2 The second sprite.
 * @return True if the sprites collide, false otherwise.
 */
bool (collide)(Sprite* s1, Sprite* s2);

/**
 * @brief Sets up the game map for a specific level.
 *
 * This function sets up the game map based on the specified level.
 * It positions the sprites (panda, home, little_block, big_block, little_plank, big_plank, bamboo) according to the level.
 *
 * @param level The level of the game.
 */
void (map_set_up)(int level);

#endif 

