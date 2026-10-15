/**
 * @file config_state.h
 * @brief Fast Teclas game configuration screen.
 */
#ifndef _TECLAS_CONFIG_STATE_H_
#define _TECLAS_CONFIG_STATE_H_

#include <lcom/lcf.h>
#include <stdbool.h>

#include "../config.h"

/**
 * @brief Handle a keyboard event in the fast teclas config screen.
 *
 * @param sc Raw keyboard scancode.
 * @param cfg Game configuration to update.
 * @return true if the player confirmed and the game should start, false otherwise.
 */
bool config_state_handle_key(uint8_t sc, GameConfig *cfg);

/**
 * @brief Handle a mouse click in the fast teclas config screen.
 *
 * @param cfg Game configuration to update.
 * @return true if the player confirmed and the game should start, false otherwise.
 */
bool config_state_handle_click(GameConfig *cfg);

/**
 * @brief Draw the fast teclas configuration screen.
 *
 * @param cfg Current game configuration to display.
 */
void config_state_draw(const GameConfig *cfg);

#endif // _TECLAS_CONFIG_STATE_H_
