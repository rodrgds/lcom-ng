/**
 * @file gameOverController.h
 * @brief Game over controller header file
 * 
 * This header file defines the game over screen controller interface,
 * including menu options enumeration and functions for handling
 * keyboard input in the game over state.
 */

#ifndef _GAMEOVERCONTROLLER_H_
#define _GAMEOVERCONTROLLER_H_

#include <lcom/lcf.h>
#include <stdio.h>

#include "Controller/Devices/VideoCard/video_graphics.h"
#include "../../Model/SelectorMine/selectorMine.h"
#include "../state.h"
#include "../scancodes.h"

/**
 * @brief Enumeration for game over screen options
 */
typedef enum {
    RETRY,      // Retry game option
    EXIT_OVER   // Exit game option
} option_game_over;

// Position constants for selector mine sprite in game over screen
#define SEL_RETRY_X 527         // X position for RETRY option selector
#define SEL_RETRY_Y 240         // Y position for RETRY option selector
#define SEL_OVER_EXIT_X 496     // X position for EXIT option selector
#define SEL_OVER_EXIT_Y 440     // Y position for EXIT option selector

extern uint8_t scan_code;       // External keyboard scan code variable

/**
 * @brief Handles keyboard scan codes for game over screen navigation
 * 
 * This function processes keyboard input in the game over state,
 * allowing navigation between retry and exit options.
 * 
 * @param mine Pointer to the selector mine sprite for visual feedback
 * @param state Pointer to the current game state to be updated
 * @param stateChanged Pointer to flag indicating if state was changed
 */
void handleScanCodeGameOver(SMine* mine, game_state* state, bool* stateChanged);

#endif
