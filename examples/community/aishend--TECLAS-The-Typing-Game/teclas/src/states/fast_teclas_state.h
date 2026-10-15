/**
 * @file fast_teclas_state.h
 * @brief Fast Teclas typing game state.
 */
#ifndef _TECLAS_FAST_TECLAS_STATE_H_
#define _TECLAS_FAST_TECLAS_STATE_H_

#include "../config.h"
#include "../game_results.h"
#include <stdbool.h>

/**
 * @brief Initialize the fast teclas game state.
 *
 * @return 0 on success, non-zero on failure.
 */
int fast_teclas_state_init();

/**
 * @brief Reset the fast teclas game state for a new round.
 *
 * @param cfg Game configuration containing theme, time limit, and player name.
 */
void fast_teclas_state_reset(GameConfig cfg);

/**
 * @brief Draw the fast teclas game screen.
 */
void fast_teclas_state_draw();

/**
 * @brief Advance the game logic by one tick.
 *
 * @param cfg Game configuration used for theme and timing.
 * @return true if the game has ended, false otherwise.
 */
bool fast_teclas_state_tick(GameConfig cfg);

/**
 * @brief Handle a keyboard scancode event.
 *
 * @param scancode Raw keyboard scancode to process.
 */
void fast_teclas_state_handle_key(uint8_t scancode);

/**
 * @brief Handle a mouse click event.
 *
 * @return true if the click triggered a state transition, false otherwise.
 */
bool fast_teclas_state_handle_click();

/**
 * @brief Compute and return the results of the finished game.
 *
 * @return GameResults struct with WPM, accuracy, and error statistics.
 */
GameResults fast_teclas_state_compute_results();

#endif
