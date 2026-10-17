#ifndef ALLOCATIONS_MANAGER_H
#define ALLOCATIONS_MANAGER_H

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "../elements/elements.h"

///@brief Manages the allocation of game resources.
///@note Responsible for allocating memory for mouse, screens, game elements, players, buttons, and numbers.
void manage_game_allocations();

///@brief Manages the deletion of game resources.
///@note Responsible for freeing memory allocated for screens, game elements, players, buttons, and numbers.
void manage_game_deletes();

///@brief Manages the start states of the game.
///@note Initializes various game-related variables and positions the sprites.
void manage_start_states();

#endif
