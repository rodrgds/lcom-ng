#ifndef MENU_H
#define MENU_H

#include "../macros/includes.h"
#include "../load/view.h"
#include "../devices/mouse.h"

/**
 * @brief Changes the current value of a menu state controlling variable
 * @param action The action to which the menu state controlling variable is going to change into
 */
void set_selection_menu(action_t action);
/**
 * @brief Controls the inputs of the mouse to control the menu state
 * @return Returns the action that has been made by the user using the mouse
 */
action_t mouse_ih_menu();
/**
 * @brief Controls the inputs of the keyboard to control the menu state
 * @param scancode Keyboard input which will control the menu state and it's features
 * @return Returns the action that has been made by the user using the keyboard
 */
action_t keyboard_ih_menu(uint32_t scancode);

/**
 * @brief Draws the letters from the main title onto the screen
 */
void draw_menu_title();
/**
 * @brief Drawing function that draws all the features of the menu state.
 */
void draw_menu();
/**
 * @brief Draws all the menu letters and symbols
 */
void draw_menu_words();

#endif
