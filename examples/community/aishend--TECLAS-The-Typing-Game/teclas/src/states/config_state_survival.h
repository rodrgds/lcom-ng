/**
 * @file config_state_survival.h
 * @brief Survival mode configuration screen.
 */
#ifndef _SURVIVAL_CONFIG_STATE_H_
#define _SURVIVAL_CONFIG_STATE_H_

#include <lcom/lcf.h>
#include <stdbool.h>

#include "../config.h"

/**
 * @brief Handle a keyboard event in the survival config screen.
 *
 * @param sc  Raw keyboard scancode.
 * @param cfg Game configuration to update.
 * @return true if the player confirmed and the game should start, false otherwise.
 */
bool config_state_survival_handle_key(uint8_t sc, GameConfig *cfg);

/**
 * @brief Handle a mouse click in the survival config screen.
 *
 * @param cfg Game configuration to update.
 * @return true if the player confirmed and the game should start, false otherwise.
 */
bool config_state_survival_handle_click(GameConfig *cfg);

/**
 * @brief Draw the survival mode configuration screen.
 *
 * @param cfg Current game configuration to display.
 */
void config_state_survival_draw(const GameConfig *cfg);

#endif // _SURVIVAL_CONFIG_STATE_H_
