#ifndef __MENU_OPTION_H_
#define __MENU_OPTION_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "video/video.h"
#include "sprites/animSprite.h"

/**
 * @defgroup menu_option Menu Option
 * @{
 *
 * Functions and structures for managing menu options
 */


/**
 * @brief Structure representing a menu option
 */
typedef struct MenuOption {
    uint32_t x, y; /**< X and Y coordinates of the menu option */
    uint32_t width, height; /**< Width and height of the menu option */
} MenuOption;

/**
 * @brief Checks if a point is on a menu option
 * @param option The menu option to check
 * @param x X coordinate of the point
 * @param y Y coordinate of the point
 * @return true if the point is on the menu option, false otherwise
 */
bool (is_point_on_menu_option)(MenuOption option, uint32_t x, uint32_t y);

/**
 * @brief Moves the selector on the menu options based on keyboard input
 * @param scancode The scancode of the keyboard input
 * @param selected Pointer to the currently selected option
 * @param number_of_options Number of menu options
 */
void (menu_move_selector)(uint8_t scancode, uint8_t *selected, uint8_t number_of_options);

/**
 * @brief Draws a selector box around the selected menu option
 * @param x X coordinate of the menu option
 * @param y Y coordinate of the menu option
 * @param width Width of the menu option
 * @param height Height of the menu option
 * @param box true to draw a box, false to remove it
 */
void (draw_menu_option_selector)(int x, int y, int width, int height, bool box);

/** @} */

#endif /* __MENU_OPTION_H_ */
