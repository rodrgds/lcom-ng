#pragma once

/** @brief Sprite width of a minion in pixels. */
#define MINION_W 30
/** @brief Sprite height of a minion in pixels. */
#define MINION_H 40
/** @brief Number of minions spawned per wave. */
#define MINION_COUNT 2
/** @brief Y coordinate of the minion lane. */
#define MINION_LANE_Y 330
/** @brief Horizontal spacing between minions in the same wave. */
#define MINION_SPACING 15
/** @brief Maximum number of simultaneous waves per team. */
#define MAX_WAVES 8
/** @brief Interval between wave spawns in seconds. */
#define SPAWN_SECONDS 10

/** @brief Starting HP of a minion. */
#define MINION_HP 100
/** @brief Movement speed of a minion in pixels per tick. */
#define MINION_SPEED 0.8
/** @brief Damage dealt by one minion attack. */
#define MINION_ATTACK_DMG 10
/** @brief Ticks between consecutive minion attacks. */
#define MINION_ATTACK_SPEED 50
/** @brief Maximum distance in pixels at which a minion can attack a target. */
#define ATTACK_RANGE 55
