/**
 * @file mainMenuController.c
 * @brief Controller for handling main menu interactions
 * 
 * This file contains the main menu controller that handles keyboard input
 * and manages navigation through the main menu options (Play, Instructions, Exit).
 * It processes scan codes and updates the game state accordingly.
 */

#include "mainMenuController.h"

// Global variables for main menu state management
bool twoByteScan = false;   // Flag to track if a two-byte scan code is being processed
option_menu opt = PLAY;     // Current selected menu option, starts with PLAY

/**
 * @brief Handles keyboard scan codes for main menu navigation
 * 
 * This function processes keyboard input and updates the menu state accordingly.
 * It handles navigation between menu options (Play, Instructions, Exit) using
 * keyboard keys (W/S or arrow keys) and selection with Enter.
 * 
 * @param mine Pointer to the selector mine sprite for visual feedback
 * @param state Pointer to the current game state to be updated
 * @param stateChanged Pointer to flag indicating if state was changed
 */
void handleScanCodeMainMenu(SMine* mine, game_state* state, bool* stateChanged) {
    // Check if this is the first byte of a two-byte scan code
    if (scan_code == TWO_BYTES) {
        twoByteScan = true;
        return;
    }
    else {
        // Process the scan code based on current menu option
        switch (opt)
        {
        case PLAY:
            // Handle navigation from PLAY option
            if ((scan_code == M_S) || ((scan_code == M_DOWN) && twoByteScan)) {
                // Move down to INSTRUCTIONS option
                opt = INSTRUCTIONS;
                setSMinePosition(mine, SEL_INST_X, SEL_INST_Y);
            }
            else if (scan_code == B_ENTER) {
                // Enter pressed - start the game
                *state = GAME;
                *stateChanged = true;
                return;
            }
            break;        case INSTRUCTIONS:
            // Handle navigation from INSTRUCTIONS option
            if ((scan_code == M_S) || ((scan_code == M_DOWN) && twoByteScan)) {
                // Move down to EXIT option
                opt = EXIT;
                setSMinePosition(mine, SEL_EXIT_X, SEL_EXIT_Y);
            }
            else if ((scan_code == M_W) || ((scan_code == M_UP) && twoByteScan)) {
                // Move up to PLAY option
                opt = PLAY;
                setSMinePosition(mine, SEL_PLAY_X, SEL_PLAY_Y);
            }
            if (scan_code == B_ENTER) {
                // Enter pressed - go to instructions screen
                *state = INST;
                *stateChanged = true;
                return;
            }
            break;        case EXIT:
            // Handle navigation from EXIT option
            if ((scan_code == M_W) || ((scan_code == M_UP) && twoByteScan)) {
                // Move up to INSTRUCTIONS option
                opt = INSTRUCTIONS;
                setSMinePosition(mine, SEL_INST_X, SEL_INST_Y);
            }
            if (scan_code == B_ENTER) {
                // Enter pressed - quit the game
                *state = QUIT;
                *stateChanged = true;
                return;
            }
            break;        
        
        default:
            break;
        }
        // Reset two-byte scan flag after processing
        twoByteScan = false;
    }
}

