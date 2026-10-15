/**
 * @file survival.h
 * @brief Survival typing mode state.
 */
#ifndef _SURVIVAL_H_
#define _SURVIVAL_H_

#include "../config.h"
#include <stdbool.h>

/**
 * @brief Initialize the survival mode.
 *
 * @return 0 on success, non-zero on failure.
 */
int survival_init();

/**
 * @brief Reset the survival mode to its initial state.
 */
void survival_reset();

/**
 * @brief Draw the survival mode screen.
 */
void survival_draw();

/**
 * @brief Advance the survival game logic by one tick.
 */
void survival_tick();

/**
 * @brief Handle a keyboard scancode event.
 *
 * @param scancode Raw keyboard scancode to process.
 */
void survival_handle_key(uint8_t scancode);

/**
 * @brief Handle a mouse click event.
 *
 * @return true if the click triggered a state transition, false otherwise.
 */
bool survival_handle_click();

/**
 * @brief Check whether the survival game has ended.
 *
 * @return true if the game is over, false otherwise.
 */
bool survival_is_done();

/**
 * @brief Free all resources used by the survival mode.
 */
void survival_destroy();

/**
 * @brief Get the current survival score.
 *
 * @return Number of survival points accumulated.
 */
int survival_get_survival_points();

/**
 * @brief Set the game configuration for the survival mode.
 *
 * @param cfg Game configuration containing theme and player name.
 */
void survival_set_config(GameConfig cfg);

#endif
