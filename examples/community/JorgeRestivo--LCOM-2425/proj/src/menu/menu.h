/**
 * @file menu.h
 * @brief Functions for drawing and running the game menu and instructions screens.
 */

#ifndef MENU_H
#define MENU_H

#include "game.h" 
#include "gamestate.h" 
#include "keyboard.h"
#include "graphics.h" 
#include "sprites.h"
#include <lcom/lcf.h>

/**
 * @brief Runs the main menu loop.
 * @param game Pointer to the Game structure.
 * @param kb_mask Keyboard interrupt mask.
 * @return 0 on success, non-zero otherwise.
 */
int run_menu(Game *game, uint8_t kb_mask);

/**
 * @brief Draws the menu with the given selection and images.
 * @param selected Index of the currently selected menu option.
 * @param imgs Array of menu option images.
 * @param bg_img Pointer to the background image.
 */
void draw_menu(int selected, xpm_image_t imgs[4], xpm_image_t *bg_img);

/**
 * @brief Draws the instructions screen and waits for a key press to return.
 * @param game Pointer to the Game structure.
 * @param kb_mask Keyboard interrupt mask.
 */
void draw_instructions(Game *game, uint8_t kb_mask);

/**
 * @brief Background image for the menu.
 */
static xpm_image_t bg_img;

#endif
