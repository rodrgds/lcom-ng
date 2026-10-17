#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include "../cursor.h"

/**
 * @ingroup Game
 * @{
 * 
 * @defgroup menu_state Menu State
 * @{
 * 
 * @brief Functions responsible for processing the buttons on the menus.
*/

/**
 * @brief Process the start button
 * 
 * @param cursor pointer to the cursor
 * 
 * @return True if the button was clicked with the left button of the mouse, false otherwise
*/
bool process_start_button(Cursor* cursor);

/**
 * @brief Process the guess button
 * 
 * @param cursor pointer to the cursor
 * 
 * @return True if the button was clicked with the left button of the mouse, false otherwise
*/
bool process_guess_button(Cursor* cursor);

/**
 * @brief Process the exit button
 * 
 * @param cursor pointer to the cursor
 * 
 * @return True if the button was clicked with the left button of the mouse, false otherwise
*/
bool process_exit_button(Cursor* cursor);

/**
 * @brief Process the singleplayer button
 * 
 * @param cursor pointer to the cursor
 * 
 * @return True if the button was clicked with the left button of the mouse, false otherwise
*/
bool process_singleplayer_button(Cursor* cursor);

/**
 * @brief Process the multiplayer button
 * 
 * @param cursor pointer to the cursor
 * 
 * @return True if the button was clicked with the left button of the mouse, false otherwise
*/
bool process_multiplayer_button(Cursor* cursor);

/**
 *  }@
 * }@
*/

#endif
