/**
 * @file mode.h
 * @brief Manages the game mode selection functionality of the Battleship game
 * 
 * This file contains declarations for functions and variables related to
 * the mode selection screen, where players can choose between single-player
 * and multiplayer gameplay.
 */

#ifndef MODE_H
#define MODE_H

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "controller/graphics.h"
#include "model/game_macro.h"
#include "model/game.h"
#include "view/mode_view.h"

/// @brief Currently selected game mode (single player or multiplayer)
extern GameMode mode;

/**
 * @brief Allocates memory for the mode buffer if not already allocated
 * 
 * Checks if the mode_buffer is NULL and allocates memory equal to the
 * size of the frame buffer if needed.
 */
void set_mode_buffer();

/**
 * @brief Initializes the mode selection screen
 * 
 * Allocates the buffer and draws the initial mode selection interface.
 */
void init_mode();

/**
 * @brief Main loop for the Mode selection state
 * 
 * Copies the mode buffer to the current buffer, draws the cursor,
 * and swaps buffers to display the mode selection screen.
 */
void mode_main_loop();

/**
 * @brief Handles keyboard input in the mode selection screen
 * 
 * Processes keyboard input for mode selection:
 * - 1 key: Select single-player mode and transition to Arena
 * - 2 key: Select multiplayer mode and transition to Arena
 */
void mode_keyboard_handler();

#endif
