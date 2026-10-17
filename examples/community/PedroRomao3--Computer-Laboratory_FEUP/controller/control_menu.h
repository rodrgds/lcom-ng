#ifndef _CONTROLMENU_H_
#define _CONTROLMENU_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "control_base.h"

extern GameState state;
extern bool isMulti;

/**
 @brief Handles the timer interrupt in the menu state.
This function handles the timer interrupt when the game is in the menu state.
It performs different actions based on the current state of the game.
If the state is MENU or OPRDY, it calls the drawMainMenu() function and draws the cursor.
If the state is WAIT, it calls the draw_wait() function.
If the state is NO_WINNER, it calls the drawNoWinner() function and draws the cursor.
If the state is WIN, it calls the drawWin() function and draws the cursor.
If the state is LOOSE, it calls the drawLoose() function and draws the cursor.
 @return 0 on success.
*/
int ti_menu();

/**
 @brief Handles the keyboard interrupt in the menu state.
This function handles the keyboard interrupt when the game is in the menu state.
It performs different actions based on the current state and the received scancode.
If the state is WAIT and the scancode is Ql, it sets the state to MENU, sends a character (0xFF), and sets isMulti to false.
If the scancode is BREAK_ESC, it sets the state to END.
If the state is STAND, it checks for a winner based on the game mode (singleplayer or multiplayer).
 @return 0 on success.
*/
int ki_menu();

/**
 @brief Handles the mouse action in the menu state.
This function handles the mouse action when the game is in the menu state.
It performs different actions based on the current state and the mouse coordinates.
If the state is MENU or OPRDY, it calls the handleMenuState() function and passes the mouse coordinates.
 @return 0 on success.
*/
int mouseaction_menu();

/**
 @brief Allows the player to stand.
This function sets the stand_allowed variable to true, allowing the player to stand in the game.
 @return 0 on success.
*/
int allow_stand();

/**
 @brief Determines the winner between the player and the computer.
This function calculates the scores for both the player and the computer,
compares the scores, and updates the game state accordingly (NO_WINNER, WIN, or LOOSE).
It also updates the player's money and resets the cards and coins.
Time Complexity: O(1) - Constant time complexity as the operations performed are independent of input size.
 @return int - Always returns 0, as there is no error handling in this function.
*/
int check_winner();


#endif
