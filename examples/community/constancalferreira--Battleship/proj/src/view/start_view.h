/**
 * @file start_view.h
 * @brief Handles the visual representation of the start menu screen
 * 
 * This file contains declarations for functions related to rendering
 * the start menu screen, where players can choose to start the game,
 * view rules, or access help information.
 */

#ifndef START_VIEW_H
#define START_VIEW_H


#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "controller/graphics.h"

#include "model/game_macro.h"
#include "model/game.h"
#include "model/sprite.h"
#include "model/start.h"
#include "view/sprite_view.h"


/// @brief X position for all menu options
#define OPTIONS_X         300   

/// @brief Y position for "Start" option
#define OPTION_START_Y    280   

/// @brief Y position for "Rules" option
#define OPTION_RULES_Y    350   

/// @brief Y position for "Help" option
#define OPTION_HELP_Y     420   

/**
 * @brief Draws the start screen background
 * 
 * Fills the screen with a teal background color and renders the game logo
 * at the top of the screen.
 *
 * @param buffer Pointer to the graphics buffer where the background will be drawn
 */
void draw_start_background(uint8_t *buffer);

/**
 * @brief Draws the complete start screen
 * 
 * Copies the static start background to the current buffer and adds
 * the interactive menu options with their current selection state.
 */
void draw_start();

/**
 * @brief Draws the selectable options on the start screen
 * 
 * Renders each option (Start, Rules, Help) with appropriate highlighting
 * based on the currently selected option. Also positions a ship sprite
 * next to the selected option as a visual indicator.
 *
 * @param buffer Pointer to the graphics buffer where the options will be drawn
 */
void draw_start_options(uint8_t *buffer);






#endif 
