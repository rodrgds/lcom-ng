#ifndef GAME_OVER_H
#define GAME_OVER_H

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

/**
 * @enum GameOverOption
 * @brief Represents options available on the Game Over screen.
 */
typedef enum {
  GO_MENU = 0, ///< Option to return to the main menu
  GO_EXIT = 1  ///< Option to exit the game
} GameOverOption;

/// @brief Buffer to store the Game Over screen graphics
extern uint8_t *game_over_buffer;

/**
 * @brief Allocates memory for the game over buffer if not already allocated.
 */
void set_game_over_buffer();

/**
 * @brief Initializes the Game Over screen by setting the buffer and drawing the background.
 */
void init_game_over();

/**
 * @brief Main loop for the Game Over state. Draws the screen and handles buffer swapping.
 */
void game_over_main_loop();

/**
 * @brief Handles keyboard input for the Game Over screen.
 *
 * Responds to UP, DOWN, and ENTER keys to select and confirm options.
 */
void game_over_keyboard_handler();

/**
 * @brief Handles mouse input for the Game Over screen.
 *
 * Detects hovering over options and clicks to change game state.
 */
void game_over_mouse_handler();

/**
 * @brief Checks if the mouse cursor is over the specified Game Over option.
 *
 * @param option The GameOverOption to check (GO_MENU or GO_EXIT).
 * @return true if the cursor is over the option, false otherwise.
 */
bool is_mouse_over_option(GameOverOption option);

/**
 * @brief Returns the currently selected Game Over option.
 *
 * @return The current GameOverOption.
 */
GameOverOption get_game_over_option();

/**
 * @brief Sets the current Game Over option.
 *
 * @param option The GameOverOption to set as selected.
 */
void set_game_over_option(GameOverOption option);

#endif // GAME_OVER_H
