#pragma once

#include <stdbool.h>

#include "../Position.h"

#include "MinionConfig.h"
#include "../champion/Champion.h"

/**
 * @brief Action state of a single minion.
 */
typedef enum {
    MINION_MOVING, /**< Walking toward the enemy base. */
    MINION_ATTACKING, /**< Attacking a nearby target. */
    MINION_DEAD /**< Eliminated; should be removed from the wave. */
} MinionState;

/**
 * @brief Data for a single minion unit.
 */
typedef struct {
    Position pos; /**< World position of the minion. */
    Team team; /**< Team this minion belongs to. */
    MinionState state; /**< Current action state. */
    int hp; /**< Current hit points. */
    int attack_timer; /**< Ticks remaining until the next attack is ready. */
} Minion;

/**
 * @brief A wave of minions spawned together.
 */
typedef struct {
    Minion minions[MINION_COUNT]; /**< Minions in this wave. */
    bool active; /**< False when all minions in the wave are dead. */
} MinionWave;

/**
 * @brief Container for all active minion waves on both teams.
 */
typedef struct {
    MinionWave red_waves[MAX_WAVES]; /**< Active waves for the red team. */
    MinionWave blue_waves[MAX_WAVES]; /**< Active waves for the blue team. */
    int red_count; /**< Number of active red waves. */
    int blue_count; /**< Number of active blue waves. */
    int spawn_timer; /**< Ticks remaining until the next wave spawns. */
} Minions;

/**
 * @brief Initialises a Minions container, zeroing all waves and resetting the spawn timer.
 * @param minions Container to initialise.
 */
void minionsInit(Minions *minions);
