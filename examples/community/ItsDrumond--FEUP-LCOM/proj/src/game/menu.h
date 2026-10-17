/**
 * @file menu.h
 * 
 */

#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "devices/keyboard.h"
#include "devices/values.h"

typedef enum {
    MAINMENU,
    INSTRUCTIONS,
    GAME,
    GAMEOVER
} GameState;

/**
 * @brief This method handles mouse events for the current game state
 * 
 * @param mouse Pointer to the mouse
 * @param pp Pointer to the packet
 */
void menu_mouse(Mouse *mouse, struct packet *pp);

/**
 * @brief This method handles mouse input for the main menu
 * 
 * @param mouse Pointer to the mouse
 * @param pp Pointer to the packet
 */
void main_menu_mouse(Mouse *mouse, struct packet *pp);

/**
 * @brief This method checks if the left button of the mouse was clicked inside the menu area to set the game state to MAINMENU
 * 
 * @param mouse Pointer to the mouse
 * @param pp Pointer to the packet
 */
void instructions_mouse(Mouse *mouse, struct packet *pp);

/**
 * @brief This method changes the game state to MAINMENU when "Play Again" is pressed
 * 
 * @param mouse Pointer to the mouse
 * @param pp Pointer to the packet
 */
void (game_over_mouse)(Mouse *mouse, struct packet *pp);

/**
 * @brief This method restores the game to its default settings
 * 
 */
void restore_game_def();

#endif // _LCOM_MENU_H_
