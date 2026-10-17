/**
 * @file plants_cooldown.h
 * @brief Cooldown timers and management for plant placement.
 */

#ifndef __PLANTS_COOLDOWN_H
#define __PLANTS_COOLDOWN_H

#include <stdbool.h>
#include "board.h"

#define COOLDOWN_FAST       450  // 7.5s
#define COOLDOWN_SLOW       1800 // 30s
#define COOLDOWN_VERY_SLOW  3000 // 50s

/**
 * @brief Initializes the cooldown timers array.
 */
void cooldowns_init();

/**
 * @brief Updates and decreases active cooldowns on every frame.
 */
void cooldowns_update();

/**
 * @brief Checks if a specific plant type is off cooldown and ready to be placed.
 * * @param type The type of plant.
 * @return true if the plant is ready, false if it's still cooling down.
 */
bool cooldown_is_ready(plant_type_t type);

/**
 * @brief Starts the cooldown timer for a specific plant type after placement.
 * * @param type The type of plant.
 */
void cooldown_start(plant_type_t type);

#endif
