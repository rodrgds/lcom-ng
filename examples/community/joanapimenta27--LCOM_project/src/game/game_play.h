/**
 * @file game_play.h
 * @brief Header file for game play functionality.
 */

#ifndef _GAME_PLAY_H
#define _GAME_PLAY_H

#include "../visual/visual.h"            // Include necessary headers
#include "../visual/visual_menu.h"
#include "../states/state_game.h"
#include "../sprites/sprites.h"
#include "game_utils.h"

/**
 * Move the player sprite to a new position.
 * @param pos The new position to move the player to.
 * @param i The frame index for player animation.
 */
void move_player(position_t pos, int i);

/**
 * Reset the player sprite to its initial position.
 */
void reset_player();

/**
 * Verify if the player's move to a new position is valid.
 * @param pos The position to verify.
 * @return True if the move is valid, false otherwise.
 */
bool verify_move(position_t pos);

/**
 * Handle player death event.
 * @param death_str The message to display upon player's death.
 */
void kill_player(const char *death_str);

/**
 * Check if the player has collected any coins at a given position.
 * @param pos The position to check for coins.
 */
void check_coin(position_t pos);

/**
 * Handle countdown for the game.
 */
void handle_countdown();

/**
 * Drain remaining time for a target.
 * @param target Pointer to the target whose time needs to be drained.
 */
void drain_target_time(target_t *target);

/**
 * Prepare values for a new level.
 * @param level_state The state of the level to prepare.
 */
void prepare_level_values(level_state_e level_state);

/**
 * Clear the targets array.
 */
void clear_targets_array();

/**
 * Unspawn targets if their time is up or they have been popped.
 * @return 1 if a target was unspawned, 0 otherwise.
 */
int unspawn_targets_if_applicable();

/**
 * Spawn a new target sprite at a given position.
 * @param spr Pointer to the sprite of the target to spawn.
 * @param pos The position to spawn the target.
 * @param target_index Index of the target in the targets array.
 * @return 1 if successful, 0 otherwise.
 */
int spawn_target(sprite_t* spr, position_t pos, int target_index);

/**
 * Spawn new targets according to the game's logic.
 * @return 1 if a target was spawned, 0 otherwise.
 */
int target_spawner();

/**
 * Get the target at a given index from the targets array.
 * @param i Index of the target to retrieve.
 * @return The target at the specified index.
 */
target_t get_target(int i);

/**
 * Add points to the player's score.
 * @param plus_points The number of points to add.
 */
void add_points(unsigned int plus_points);

/**
 * Get the minimum points required to pass a given level.
 * @param level_state The state of the level.
 * @return The minimum points required for the level.
 */
int get_min_points(level_state_e level_state);

#endif
