/**
 * @file game_over_view.h
 * @brief This file handles the drawing and rendering of the game over screen.
 * It includes functions for drawing the background, winner text, and menu options.
 */

#ifndef GAME_OVER_VIEW_H
#define GAME_OVER_VIEW_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/graphics.h"
#include "model/game_macro.h"
#include "model/game.h"
#include "model/sprite.h"
#include "model/game_over.h"
#include "view/sprite_view.h"

/** @def OPTION_MENU_Y
 * @brief Y position for the "Menu" option on the game over screen.
 */
#define OPTION_MENU_Y 340

/** @def OPTION_EXIT_Y
 * @brief Y position for the "Exit" option on the game over screen.
 */
#define OPTION_EXIT_Y 400

/** @def OPTIONS_X
 * @brief X position for both the "Menu" and "Exit" options on the game over screen.
 */
#define OPTIONS_X 300

/** @def WINNER_TEXT_Y
 * @brief Y position for displaying the winner text on the game over screen.
 */
#define WINNER_TEXT_Y 200

/**
 * @brief Draws the background of the game over screen.
 * @param buffer Pointer to the graphics buffer where the background will be drawn.
 */
void draw_game_over_background(uint8_t *buffer);

/**
 * @brief Draws the entire game over screen, including background, winner, and options.
 */
void draw_game_over_screen();

/**
 * @brief Draws the text indicating the winner of the game.
 * @param buffer Pointer to the graphics buffer where the winner text will be drawn.
 */
void draw_game_over_winner(uint8_t *buffer);

/**
 * @brief Draws the "Menu" and "Exit" options on the game over screen.
 * @param buffer Pointer to the graphics buffer where the options will be drawn.
 */
void draw_game_over_options(uint8_t *buffer);

#endif
