#include <lcom/lcf.h>
#ifndef KEYBOARD_H
#define KEYBOARD_H

/** @defgroup Keyboard Keyboard
 * @{
 * Functions to control the keyboard input
 */

/** @brief Extern variable that contains the key value from kbc */
extern uint8_t scannedKey;
/**
 * @brief Function to subscribe to the keyboard interrupts
 * @param bit_no uint8_t* for the variable bit_no to the changed 
 * @return Returns 0 if successfull
 */
int kbd_subscribe_int(uint8_t *bit_no);
/**
 * @brief Function to unsubscribe from the keyboard interrupts
 * @return Returns 0 if successfull
 */
int kbd_unsubscribe_int();
/**
 * @brief Function to parse the keyboard input from menu
 * @param option int* for the variable option to the changed 
 */
void kbd_menu_input(int *option);
/**
 * @brief Function to parse the keyboard input from game
 * @param input int* for the variable input to the changed 
 * @param nPlanets int for the number of planets in the mission
 */
void kbd_game_input(int *input, int nPlanets);
/**
 * @}
 */
#endif /*KEYBOARD_H*/
