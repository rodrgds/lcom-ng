#ifndef NUT_H
#define NUT_H

#include <stdbool.h>
#include <stdint.h>

#define NUT_MAX_HP 4000

/**
 * @brief Initializes the nuts array, setting all instances to inactive.
 */
void nuts_init(void);

/**
 * @brief Spawns a nut at the specified board coordinates.
 * @param row The board row (typically 0-4)
 * @param col The board column (typically 0-8)
 * @return true if successfully placed, false if the maximum nuts limit was reached.
 */
bool nuts_spawn(int row, int col);

/**
 * @brief Updates all active nuts (used for animation frames or timers).
 */
void nuts_update(void);

/**
 * @brief Renders all active nuts to the screen.
 */
void nuts_render(void);

/**
 * @brief Applies damage to a nut at a specific grid position.
 * @param row The board row
 * @param col The board column
 * @param damage The amount of damage to subtract from the nut's HP
 */
void nuts_take_damage(int row, int col, int damage);

#endif // NUT_H