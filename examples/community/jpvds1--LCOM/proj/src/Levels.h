/**

@file Levels.h
@brief Header file containing declarations of functions used for calling different levels, modes and menus of the games.

*/
#ifndef LEVELS_H
#define LEVELS_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include "proj.h"
#include "Menu.h"

extern int shootfrequency;
extern int movementmode;

/**
 * @brief Sends the user to the menu or a level/mode
 * 
 * @param lvl chooses mode (0 for levels, 1 for endless, 2 for instructions, 3 to exit, 4 for menu)
 * @return int 0 upon calling another successful mode, 1 otherwise
 */
int call_level(int lvl);
/**
 * @brief sets game variables and spawns enemies
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int call_endless();
/**
 * @brief spawns mob of enemies for the endless mode
 * 
 */
void spawn_endless();
/**
 * @brief sets game variables and spawns enemies for level 1. Calls level 2 when completed
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int call_1();
/**
 * @brief sets game variables and spawns enemies for level 2. Calls level 3 when completed
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int call_2();
/**
 * @brief sets game variables and spawns enemies for level 3.
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int call_3();

#endif //LEVELS_H
