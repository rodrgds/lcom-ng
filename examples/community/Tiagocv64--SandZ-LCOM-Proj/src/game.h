#pragma once

/** @defgroup game game
 * @{
 * Functions for manipulating the game
 */

/**
 * @brief      Initialize game structure.
 */
void init_game();

/**
 * @brief      Destroy game structure and free memory used by it
 */
void destroy_game();

/**
 * @brief      Updates current game
 */
void update_game();

/**
 * @brief      Pauses current game state
 */
void pause_game();

/**
 * @brief      Resumes previously saved game state
 */
void resume_game();
