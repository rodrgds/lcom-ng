/**
 * @file sunflower.h
 * @brief Logic and rendering for the sunflower plant.
 */

#ifndef __SUNFLOWER_H
#define __SUNFLOWER_H

#include <stdbool.h>

/**
 * @brief Initializes the sunflowers array and loads sprite.
 */
void sunflowers_init();

/**
 * @brief Places a sunflower at the specified grid position.
 * * @param row The row on the grid.
 * @param col The column on the grid.
 * @return true if successfully spawned, false if the maximum sunflowers limit was reached.
 */
bool sunflowers_spawn(int row, int col);

/**
 * @brief Updates the state and sun-generation logic of all active sunflowers.
 */
void sunflowers_update();

/**
 * @brief Renders all active sunflowers.
 */
void sunflowers_render();

/**
 * @brief Applies damage to a sunflower at a specific grid position.
 * * @param row The row on the grid.
 * @param col The column on the grid.
 * @param damage The amount of damage to apply.
 */
void sunflowers_take_damage(int row, int col, int damage);

#endif
