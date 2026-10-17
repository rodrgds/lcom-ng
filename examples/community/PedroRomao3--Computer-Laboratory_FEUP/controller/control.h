#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "control_menu.h"
#include "control_game.h"

/**
 @brief Handles the timer interrupt and updates the game state.
This function is called on every timer interrupt. It calls the appropriate
functions based on the current game state, updates the display and swaps
the buffers. Returns 1 if there is an error in any of the called functions,
otherwise 0.
Time Complexity: O(n) - where n is the length of the player's name.
 @return int - 1 if there is an error, 0 if the function runs successfully.
*/
int timer_interrupt();


/**
 @brief Handles the keyboard interrupt and updates the game state accordingly.
This function is called on every keyboard interrupt. It processes the scancode,
updates the game state, and performs actions based on the current game state.
Time Complexity: O(n) - where n is the length of the player's name.
 @return int - Always returns 0, as there is no error handling in this function.
*/
int keyboard_interrupt();


/**
 @brief Handles the mouse interrupt and processes mouse actions.
This function is called on every mouse interrupt. It synchronizes mouse bytes,
updates the mouse location, and performs actions based on the current mouse packet.
Time Complexity: O(1) - Constant time complexity as the operations performed are independent of input size.
 @return int - Always returns 0, as there is no error handling in this function.
*/
int mouse_interrupt();

/**
 @brief Serial interrupt handler.
This function handles the serial interrupt and performs various operations based on the interrupt status.
 @return 0 indicating successful execution.
*/
int ser_p_interrupt();


/**
 @brief Calculates the point value based on the input integer.
This function takes an integer 'x' as input and calculates the point value
based on the given formula: point = (x - 1) / 4 + 6 if x is not 0, otherwise 0.
Time Complexity: O(1) - Constant time complexity as the calculation is independent of input size.
 @param x int - The input integer for which the point value is to be calculated.
 @return int - The calculated point value.
*/
int point(int x);


/**
 @brief Processes mouse actions based on the current game state.
This function handles mouse actions depending on the current game state.
It checks if the left button of the mouse is pressed and updates the game state
accordingly. The function handles various states such as MENU, GAME, CHIP, and EDIT.
Time Complexity: O(1) - Constant time complexity as the operations performed are independent of input size.
 @return int - Always returns 0, as there is no error handling in this function.
*/
int mouse_action();


#endif
