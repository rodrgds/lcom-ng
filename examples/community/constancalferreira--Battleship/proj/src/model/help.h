/**
 * @file help.h
 * @brief Manages the Help screen functionality of the Battleship game
 * 
 * This file contains declarations for functions and variables related to
 * the Help screen, which provides game instructions to the player.
 */

#ifndef HELP_H
#define HELP_H

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
#include "view/game_view.h"
#include "view/help_view.h"

/// @brief Buffer used to store the Help screen graphics
extern uint8_t *help_buffer;

/**
 * @brief Allocates memory for the help buffer if not already allocated
 * 
 * Checks if the help_buffer is NULL and allocates memory equal to the
 * size of the frame buffer if needed.
 */
void set_help_buffer();

/**
 * @brief Initializes the Help screen
 * 
 * Allocates the buffer and draws the initial Help screen content.
 */
void init_help();

/**
 * @brief Main loop for the Help state
 * 
 * Copies the help buffer to the current buffer, draws the cursor,
 * and swaps buffers to display the Help screen.
 */
void help_main_loop();

/**
 * @brief Handles keyboard input in the Help screen
 * 
 * Processes keyboard input for the Help screen. Currently handles:
 * - Q key: Return to previous state
 */
void help_keyboard_handler();

#endif
