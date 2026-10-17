/** 
 * @file arena.h
 * @brief Header file for @ref arena.c
 * 
 * This file declares all the functions to be used for the arena struct.
 */

#ifndef _ARENA_PROJ_H_
#define _ARENA_PROJ_H_

#include <lcom/lcf.h>

#include "entity.h"
#include "structs.h"
#include "../events/events.h"
#include "../drivers/drivers.h"
#include "../controller/main_controller.h"

/**
 * @brief Creates an arena by dynamically allocating space
 * 
 * Creates an arena with the starting parameters: all the lists empty and the player in the middle.
 * 
 * @return The pointer to the new arena
 */
arena* (arena_create)();

/**
 * @brief Destroys an arena by freeing its memory
 * 
 * Calls entity_destroy() to destroy the player and entity_list_destroy() to destroy the enemies, the player_attacks and the hearts. Finally frees the memory of the arena itself.
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (arena_destroy)(arena* arena);


#endif /*_ARENA_PROJ_H_*/
