/**
 * @file levels.h
 * @brief This file contains the level drawings for the game. Each level is represented as a 2D array of colors. It also includes the function that loads a specific level into the grid.
 */

#ifndef _LEVELS_H_
#define _LEVELS_H_

#include <stdint.h>
#include "grid.h"

#define MAX_LEVELS 5

extern int current_level; 

extern const uint32_t level1[GRID_ROWS][GRID_COLS];
extern const uint32_t level2[GRID_ROWS][GRID_COLS];
extern const uint32_t level3[GRID_ROWS][GRID_COLS];
extern const uint32_t level4[GRID_ROWS][GRID_COLS];
extern const uint32_t level5[GRID_ROWS][GRID_COLS];

/**
 * @brief Loads a level into the original grid.
 * 
 * @param level_index The index of the level to be loaded (1 to 5).
 */
void load_level(int level_index);

#endif /* _LEVELS_H_ */
