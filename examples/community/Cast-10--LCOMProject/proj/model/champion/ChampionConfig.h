#pragma once

#include "../Position.h"

#include "vbe.h"

/** @brief Sprite width of a champion in pixels. */
#define CHAMPION_W 100
/** @brief Sprite height of a champion in pixels. */
#define CHAMPION_H 100

/** @brief Starting and maximum HP of a champion. */
#define CHAMPION_HP 100
/** @brief Base movement speed in pixels per tick. */
#define CHAMPION_SPEED 1.3
/** @brief Base attack damage dealt per hit. */
#define CHAMPION_ATTACK_DMG 10
/** @brief Fraction of the sprite size used as the collision box (0.5 = 50%). */
#define COLLISION_PROPORTION 0.5
/** @brief Fraction of the sprite size added as attack-range offset. */
#define ATTACK_RANGE_OFFSET 0.05
/** @brief Respawn countdown in seconds after a champion dies. */
#define RESPAWN_SECONDS 9
/** @brief Sentinel value stored in the respawn timer when the champion is alive. */
#define ALIVE -1

/** @brief Spawn/respawn position for the blue (player 1) champion. */
#define BLUE_SPAWN (Position){CHAMPION_W, CHAMPION_H}
/** @brief Spawn/respawn position for the red (player 2) champion. */
#define RED_SPAWN (Position){MODE105_WIDTH - 2*CHAMPION_W, CHAMPION_H}
