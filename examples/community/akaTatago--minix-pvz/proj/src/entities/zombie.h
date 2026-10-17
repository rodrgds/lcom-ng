/**
 * @file zombie.h
 * @brief Logic, movement, and rendering for zombies.
 */

#ifndef __ZOMBIE_H
#define __ZOMBIE_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_ZOMBIES 20

/**
 * @brief Structure defining zombie entity.
 */
typedef struct {
    bool active;        /**< @brief Indicates if the zombie is currently alive. */
    int16_t x;          /**< @brief Current x-coordinate. */
    uint8_t lane;       /**< @brief The lane (row) the zombie is on. */
    int16_t hp;         /**< @brief Current health points. */
    int16_t speed;      /**< @brief Movement speed. */
    bool eating;        /**< @brief Indicates if the zombie is currently eating a plant. */
    uint16_t eat_timer; /**< @brief Timer to manage damage application while eating plant. */
} zombie_t;

extern zombie_t zombies[MAX_ZOMBIES];

/**
 * @brief Initializes the zombies array and loads sprite.
 */
void zombies_init();

/**
 * @brief Spawns a zombie in a specific lane.
 * * @param lane The lane to spawn the zombie in.
 * @param level Current game level to adjust zombie difficulty.
 */
void spawn_zombie(uint8_t lane, int level);

/**
 * @brief Updates the movement, eating logic, and life cycle of zombies.
 * * @return true if a zombie reached the left border (game over), false otherwise.
 */
bool zombies_update(); 

/**
 * @brief Renders all active zombies.
 */
void zombies_render();

/**
 * @brief Gets the number of zombies currently active on the board.
 * * @return Integer representing the alive zombie count.
 */
int zombies_alive_count();

/**
 * @brief Checks if there is a zombie in a given lane ahead of a certain x-coordinate.
 * * @param lane The lane to check.
 * @param plant_x The reference x-coordinate (usually a plant's position).
 * @return true if a zombie is found ahead, false otherwise.
 */
bool zombie_in_lane_ahead(uint8_t lane, int16_t plant_x);

/**
 * @brief Checks if a projectile hits any zombie.
 * * @param lane The lane where the projectile is travelling.
 * @param proj_x The x-coordinate of the projectile.
 * @param damage The amount of damage to deal if a hit occurs.
 * @return true if a zombie was hit, false otherwise.
 */
bool zombies_check_hit(uint8_t lane, int16_t proj_x, int16_t damage);

#endif
