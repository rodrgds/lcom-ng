#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>
#include <stddef.h>
#include <stdlib.h>
#include "sprite.h"
#include "spriteLoader.h"
#include "cursor.h"
#include "button.h"

/**
 * @file menu.h
 * @brief Menu management in the game
 * 
 * This module provides functions to create, manage, and draw the main menu of the game.
 * It handles the background, buttons, and input processing for starting the game or exiting.
 */

struct menu_imp;
typedef struct menu_imp Menu;

/**
 * @brief Creates a menu object.
 * 
 * @param loader Pointer to the SpriteLoader object used to load sprites.
 * @return Pointer to the created Menu object, or NULL on failure.
 */
Menu* create_menu(SpriteLoader *loader);

/** 
  * @brief Destroys a menu object and frees its resources.
  * 
  * @param m Pointer to the Menu object to be destroyed.
  */
void destroy_menu(Menu *m);

/**
 * @brief Draws the menu on the screen.
 * 
 * @param m Pointer to the Menu object to be drawn.
 */
void draw_menu(Menu *m);

/**
 * @brief Processes the input from the cursor and determines the action to take.
 * 
 * @param c Pointer to the Cursor object used for input.
 * @return 0 if no action is taken, 1 if the game should exit, or 2 if single mode is selected.
 */
int process_menu_mouse(Menu *m, Cursor *cursor);

#endif

