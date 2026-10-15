/**
 * @file dino.h
 * @brief Dino mini-game state interface.
 */
#ifndef _TECLAS_DINO_H_
#define _TECLAS_DINO_H_

#include <lcom/lcf.h>
#include <stdbool.h>

/**
 * @brief Initialize the dino mini-game.
 *
 * @return 0 on success, non-zero on failure.
 */
int dino_game_init();

/**
 * @brief Reset the dino game to its initial state.
 */
void dino_game_reset();

/**
 * @brief Draw the dino game screen.
 */
void dino_game_draw();

/**
 * @brief Advance the dino game logic by one tick.
 */
void dino_game_tick();

/**
 * @brief Handle a keyboard scancode event.
 *
 * @param scancode Raw keyboard scancode to process.
 */
void dino_game_handle_key(uint8_t scancode);

/**
 * @brief Handle a mouse click event.
 *
 * @return true if the click triggered a state transition, false otherwise.
 */
bool dino_game_handle_click();

/**
 * @brief Check whether the dino game has ended.
 *
 * @return true if the game is over, false otherwise.
 */
bool dino_game_is_over();

/**
 * @brief Free all resources used by the dino game.
 */
void dino_game_destroy();

#endif
