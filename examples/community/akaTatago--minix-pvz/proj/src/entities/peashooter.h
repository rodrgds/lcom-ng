/**
 * @file peashooter.h
 * @brief Logic and rendering peashooter plant.
 */

#ifndef __PEASHOOTER_H
#define __PEASHOOTER_H

#include <stdbool.h>

/**
 * @brief Initializes peashooters array and loads sprites.
 */
void peashooters_init();

/**
 * @brief Places a peashooter at the specified grid position.
 * * @param row The row on the grid.
 * @param col The column on the grid.
 * @return true if successfully placed, false if the maximum peashooters limit was reached.
 */
bool peashooters_spawn(int row, int col);

/**
 * @brief Updates the state and shooting logic of all active peashooters.
 */
void peashooters_update();

/**
 * @brief Renders all active peashooters on the screen.
 */
void peashooters_render();

/**
 * @brief Applies damage to a peashooter at the given grid position.
 * * @param row The row on the grid.
 * @param col The column on the grid.
 * @param damage The amount of damage to apply.
 */
void peashooters_take_damage(int row, int col, int damage);

#endif
