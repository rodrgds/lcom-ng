#ifndef PAUSE_H
#define PAUSE_H

#include "../macros/includes.h"
#include "../game_state/level.h"
#include "../devices/mouse.h"

/**
 * @brief Drawing function that draws all the features of the pause state.
 */
void draw_pause();
/**
 * @brief Draws all the pause letters and symbols
 */
void draw_pause_words();

/**
 * @brief Changes the current value of a pause state controlling variable
 * @param action The action to which the pause state controlling variable is going to change into
 */
void set_selection_pause(action_t action);

/**
 * @brief Controls the inputs of the keyboard to control the pause state
 * @param scancode Keyboard input which will control the pause state and it's features
 * @return Returns the action that has been made by the user using the keyboard
 */
action_t keyboard_ih_pause(uint32_t scancode);
/**
 * @brief Controls the inputs of the mouse to control the pause state
 * @return Returns the action that has been made by the user using the mouse
 */
action_t mouse_ih_pause();

#endif

