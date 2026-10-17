#pragma once

#include <lcom/lcf.h>
#include "errors.h"

/**
 * @brief Handles operations to inicialize the game
 *
 * @return 0 if the operation was successful, non-zero otherwise 
 */
void (game_init)();

/**
 * @brief Handles operations to terminate the game
 * 
 * @return 0 if the operation was successful, non-zero otherwise
 */
void (game_cleanup)();

/**
 * @brief Starts the game
 * 
 */
void (game_state_start)();

/**
 * @brief Ends the game
 * 
 */
void (game_state_end)();
