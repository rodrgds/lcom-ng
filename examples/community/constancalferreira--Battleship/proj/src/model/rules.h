/**
 * @file rules.h
 * @brief Manages the game rules screen functionality of the Battleship game
 * 
 * This file contains declarations for functions and variables related to
 * the rules screen, which displays the gameplay rules to the player.
 */

#ifndef RULES_H
#define RULES_H

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
#include "view/rules_view.h"

/// @brief Buffer used to store the rules screen graphics
extern uint8_t *rules_buffer;

/**
 * @brief Allocates memory for the rules buffer if not already allocated
 * 
 * Checks if the rules_buffer is NULL and allocates memory equal to the
 * size of the frame buffer if needed.
 */
void set_rules_buffer();

/**
 * @brief Initializes the rules screen
 * 
 * Allocates the buffer and draws the initial rules content.
 */
void init_rules();

/**
 * @brief Main loop for the Rules state
 * 
 * Copies the rules buffer to the current buffer, draws the cursor,
 * and swaps buffers to display the rules screen.
 */
void rules_main_loop();

/**
 * @brief Handles keyboard input in the rules screen
 * 
 * Processes keyboard input for the rules screen. Currently handles:
 * - Q key: Return to the previous state
 */
void rules_keyboard_handler();

#endif
