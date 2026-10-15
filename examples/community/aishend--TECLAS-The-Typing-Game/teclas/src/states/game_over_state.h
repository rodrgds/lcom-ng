/**
 * @file game_over_state.h
 * @brief Game over screen after a typing session.
 */
#ifndef _TECLAS_GAME_OVER_STATE_H_
#define _TECLAS_GAME_OVER_STATE_H_

#include "../config.h"
#include "../game_results.h"

/**
 * @brief Initialize the game over screen with the final results.
 *
 * @param results Statistics from the finished game.
 * @param cfg Game configuration used for theme and player name.
 */
void game_over_state_init(GameResults results, GameConfig cfg);

/**
 * @brief Draw the game over screen.
 */
void game_over_state_draw();

#endif /* _TECLAS_GAME_OVER_STATE_H_ */
