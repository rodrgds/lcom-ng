/**
 * @file mouse_handler.h
 * @brief Function declaration for handling mouse events during gameplay.
 */

#ifndef MOUSE_HANDLER_H
#define MOUSE_HANDLER_H

#include <stdio.h>
#include "controllers/keyboardMouse/mouse.h"
#include "gameState.h"

/**
 * @brief Handles mouse events such as movement and button clicks.
 *
 * Updates the game state and triggers actions (like spawning characters or navigating menus)
 * based on the mouse input and current game state.
 *
 * @param pp The mouse packet containing movement deltas and button states.
 */
void mouse_event_handler(struct packet pp);

#endif // MOUSE_HANDLER_H

