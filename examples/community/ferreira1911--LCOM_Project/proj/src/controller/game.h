#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <lcom/lcf.h>

#define VBE_GAME_MODE 0x115 /**< @brief 800x600 - Direct Color - 24 (8:8:8) */
#define GAME_MODE_1_DURATION 30 /**< @brief Duration of game mode 1 in seconds */

/**
 * @brief Main controller function for the game.
 * 
 * Initializes the frame buffer, enters the main loop alternating between the menu and gameplay.
 * 
 * @return 0 on success, 1 on failure.
 */
int (game_controller)();

/**
 * @brief Initializes game resources and resets state for a new session.
 * 
 * Resets counters, initializes targets and crosshair based on the game mode.
 * 
 * @return 0 on success, 1 on failure.
 */
int (game_init)();

/**
 * @brief Main game loop.
 * 
 * Handles timer, keyboard, and mouse interrupts. Updates the game state, renders elements,
 * and checks for timeout or exit conditions.
 * 
 * @return 0 on success, 1 on failure.
 */
int (game_loop)();

/**
 * @brief Cleans up and exits the game.
 * 
 * Displays game statistics and frees video memory.
 * 
 * @return 0 on success, 1 on failure.
 */
int (game_exit)();

/**
 * @brief Draws all current game elements to the screen.
 * 
 * Includes targets, timer, hit/fail counters, crosshair, and optional warning messages.
 * 
 * @return 0 on success, 1 on failure.
 */
int (draw_game_elements)();

#endif
