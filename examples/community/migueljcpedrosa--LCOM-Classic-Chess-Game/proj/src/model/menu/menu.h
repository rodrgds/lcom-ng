/**
 * @file menu
 * @brief This file has the implementation of the functions related to the initial menu.
 */

#ifndef _MENU_H_
#define _MENU_H_

#include <lcom/lcf.h>
#include "../../view/sprite.h"
#include "model/cursor/cursor_input.h"
#include "model/model_utils.h"

/**
 * @brief Struct representing the Menu state.
 *
 * This struct includes the title, two buttons (play and exit), and a background sprite.
 */
typedef struct {

  Title title;
  Button playButton;
  Button exitButton;
  Sprite* background;

} Menu;

extern Menu* menu;

/**
 * @brief Creates the initial menu.
 *
 * This function initializes the menu with a title, two buttons (play and exit) and a background sprite.
 * The positions of the title and buttons, and the pointers to their sprites are set.
 * 
 * @return void.
 */
void create_menu();

/**
 * @brief Destroys the initial menu.
 *
 * This function frees the memory allocated for the menu state and its associated elements.
 * 
 * @return void.
 */
void destroy_menu();

/**
 * @brief Checks if the play button in the initial menu has been clicked.
 * 
 * @param cursor CursorInput struct that contains the current cursor input.
 *
 * This function checks if the current cursor position is within the bounds of the play button in the menu.
 * 
 * @return true if the play button was clicked, false otherwise.
 */
bool menu_clicked_play(CursorInput* cursor);

/**
 * @brief Checks if the exit button in the initial menu has been clicked.
 * 
 * @param cursor CursorInput struct that contains the current cursor input.
 *
 * This function checks if the current cursor position is within the bounds of the exit button in the menu.
 * 
 * @return true if the exit button was clicked, false otherwise.
 */
bool menu_clicked_exit(CursorInput* cursor);

#endif
