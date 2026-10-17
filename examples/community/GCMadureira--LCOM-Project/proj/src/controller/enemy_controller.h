/** 
 * @file enemy_controller.h
 * @brief Header file for @ref enemy_controller.c
 * 
 * This file declares all the functions to be used on the enemies's logic.
 */

#ifndef _ENEMY_CONTROLLER_PROJ_H_
#define _ENEMY_CONTROLLER_PROJ_H_

#include <lcom/lcf.h>
#include "../model/structs.h"
#include "../model/entity.h"
#include "../model/arena.h"
#include "main_controller.h"


/**
 * @brief Spawns an enemy offscreen.
 * 
 * Picks a random position 10 pixels away from the map limit and spawns an enemy on the resulting random position, adding it to the arena's list of active enemies.
 * 
 * @param arena A pointer to the arena where the enemy will be spawned
 * @param speed_multiplier The speed multiplier of the new enemy
 * @param health The health of the new enemy
 * @param animations A pointer to the animations the new enemy will use
 */
void (spawn_enemy)(arena* arena, double speed_multiplier, uint32_t health, animation* animations);

/**
 * @brief Spawns enemies on the arena with cooldown
 * 
 * Spawns different types of enemies with different spawn rates on the arena, limited by the @ref MAX_ENEMIES count.
 * 
 * @param arena A pointer to the arena where the enemies will spawn
 */
void (handle_enemy_spawning)(arena* arena);

/**
 * @brief Sets up the enemy controller for a new game by resetting all the static variables.
 * 
 * Resets @ref last_normal_enemy_spawn, @ref last_mummy_spawn and @ref enemy_count.
 */
void (setup_enemy_controller)();

/**
 * @brief Checks the collisions of the active enemies with the player and with attacks
 * 
 * Iterates through the list of active enemies and decreases their health if hit by an attack, using check_attack_collison() to check, removing them from the arena if the health reaches 0. <br>
 * If the enemy was not removed, calls check_player_collision() to check if the it collided with the player.
 * 
 * @param arena A pointer to the arena where the collisions will be checked
 * @return true if an enemy collided with the player, false otherwise
 */
bool (handle_enemy_collisions)(arena* arena);

/**
 * @brief Checks if the player is colliding with the enemy.
 * 
 * Uses a simple box collision detection utilizing the image sizes of the sprites. Deflates the hitboxes by a small ammount to make the collisions more accurate.
 * 
 * @param enemy A pointer to the enemy entity to check
 * @param player A pointer to the player entity to check
 * 
 * @return true if the entities are colliding, false otherwise
 */
bool (check_player_collision)(entity* enemy, entity* player);

/**
 * @brief Checks if the enemy is colliding with the attack.
 * 
 * Uses a simple box collision detection utilizing the image sizes of the sprites. Inflates the hitboxes by a small ammount to make the collisions easier to achieve.
 * 
 * @param enemy A pointer to the enemy entity to check
 * @param player A pointer to the player entity to check
 * 
 * @return true if the entities are colliding, false otherwise
 */
bool (check_attack_collision)(entity* enemy, attack* attack);

/**
 * @brief Moves all the enemies one frame
 * 
 * Iterates through the list of active enemies and calculates the direction they need to move (towards the player).
 * 
 * @param arena A pointer to the arena whose enemies will be moved
 */
void (enemies_move)(arena* arena);


#endif /*_ENEMY_CONTROLLER_PROJ_H_ */
