/**
 * @file game_state.h
 * @brief Global game state variables and macro definitions.
 */

#ifndef __GAME_STATE_H
#define __GAME_STATE_H

#include <stdint.h>
#include <stdbool.h>

/** @brief Shared global variable for the player's total suns. */
extern int suns_count;

/** @brief Shared global variable for the player's score. */
extern int score;

/** @brief Shared global variable for the current level. */
extern int current_level;

/** @brief Shared global variable indicating if the board is frozen. */
extern bool frozen;

/** @brief Shared global variable for the general timer counter. */
extern uint32_t timer_counter;

// grid macros //
#define GRID_START_X 0
#define GRID_START_Y 100
#define GRID_COLS    9
#define GRID_ROWS    5
#define CELL_W       89
#define CELL_H       100

#endif
