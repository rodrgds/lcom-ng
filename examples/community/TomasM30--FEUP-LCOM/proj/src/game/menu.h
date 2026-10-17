#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>
#include "game.h"
#include "../sprites/sprite.h"

/** @defgroup menu menu
 *  @brief Functions for drawing the menu
*/

extern Sprite* logo;
extern Sprite* single_machine;
extern Sprite* quit;
extern Sprite* return_button;
extern Sprite* undo;

extern GameState state; /*!< State of the game*/

static int menu_index = 0; /*!< Index of the menu option*/

/**
 * @brief Handles the menu logic for the mouse actions
*/
void menu_handle_mouse();

/**
 * @brief Handles the menu logic for the keyboard actions
 * @param scancode Scancode of the last key pressed
*/
void menu_handle_keyboard(uint8_t scancode);

/**
 * @brief Draws the menu
 * @return 0 upon success, 1 otherwise
*/
int draw_menu();

/**
 * @brief Draws the main menu
 * @return 0 upon success, 1 otherwise
*/
int draw_main_menu();

/**
 * @brief Draws the game menu
 * @return 0 upon success, 1 otherwise
*/
int draw_game_menu();

#endif
