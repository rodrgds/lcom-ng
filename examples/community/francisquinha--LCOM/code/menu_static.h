#ifndef __MENU_STATIC_H
#define __MENU_STATIC_H

/** @file menu_static.h
 * @brief file containing declarations of the static functions that deal with the game menus
 */

/** @defgroup menu_static menu_static
 * @{
 *
 * @brief functions for handling game menus
 */

/**
 * @brief determines the next letter to be filled when choosing names for the players
 * @param menu menu where we are choosing names
 */
static void manage_name_letter(Menu *menu);

/**
 * @brief to obtain a keyboard number
 * @param menu the menu we want the number for
 * @param scan_code the keyboard scan code
 */
static void get_keyboard_digit(Menu *menu, long scan_code);

/**
 * @brief function to handle keyboard input in the menus
 * @param menu the menu where we want to handle keyboard input
 * @param scan_code the keyboard scan code
 */
static void get_keyboard_menu(Menu *menu, long scan_code);

/**
 * @brief function to determine the option selected with the mouse in the initial game menu
 * @param menu where we want to handle the mouse
 * @param button menu button that was selected by the mouse
 * @param x horizontal position of the mouse
 * @param y vertical position of the mouse
 */
static void get_mouse_menu_init(Menu *menu, MenuButton *button, unsigned short x, unsigned short y);

/**
 * @brief function to determine the option selected with the mouse in the new game menu
 * @param menu where we want to handle the mouse
 * @param button menu button that was selected by the mouse
 * @param x horizontal position of the mouse
 * @param y vertical position of the mouse
 */
static void get_mouse_menu_new(Menu *menu, MenuButton *button, unsigned short x, unsigned short y);

/**
 * @brief function to determine the option selected with the mouse in the load game menu
 * @param menu where we want to handle the mouse
 * @param button menu button that was selected by the mouse
 * @param x horizontal position of the mouse
 * @param y vertical position of the mouse
 */
static void get_mouse_menu_load_save(Menu *menu, MenuButton *button, unsigned short x, unsigned short y);

/**
 * @brief function to determine the option selected with the mouse in the menus
 * @param menu where we want to handle the mouse
 * @param button menu button that was selected by the mouse
 * @param x horizontal position of the mouse
 * @param y vertical position of the mouse
 */
static void get_mouse_menu(Menu *menu, MenuButton *button, unsigned short x, unsigned short y);

/**
 * @brief function to select the corresponding option in the menu
 * @param menu the menu where we want to select an option
 * @param button that we selected
 */
static void select_mouse_menu(Menu *menu, MenuButton button);

/** @} end of menu_static */
#endif /* __MENU_STATIC_H */
