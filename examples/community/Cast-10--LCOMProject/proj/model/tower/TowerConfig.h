#pragma once

/** @brief Sprite width of a tower in pixels. */
#define TOWER_W 150
/** @brief Sprite height of a tower in pixels. */
#define TOWER_H 225
/** @brief Y coordinate of the tower lane. */
#define TOWER_LANE_Y 270
/** @brief Horizontal margin from the map edge to the tower. */
#define TOWER_X_MARGIN 5

/** @brief Starting HP of a tower. */
#define TOWER_HP 20
/** @brief Damage dealt by one tower projectile on impact. */
#define TOWER_DMG 30
/** @brief Ticks between consecutive tower shots. */
#define TOWER_ATTACK_SPEED 90
/** @brief Maximum range in pixels within which a tower can acquire a target. */
#define TOWER_RANGE 300

/** @brief Movement speed of a tower projectile in pixels per tick. */
#define PROJECTILE_SPEED 4
/** @brief Radius in pixels within which a projectile is considered to have hit its target. */
#define PROJECTILE_HIT_RADIUS 3

/** @brief Width of the projectile sprite in pixels. */
#define PROJECTILE_W 150
/** @brief Height of the projectile sprite in pixels. */
#define PROJECTILE_H 100
/** @brief X offset of the projectile tip for the blue team (right side). */
#define PROJECTILE_TIP_BLUE_X ((9 * PROJECTILE_W) / 10)
/** @brief X offset of the projectile tip for the red team (left side). */
#define PROJECTILE_TIP_RED_X (PROJECTILE_W / 10)
/** @brief Y offset of the projectile tip (vertical centre). */
#define PROJECTILE_TIP_Y (PROJECTILE_H / 2)
