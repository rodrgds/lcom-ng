#ifndef __PROJ_EV_H
#define __PROJ_EV_H

#include "controller/input/input.h"
#include "controller/kbc/mouse.h"
#include "model/game/combat.h"
#include "model/game/game.h"
#include "model/game/movement.h"

/** @defgroup ev ev
 * @{
 *
 * Module responsible for event handling in the game
 */

/**
 * @brief Handles timer events
 *
 * @param game Pointer to the game instance
 * @param counter The timer counter
 *
 * @return 0 upon success, non-zero otherwise
 */
int(handle_timer_event)(Game *game, uint32_t counter);

/**
 * @brief Handles keyboard events
 *
 * Processes a Key event and updates the game state or player position accordingly.
 *
 * @param game Pointer to the game instance
 * @param key Key from the input module
 *
 * @return 0 upon success, non-zero otherwise
 */
int(handle_kbd_event)(Game *game, Key key);

/**
 * @brief Handles mouse events
 *
 * Processes mouse position and button events and updates the game accordingly
 * based on the current game state.
 *
 * @param game Pointer to the game instance
 * @param mouse_info Mouse position and button states
 *
 * @return 0 upon success, non-zero otherwise
 */
int(handle_mouse_event)(Game *game, mouse_info_t mouse_info);

/**
 * @brief Handles RTC events
 *
 * @param game Pointer to the game instance
 *
 * @return 0 upon success, non-zero otherwise
 */
int(handle_rtc_event)(Game *game);

/**@}*/

#endif /* __PROJ_EV_H */
