/**
 * @file kbd_handler.h
 * @brief Declares the keyboard event handler and keyboard selection state.
 *
 * This module provides the interface for handling raw keyboard scan codes
 * and updating the global keyboard-driven character selection state.
 */

#ifndef KBD_HANDLER_H
#define KBD_HANDLER_H

#include <lcom/lcf.h>
#include <stdio.h>
#include "controllers/keyboardMouse/keyboard.h"
#include "gameState.h"

extern int kbd_state;
/**
 * @brief Processes a keyboard scan code and updates game state.
 *
 * Interprets a one- or two-byte scan code array, determines make/break,
 * and updates the global `state` and `kbd_state` accordingly.
 *
 * @param scan_code Array of two bytes containing the raw scan code(s).
 */
void kbd_event_handler(uint8_t scan_code[2]);

#endif // KBD_HANDLER_H

