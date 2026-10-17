/**
 * @file board.h
 * @brief Grid and placement mechanics for the game board.
 */

#ifndef __BOARD_H
#define __BOARD_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Enum of available plant types.
 */
typedef enum { 
    PLANT_NONE = 0, 
    PLANT_SUNFLOWER = 1, 
    PLANT_PEASHOOTER = 2,
    PLANT_NUT = 3
} plant_type_t;

/**
 * @brief Initializes the empty board matrix.
 */
void board_init();

/**
 * @brief Checks if a specific cell on the board is empty.
 * * @param row The grid row.
 * @param col The grid column.
 * @return true if empty, false if occupied or out of bounds.
 */
bool board_is_empty(int row, int col);

/**
 * @brief Places a plant on a specified grid cell.
 * * @param row The grid row.
 * @param col The grid column.
 * @param type The type of plant to place.
 * @return true if successfully placed, false otherwise.
 */
bool board_place_plant(int row, int col, plant_type_t type);

/**
 * @brief Applies damage to a plant at the specified cell.
 * * @param row The grid row.
 * @param col The grid column.
 * @param damage The amount of damage to apply.
 */
void board_take_damage(int row, int col, int damage);

/**
 * @brief Gets the type of plant located at a specific grid cell.
 * * @param row The grid row.
 * @param col The grid column.
 * @return The plant tupe at that cell.
 */
plant_type_t board_get_plant_type(int row, int col);

/**
 * @brief Clears the specified grid cell completely.
 * * @param row The grid row.
 * @param col The grid column.
 */
void board_clear_cell(int row, int col);

#endif
