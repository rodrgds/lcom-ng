/**
 * @file menu.h
 * @brief Declares the menu UI and its operations.
 *
 * Provides the interface for creating, destroying, and drawing menus,
 * as well as accessing menu options, callbacks, and layout properties.
 */

#ifndef __MENU_H
#define __MENU_H

#include "model/sprite.h"
#include "text.h"
#include "assets/ui.h"

/**
 * @brief Represents a menu with a title, options, and corresponding actions.
 */
struct menu;
typedef struct menu menu_t;

/**
 * @brief Creates a menu with the given title, options, and callback functions.
 * @param title_str The menu title string.
 * @param options Array of option strings.
 * @param func Array of callback functions for each option.
 * @param size Number of options.
 * @param h_res Horizontal resolution of the screen.
 * @param v_res Vertical resolution of the screen.
 * @return Pointer to the created menu, or NULL on failure.
 */
menu_t* create_menu(const char *title_str, const char **options, void (**func)(void), unsigned int size, uint16_t h_res, uint16_t v_res);

/**
 * @brief Changes the currently selected menu option.
 * @param menu Pointer to the menu.
 * @param opt Index of the option to select.
 */
void change_menu_opt(menu_t * menu, unsigned int opt);

/**
 * @brief Destroys a menu and frees its resources.
 * @param menu Pointer to the menu to destroy.
 */
void destroy_menu(menu_t *menu);

/**
 * @brief Draws the menu on the screen.
 * @param menu Pointer to the menu.
 * @return 0 on success, non-zero on error.
 */
int draw_menu(menu_t* menu);

/**
 * @brief Gets the currently selected menu option index.
 * @param menu Pointer to the menu.
 * @return Index of the selected option.
 */
unsigned int get_menu_opt(menu_t* menu);

/**
 * @brief Sets the currently selected menu option index.
 * @param menu Pointer to the menu.
 * @param opt Index to set as selected.
 */
void set_menu_opt(menu_t* menu, unsigned int opt);

/**
 * @brief Gets the number of options in the menu.
 * @param menu Pointer to the menu.
 * @return Number of options.
 */
unsigned int get_menu_size(menu_t* menu);

/**
 * @brief Gets the callback function for a given menu option.
 * @param menu Pointer to the menu.
 * @param opt Option index.
 * @return Function pointer for the option's callback.
 */
void (*get_menu_callback(menu_t *menu, unsigned int opt))(void);

/**
 * @brief Gets the sprite for the menu option background.
 * @param menu Pointer to the menu.
 * @return Pointer to the option sprite.
 */
sprite_t* get_menu_option(menu_t *menu);

/**
 * @brief Gets the horizontal position for menu options.
 * @param menu Pointer to the menu.
 * @return X position in pixels.
 */
uint16_t get_menu_x_pos(menu_t *menu);

/**
 * @brief Gets the vertical offset between menu options.
 * @param menu Pointer to the menu.
 * @return Vertical offset in pixels.
 */
uint16_t get_menu_opt_vertical_offset(menu_t *menu);

/**
 * @brief Gets the Y position of the menu title.
 * @param menu Pointer to the menu.
 * @return Y position in pixels.
 */
uint16_t get_menu_title_y(const menu_t* menu);

#endif //__MENU_H
