#pragma once

/** @brief Total number of champion animation clips (4 directions × 3 states). */
#define NUM_ANIMS 12
/** @brief Number of frames in each attack animation. */
#define ATTACK_FRAMES 2
/** @brief Ticks per frame for attack animations. */
#define ATTACK_TIME 20
/** @brief Number of frames in each walk animation. */
#define MOVE_FRAMES 2
/** @brief Ticks per frame for walk animations. */
#define MOVE_TIME 10

#include <stdlib.h>

#include "Sprite.h"
#include "AnimSprite.h"

#include "../../assets/game/players/players_north_xpms.h"
#include "../../assets/game/players/players_south_xpms.h"
#include "../../assets/game/players/players_east_xpms.h"
#include "../../assets/game/players/players_west_xpms.h"

/**
 * @brief Builds the full set of champion AnimSprite clips for the given player.
 *
 * Returns a heap-allocated array of NUM_ANIMS pointers laid out as:
 *  - [0..3] Idle animations for each of the 4 directions.
 *  - [4..7] Attack animations for each of the 4 directions.
 *  - [8..11] Walk animations for each of the 4 directions.
 *
 * The direction order within each group is (NORTH, SOUTH, EAST, WEST).
 *
 * @param player Player index (1 for blue, 2 for red).
 * @return Heap-allocated array of NUM_ANIMS AnimSprite pointers, or NULL on failure.
 */
AnimSprite **buildAnimations(int player);
