#ifndef _PAINT_WINDOW_H_
#define _PAINT_WINDOW_H_

#include <stdint.h>

#include "../cursor.h"

/**
 * @defgroup Graphics Graphics
 * 
 * @brief Graphics module
 * 
 * This module includes general functions for painting the screen as well as more specific functions correspondent to different states of the game.
 * 
 * 
 * @{
 * 
 * @defgroup paint_window Paint Window
 * 
 * @brief Paint Window Module
 * 
 * This module provides functionality for painting windows and menus. It includes functions for painting images, menus, and various states of
 * painting application. The module also defines a color theme enum.
 * 
 * @{
*/

/**
 * @brief Paint an image on the window
 * 
 * Paints the image specified by the map array at the given coordinates on the window in the secondary buffer
 * 
 * @param map The image map
 * @param x The X-coordinate of the image
 * @param y The Y-coordinate of the image
 * 
 * @return Return 0 upon success, non-zero otherwise.
*/
int (paint_image)(char* map[], uint16_t x, uint16_t y);

/**
 * @brief Paint an image with a cursor on the window
 * 
 * Paints the image specified by the map array at the given coordinates on the window directly in the main buffer
 * 
 * @param map The image map
 * @param x The X-coordinate of the image
 * @param y The Y-coordinate of the image
 * 
 * @return Return 0 upon success, non-zero otherwise.
*/
int (paint_image_cursor)(char* map[], int16_t x, int16_t y);

/**
 * @brief Paint the main menu
 * 
 * Paints the main menu of the painting application
 * 
 * @return Return 0 upon success, non-zero otherwise.
*/
int paint_main_menu();

/**
 * @brief Paint the multiplayer menu
 * 
 * Paints the multiplayer menu of the painting application
 * 
 * @return Return 0 upon success, non-zero otherwise.
*/
int paint_multiplayer_menu();

/**
 * @brief Paint the guessing state
 * 
 * Paints the guessing button
 * 
 * @return Return 0 upon success, non-zero otherwise.
*/
int paint_guessing();

/**
 * @brief Paint the waiting state for drawing
 * 
 * Paints the waiting drawing button
 * 
 * @return Return 0 upon success, non-zero otherwise.
*/
int paint_waiting_drawing();

/**
 * @brief Paint the waiting state for guessing
 * 
 * Paints the waiting guessing button
 * 
 * @return Return 0 upon success, non-zero otherwise.
*/
int paint_waiting_guessing();

/**
 * @brief Color themes
 * 
 * This enum represents the available color themes
*/
typedef enum {
    LIGHTMODE,      /**< @brief Light Mode*/
    DARKMODE        /**< @brief Dark Mode*/
} color_theme_t;

/**
 *  }@
 * }@
*/

#endif
