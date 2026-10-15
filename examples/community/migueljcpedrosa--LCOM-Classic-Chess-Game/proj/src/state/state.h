/**
 * @file state
 * @brief This file has the implementation of the functions related to the game states.
 */

#ifndef _STATE_H_
#define _STATE_H_

#include <lcom/lcf.h>
#include "../model/cursor/cursor_input.h"

/**
 * @brief Enum representing different states.
 * 
 * This enum represents the different states, which include Menu, Game, Writing Name, Game Over, and Exit.
 */
typedef enum {MENU, GAME, WRITE_NAME, GAME_OVER, EXIT} State;

extern State state;

/**
 * @brief Function to initialize the state.
 * 
 * This function initializes the state of the game to Menu and loads the state.
 *
 * @return Returns 0 on success, 1 otherwise.
 */
int (init_state)();

/**
 * @brief Function to clean up resources as the game leaves a state.
 * 
 * Depending on the current state, this function will free up resources associated with that state.
 *
 * @return Returns 0 on success, 1 otherwise.
 */
int (leave_state)();

/**
 * @brief Function to load resources as the game enters a state.
 * 
 * Depending on the target state, this function will create resources associated with that state.
 *
 * @return Returns 0 on success, 1 otherwise.
 */
int (load_state)();

/**
 * @brief Function to set the state.
 * 
 * @param new_state The new state to which the game should transition.
 * 
 * This function changes the current state to the new state provided as an argument, and handles loading and unloading of resources accordingly. 
 *
 * @return Returns 0 on success, 1 otherwise.
 */
int (set_state)(State new_state);

/**
 * @brief Function to handle keyboard input based on the current state.
 * 
 * @param scan_code The keyboard scancode.
 * @param size The size of the scancode.
 * 
 * This function delegates keyboard event handling to the relevant function depending on the current state. 
 *
 * @return Returns 0 on success, 1 otherwise.
 */
int (state_kbd_handler)(uint8_t scan_code[2], int size);

/**
 * @brief Function to handle mouse input based on the current state.
 * 
 * @param input The mouse event input.
 * 
 * This function delegates mouse event handling to the relevant function depending on the current state.
 *
 * @return Returns 0 on success, 1 otherwise.
 */
int (state_mouse_handler)(CursorInput input);

/**
 * @brief Function to handle timer events based on the current state.
 * 
 * This function delegates timer event handling to the relevant function depending on the current state.
 *
 * @return Returns 0 on success, 1 otherwise.
 */
int (state_timer_handler)();

#endif
