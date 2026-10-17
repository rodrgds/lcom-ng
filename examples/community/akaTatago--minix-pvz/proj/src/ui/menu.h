/**
 * @file menu.h
 * @brief Functions and constants for the main menu and level selection screen.
 */

#ifndef __MENU_H
#define __MENU_H

#include <stdint.h>
#include <stdbool.h>
#include <lcom/lcf.h>

/**
 * @defgroup Menu Menu
 * @brief Menu rendering and event handling.
 * @{
 */

 /** @brief Size of the level selection square buttons (width and height in pixels). */
#define SQ_SIZE 60
/** @brief Gap in pixels between each level selection square. */
#define SQ_GAP 20
/** @brief The starting X coordinate for drawing the level selection squares. */
#define SQ_START_X 210
/** @brief The starting Y coordinate for drawing the level selection squares. */
#define SQ_START_Y 200

/**
 * @brief Initializes the menu.
 * * Resets menu state variables (like previous left-button click state) 
 * needed before the menu is first interacted with.
 */
void menu_init();
/**
 * @brief Renders the main menu to the screen.
 * * Draws the title and the Play, Levels, Scores, and Exit buttons. 
 * Highlights buttons when the mouse hovers over them.
 * * @param mouse_x The current X coordinate of the mouse cursor.
 * @param mouse_y The current Y coordinate of the mouse cursor.
 */
void menu_render(int mouse_x, int mouse_y);
/**
 * @brief Handles mouse events within the main menu.
 * * Processes mouse clicks to interact with menu buttons, changing the game 
 * state to NAME_INPUT, LEVEL_SELECT, SCOREBOARD, or quitting the game.
 * * @param pp Pointer to the mouse packet containing button states.
 * @param mouse_x The current X coordinate of the mouse cursor.
 * @param mouse_y The current Y coordinate of the mouse cursor.
 */
void menu_on_mouse(struct packet *pp, int mouse_x, int mouse_y);
/**
 * @brief Handles keyboard events within the menu.
 * * Processes keyboard inputs, such as pressing 'ESC' to quit the game.
 * * @param scancode The makecode or breakcode of the pressed/released key.
 */
void menu_on_key(uint8_t scancode);

/**
 * @brief Renders the level selection screen.
 * * Draws a grid of 10 level squares and a "BACK" button, updating their 
 * colors based on the mouse hover state.
 * * @param mouse_x The current X coordinate of the mouse cursor.
 * @param mouse_y The current Y coordinate of the mouse cursor.
 */
void level_select_render(int mouse_x, int mouse_y);

#endif
