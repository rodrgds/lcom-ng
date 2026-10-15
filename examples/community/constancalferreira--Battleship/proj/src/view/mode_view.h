/**
 * @file mode_view.h
 * @brief Handles the visual representation of the game mode selection screen
 * 
 * This file contains declarations for functions related to rendering
 * the mode selection screen, where players can choose between
 * single-player and multiplayer gameplay options.
 */

#ifndef MODE_VIEW_H
#define MODE_VIEW_H


#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "controller/graphics.h"

#include "model/game_macro.h"
#include "model/game.h"
#include "model/sprite.h"

#include "model/mode.h"


/**
 * @brief Draws the game mode selection screen
 * 
 * Fills the screen with a background color and renders the mode selection
 * interface, including the title and the available game mode options
 * (single player and multiplayer). The sprites are positioned at predefined
 * locations on the screen.
 *
 * @param buffer Pointer to the graphics buffer where the mode selection screen will be drawn
 */
void draw_mode(uint8_t *buffer);






#endif
