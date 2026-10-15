/**
 * @file menu.h
 * @brief Manages the main menu functionality of the Battleship game
 * 
 * This file contains declarations for functions and variables related to
 * the main menu screen, which serves as the starting point for the game.
 */

#ifndef MENU_H
#define MENU_H

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
#include "view/menu_view.h"

/// @brief Buffer used to store the main menu screen graphics
extern uint8_t *menu_buffer;

/**
 * @brief Allocates memory for the menu buffer if not already allocated
 * 
 * Checks if the menu_buffer is NULL and allocates memory equal to the
 * size of the frame buffer if needed.
 */
void set_menu_buffer();

/**
 * @brief Initializes the main menu screen
 * 
 * Allocates the buffer and draws the initial menu background.
 */
void init_menu();

/**
 * @brief Main loop for the Menu state
 * 
 * Draws the menu, cursor, and swaps buffers to display the menu screen.
 */
void menu_main_loop();

/**
 * @brief Handles keyboard input in the main menu screen
 * 
 * Processes keyboard input for the main menu. Currently handles:
 * - Enter key: Transition to the Start state
 */
void menu_keyboard_handler();

#endif
