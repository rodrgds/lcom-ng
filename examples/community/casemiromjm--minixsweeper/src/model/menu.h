/**
 * @file menu.h
 * @brief Header file for the menu system, defining the Menu structure and related functions.
 */

#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdbool.h>
#include "gametypes.h"

#define BUTTON_WIDTH 200       /**< Width of the menu buttons. */
#define BUTTON_HEIGHT 50       /**< Height of the menu buttons. */
#define BUTTON_SPACING 20      /**< Vertical spacing between menu buttons. */
#define ARROW_WIDTH 50         /**< Width of the arrow buttons. */
#define ARROW_HEIGHT 50        /**< Height of the arrow buttons. */

/**
 * @brief Represents a clickable button in the menu.
 */
typedef struct {
    int x, y;                  /**< Position of the button (top-left corner). */
    int width, height;         /**< Dimensions of the button. */
    bool hovered;              /**< True if the mouse is currently hovering over the button. */
} Button;

/**
 * @brief Represents the full menu with all its interactive elements.
 */
typedef struct {
    Button start_button;              /**< Button to start the game. */
    Button mode_button;               /**< Button to select game mode. */
    Button left_mode_arrow;          /**< Left arrow for game mode selection. */
    Button right_mode_arrow;         /**< Right arrow for game mode selection. */
    Button difficulty_button;        /**< Button to select game difficulty. */
    Button left_difficulty_arrow;    /**< Left arrow for difficulty selection. */
    Button right_difficulty_arrow;   /**< Right arrow for difficulty selection. */
    Button exit_button;              /**< Button to exit the game. */
    GameDifficulty selected_difficulty; /**< Currently selected game difficulty. */
    GameMode selected_mode;          /**< Currently selected game mode. */
} Menu;

/**
 * @brief Global pointer to the main menu instance.
 */
extern Menu* menu;

/**
 * @brief Allocates and initializes a new Menu structure.
 * 
 * @return Pointer to the created Menu structure.
 */
Menu* (create_menu)();

/**
 * @brief Frees the memory associated with a Menu structure.
 * 
 * @param menu Pointer to the Menu to be destroyed.
 */
void (destroy_menu)(Menu* menu);

/**
 * @brief Resets all menu buttons' hovered states to false.
 * 
 * @param menu Pointer to the Menu structure.
 */
void (reset_menu_buttons)(Menu* menu);

#endif

