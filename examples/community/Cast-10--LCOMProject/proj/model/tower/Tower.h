#pragma once

#include "../Position.h"
#include "../Box.h"
#include "../champion/Champion.h"
#include "../minion/Minion.h"
#include "TowerConfig.h"
#include <stdbool.h>

/**
 * @brief A projectile fired by a tower toward a target.
 */
typedef struct {
    Position pos; /**< Current world position of the projectile. */
    Minion *minion_target; /**< Minion target, or NULL when targeting a champion. */
    bool targeting_champion; /**< True when the projectile is homing on a champion. */
    bool active; /**< True while the projectile is in flight. */
    int damage; /**< Damage dealt on impact. */
} TowerProjectile;

/**
 * @brief A defensive tower belonging to one team.
 */
typedef struct {
    Position pos; /**< World position of the tower's top-left corner. */
    Team team; /**< Team that owns this tower. */
    int hp; /**< Current hit points. */
    int attack_timer; /**< Ticks remaining until the tower can fire again. */
    Minion *minion_target; /**< Current minion target, or NULL. */
    bool targeting_champion; /**< True when the tower is targeting a champion. */
    TowerProjectile projectile; /**< The tower's single in-flight projectile. */
} Tower;

/**
 * @brief Pair of towers, one per team.
 */
typedef struct {
    Tower blue; /**< Tower belonging to the blue team. */
    Tower red;  /**< Tower belonging to the red team. */
} Towers;

/**
 * @brief Initialises both towers and places them at the correct lane positions.
 * @param towers Towers container to initialise.
 * @param map_w Width of the map in pixels (used to position the red tower).
 */
void towersInit(Towers *towers, int map_w);

/**
 * @brief Returns the axis-aligned bounding box of a tower.
 * @param tower Target tower.
 * @return Bounding box covering the tower sprite.
 */
Box towerGetBox(Tower *tower);
