#ifndef GAME_H
#define GAME_H

/**
 * @file game.h
 * @brief Main game logic (initialization, timing, game end, etc.).
 */

#include <lcom/lcf.h>
#include "../view/graphics.h"
#include "gametypes.h"
#include "map.h"
#include "../view/view.h"
#include "../rtc.h"
#include <minix/syslib.h>

/**
 * @brief Structure holding the current real-time clock info.
 */
real_time_info time_info; 

/// Elapsed time in seconds since the game started.
extern int elapsed_seconds;

/// Timestamp in seconds (from midnight) when the game started.
extern int game_start_seconds;

/// Timestamp in seconds (from midnight) when the game ended.
extern int game_end_seconds;

/**
 * @enum GameState
 * @brief Possible states of the game.
 */
typedef enum GameState {
    GAME_STATE_MENU,    ///< In the main menu
    GAME_STATE_RUNNING, ///< Game is running
    GAME_STATE_OVER,    ///< Game ended with failure
    GAME_STATE_WIN      ///< Game ended with success
} GameState;

/**
 * @brief Initializes the game with the specified difficulty.
 * 
 * @param difficulty The chosen difficulty level.
 */
void (init_game)(GameDifficulty difficulty);

/**
 * @brief Sets rows, columns and bombs based on the difficulty level.
 * 
 * @param difficulty Difficulty level.
 * @param rows Pointer to store number of rows.
 * @param cols Pointer to store number of columns.
 * @param bombs Pointer to store number of bombs.
 */
void (select_difficulty)(GameDifficulty difficulty, unsigned int *rows, unsigned int *cols, unsigned int *bombs);

/**
 * @brief Called when the game ends to update elapsed time.
 */
void on_game_end(void);

/**
 * @brief Returns elapsed game time in seconds.
 * 
 * @return Elapsed seconds since game started.
 */
int get_elapsed_seconds(void);

/**
 * @brief Sets elapsed time manually.
 * 
 * @param seconds Number of seconds to set.
 */
void set_elapsed_seconds(int seconds);

#endif

