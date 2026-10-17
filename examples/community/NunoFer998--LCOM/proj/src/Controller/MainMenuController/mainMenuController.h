/**
 * @file mainMenuController.h
 * @brief Header file for main menu controller functionality
 * 
 * This header defines the interface for handling main menu interactions,
 * including menu option enumeration, position constants, and function declarations
 * for processing keyboard input in the main menu state.
 */

#ifndef _MAINMENUCONTROLLER_H_
#define _MAINMENUCONTROLLER_H_

#include <lcom/lcf.h>
#include <stdio.h>

#include "Controller/Devices/VideoCard/video_graphics.h"
#include "../../Model/SelectorMine/selectorMine.h"
#include "../../View/GameOverView/gameOverView.h"
#include "../state.h"
#include "../scancodes.h"

/**
 * @brief Enumeration for main menu options
 */
typedef enum {
    PLAY,           // Start game option
    INSTRUCTIONS,   // View instructions option  
    EXIT            // Exit game option
} option_menu;

// Position constants for selector mine sprite
#define SEL_PLAY_X 505      // X position for PLAY option selector
#define SEL_PLAY_Y 250      // Y position for PLAY option selector
#define SEL_INST_X 607      // X position for INSTRUCTIONS option selector
#define SEL_INST_Y 350      // Y position for INSTRUCTIONS option selector
#define SEL_EXIT_X 496      // X position for EXIT option selector
#define SEL_EXIT_Y 450      // Y position for EXIT option selector

extern uint8_t scan_code;   // External keyboard scan code variable

/**
 * @brief Handles keyboard scan codes for main menu navigation
 * 
 * @param mine Pointer to the selector mine sprite for visual feedback
 * @param state Pointer to the current game state to be updated
 * @param stateChanged Pointer to flag indicating if state was changed
 */
void handleScanCodeMainMenu(SMine* mine, game_state* state, bool* stateChanged);

#endif
