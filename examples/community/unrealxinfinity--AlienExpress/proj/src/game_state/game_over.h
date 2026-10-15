#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "../macros/includes.h"
#include "../devices/mouse.h"
#include "../game_state/level.h"


/**
 * @brief Controls the inputs of the keyboard to control the game over state
 * @param scancode Keyboard input which will control the game over state and it's features
 * @return Returns the action that has been made by the user using the keyboard
 */
action_t keyboard_ih_game_over(uint32_t scancode);
/**
 * @brief Controls the inputs of the mouse to control the game over state
 * @return Returns the action that has been made by the user using the mouse
 */
action_t mouse_ih_game_over();

/**
 * @brief Changes the current value of a game over state controlling variable
 * @param action The action to which the game over state controlling variable is going to change into
 */
void set_selection_game_over(action_t action);
/**
 * @brief Drawing function that draws all the features of the game over state.
 */
void draw_game_over();
/**
 * @brief Draws all the game over letters and symbols
 */
void draw_game_over_words();
/**
 * @brief Draws the letters from the game over tag onto the screen
 */
void draw_game_over_tag();

#endif
