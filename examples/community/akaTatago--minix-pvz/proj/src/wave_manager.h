/**
 * @file wave_manager.h
 * @brief Handles level progression and triggers zombie wave spawns.
 */

#ifndef __WAVE_MANAGER_H
#define __WAVE_MANAGER_H

#include <stdbool.h>

/**
 * @brief Loads the specified level configuration and initializes wave timers.
 * * @param level_number The level ID to initialize.
 */
void wave_manager_init(int level_number);

/**
 * @brief Main update function that decrements wave timers and spawns zombies when required.
 */
void wave_manager_update();

/**
 * @brief Checks if all waves have spawned and all zombies are defeated.
 * * @return true if the level is cleared, false otherwise.
 */
bool wave_manager_is_complete();

#endif
