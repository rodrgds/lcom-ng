/**
 * @file menu.h
 * @brief Function declarations used for the game menu.
 */


#ifndef _MENU_STATE_
#define _MENU_STATE_

#include <lcom/lcf.h>
#include "../helper/structs.h"
#include "../drivers/video.h"


/**
 * @brief Loads the sprites for the menu state.
 * 
 * Loads images corresponding to the current substate, e.g., main menu, pause and game over.
 * 
 * @return int 0 in case of success.
 */
int (load_menu_state)();


/**
 * @brief Draws the sprites that correspond to the current substate.
 * 
 * Draws the mouse cursor and buttons (if the cursor is over the button, the sprite changes).
 * In the game over state, waits before returning to the main menu.
 * 
 * @return int 0 on success.
 */
int (draw_menu_state)();


/**
 * @brief Keyboard interrupt handler for the menu.
 * 
 * It is used in the code of other states, but unused for the menu.
 * 
 * @return int 0.
 */
int (kbd_menu_ih)(); 


/**
 * @brief Mouse interrupt handler for the menu.
 * 
 * Updates the cursor position and when the left mouse button is clicked over a button the state is changed
 * 
 * @return int 0.
 */
int (mouse_menu_ih)(); 


#endif // _MENU_STATE_
