/**
 * @file state.h
 * @brief Game state enumeration definitions
 * 
 * This header file defines the various game states used throughout
 * the minesweeper application for state management and transitions.
 */

#ifndef _STATE_H_
#define _STATE_H_

/**
 * @brief Enumeration of possible game states
 * 
 * This enum defines all possible states that the game can be in,
 * allowing for proper state management and transitions.
 */
typedef enum {
    MAIN_MEU,   // Main menu state
    INST,       // Instructions screen state
    GAME,       // Active gameplay state
    GAME_OVER,  // Game over screen state
    WIN,        // Victory screen state
    QUIT        // Exit application state
} game_state;

#endif
