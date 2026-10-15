#ifndef START_H
#define START_H

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
#include "view/start_view.h"

/// @brief Pointer to the start screen's video buffer
extern uint8_t *start_buffer;

/// @brief Options available on the start screen
typedef enum {
    GO_START,  ///< Proceed to start the game
    GO_RULES,  ///< Show the rules screen
    GO_HELP,   ///< Show the help screen
} StartOption;

/**
 * @brief Allocates memory for the start buffer if not already allocated.
 */
void set_start_buffer();

/**
 * @brief Initializes the start screen by setting the buffer and drawing it.
 */
void init_start();

/**
 * @brief Main loop logic for rendering the start screen.
 */
void start_main_loop();

/**
 * @brief Handles keyboard input on the start screen.
 */
void start_keyboard_handler();

/**
 * @brief Checks if the mouse is over the specified start option.
 *
 * @param option The start option to check
 * @return true if mouse is over the option, false otherwise
 */
bool is_mouse_over_start_option(StartOption option);

/**
 * @brief Handles mouse input on the start screen.
 */
void start_mouse_handler();

/**
 * @brief Retrieves the currently selected start option.
 *
 * @return The selected StartOption
 */
StartOption get_start_option();

/**
 * @brief Sets the current start option.
 *
 * @param option The option to set as selected
 */
void set_start_option(StartOption option);

#endif
