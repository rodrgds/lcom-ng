#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>
#include <lcom/xpm.h> // Likely used for loading XPM images for menu assets

/**
 * @file menu.h
 * @brief Defines coordinates for menu buttons and declares functions for loading and drawing menu and instructions screens.
 *
 * This file contains macros for the bounding boxes of interactive menu elements (buttons)
 * and function prototypes for managing the visual assets and rendering of different menu states.
 */

/** @defgroup menu Menu System
 * @{
 *
 * Definitions and functions related to the game's menu system, including button coordinates
 * for interaction and functions to load and display menu screens and instructions.
 */

/* Play Button Coordinates */
#define PLAY_BTN_X1  340    /**< @brief X-coordinate of the top-left corner of the Play button's clickable area. */
#define PLAY_BTN_X2  485    /**< @brief X-coordinate of the bottom-right corner of the Play button's clickable area. */
#define PLAY_BTN_Y1  715    /**< @brief Y-coordinate of the top-left corner of the Play button's clickable area. */
#define PLAY_BTN_Y2  890    /**< @brief Y-coordinate of the bottom-right corner of the Play button's clickable area. */

/* Instructions Button Coordinates */
#define INSTR_BTN_X1  565   /**< @brief X-coordinate of the top-left corner of the Instructions button's clickable area. */
#define INSTR_BTN_X2  710   /**< @brief X-coordinate of the bottom-right corner of the Instructions button's clickable area. */
#define INSTR_BTN_Y1  715   /**< @brief Y-coordinate of the top-left corner of the Instructions button's clickable area. */
#define INSTR_BTN_Y2  890   /**< @brief Y-coordinate of the bottom-right corner of the Instructions button's clickable area. */

/* Back Button Coordinates */
#define BACK_BTN_X1  160    /**< @brief X-coordinate of the top-left corner of the Back button's clickable area (e.g., in instructions screen). */
#define BACK_BTN_X2  260    /**< @brief X-coordinate of the bottom-right corner of the Back button's clickable area. */
#define BACK_BTN_Y1  80     /**< @brief Y-coordinate of the top-left corner of the Back button's clickable area. */
#define BACK_BTN_Y2  140    /**< @brief Y-coordinate of the bottom-right corner of the Back button's clickable area. */

/* Exit Button Coordinates */
#define EXIT_BTN_X1  790    /**< @brief X-coordinate of the top-left corner of the Exit button's clickable area. */
#define EXIT_BTN_X2  935    /**< @brief X-coordinate of the bottom-right corner of the Exit button's clickable area. */
#define EXIT_BTN_Y1  715    /**< @brief Y-coordinate of the top-left corner of the Exit button's clickable area. */
#define EXIT_BTN_Y2  890    /**< @brief Y-coordinate of the bottom-right corner of the Exit button's clickable area. */

/**
 * @brief Loads all necessary visual assets for the main menu screen.
 *
 * This function is responsible for loading images, fonts, or any other
 * resources required to display the main menu. These assets are typically
 * loaded into memory once, for example, at the start of the menu state.
 */
void load_menu_assets(void);

/**
 * @brief Draws the main menu screen.
 *
 * This function renders the main menu onto the screen buffer using the assets
 * previously loaded by `load_menu_assets()`. This would include drawing
 * the background, buttons (Play, Instructions, Exit), and any other menu elements.
 */
void draw_menu(void);

/**
 * @brief Loads all necessary visual assets for the instructions screen.
 *
 * Similar to `load_menu_assets()`, this function loads images, text, or other
 * resources required to display the game instructions screen.
 */
void load_instructions_assets(void);

/**
 * @brief Draws the instructions screen.
 *
 * This function renders the instructions screen onto the screen buffer using assets
 * previously loaded by `load_instructions_assets()`. This would include
 * the instructions text/image and a back button.
 */
void draw_instructions(void);

/**@}*/

#endif /* MENU_H */
