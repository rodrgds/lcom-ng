/** 
 * @file arena_controller.h
 * @brief Header file for @ref arena_controller.c
 * 
 * This file declares all the functions to be used on the arena's logic.
 */

#ifndef _ARENA_CONTROLLER_PROJ_H_
#define _ARENA_CONTROLLER_PROJ_H_

#include <lcom/lcf.h>
#include "../model/structs.h"
#include "../model/entity.h"
#include "../model/arena.h"
#include "../model/attack.h"
#include "main_controller.h"
#include "enemy_controller.h"

/**
 * @brief Getter for the @ref arena_game_time static member.
 *
 * @return The time in seconds the arena was active.
 */
uint32_t (get_arena_game_time)();

/**
 * @brief Getter for the @ref arena_starting_frame static member.
 *
 * @return The frame when the arena was initialized.
 */
uint32_t (get_arena_starting_frame)();

/**
 * @brief Executes a ranged attack with a cooldown.
 * 
 * If the cooldown has passed, it executes a lightning attack in the position of the mouse and adds it to the arena's active attack list.
 * 
 * @param arena A pointer to the arena where the attack will be executed
 */
void (handle_ranged_attack)(arena* arena);

/**
 * @brief Executes the auto attack with a cooldown.
 * 
 * If the cooldown has passed, it executes the auto attack in the direction the player is facing and adds it to the arena's active attack list.
 * 
 * @param arena A pointer to the arena where the attack will be executed
 */
void (handle_auto_attack)(arena* arena);

/**
 * @brief Processes a frame on the passed arena.
 * 
 * This function is called once every frame on the main controller to process one frame on the arena.
 * First checks collisions with the enemies, reducing the health of the player with a cooldown of one second. Also handles the collisions with the heart power-ups that recover the player's missing health. <br>
 * It then handles the enemy and heart spawning by calling the functions handle_enemy_spawning() and handle_heart_spawning(), while also calling the handle_auto_attack() function to execute the normal attack on cooldown. <br>
 * At the end, it moves all the entities associated with the arena by calling arena_move_entities().
 * 
 * @param arena A pointer to the arena where the frame will be processed
 * @return 1 if the player died and thus the game ended, 0 otherwise
 */
int (arena_process_frame)(arena* arena);

/**
 * @brief Sets up the arena controller for a new game by resetting all the static variables.
 * 
 * Resets @ref last_auto_attack, @ref last_range_attack, @ref arena_starting_frame and @ref arena_game_time.
 */
void (setup_arena_controller)();

#endif /*_ARENA_CONTROLLER_PROJ_H_ */
