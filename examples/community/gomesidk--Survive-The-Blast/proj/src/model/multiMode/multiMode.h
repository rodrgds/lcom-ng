#ifndef _MULTIMODE_H_
#define _MULTIMODE_H_

#include <lcom/lcf.h>

#include "spriteLoader.h"
#include "cursor.h"
#include "key_pressed.h"
#include "mm_connection.h"
#include "mm_game.h"
#include "mm_winner.h"

/**
 * @file multiMode.h
 * @brief Multiplayer mode management
 * 
 * This module provides functions to create, manage, and draw the multiplayer mode in the game.
 * It handles the connection state, game state, and winner state, as well as processing input from the keyboard, mouse, timer, and serial port.
 */

struct multiMode_imp;
typedef struct multiMode_imp MultiMode;

typedef enum {
  MM_CONNECTION,
  MM_GAME,
  MM_WINNER
} Mm_State;

/**
 * @brief Creates a new MultiMode object, which contains multiple players and a grid of walls
 * 
 * @param loader Pointer to the SpriteLoader object
 * @return Pointer to the created MultiMode object, or NULL if the creation failed
 */
MultiMode *create_multiMode(SpriteLoader *loader);

/**
 * @brief Destroys a MultiMode object, freeing all allocated memory
 * 
 * @param mm Pointer to the MultiMode object to be destroyed
 */
void destroy_multiMode(MultiMode *mm);

/**
 * @brief Draws the MultiMode object on the screen, including all players
 * 
 * @param mm Pointer to the MultiMode object to be drawn
 */
void draw_multiMode(MultiMode *mm);

/**
 * @brief Processes the input from the keyboard
 * 
 * @param mm Pointer to the MultiMode object
 * @param key Pointer to the KeyPressed object containing the state of the keys
 * @return 0 if the game should continue, 1 to go back to the menu
 */
int process_multi_mode_kbd(MultiMode *mm, KeyPressed *key);

/**
 * @brief Processes the input from the mouse for multiplayer
 * 
 * @param mm Pointer to the MultiMode object
 * @param c Pointer to the Cursor object
 * @return 0 if the game should continue, 1 to go back to the menu
 */
int process_multi_mode_mouse(MultiMode *mm, Cursor *c);

/**
 * @brief Processes the input from the timer for multiplayer
 * 
 * @param mm Pointer to the MultiMode object
 * @return 0 if the game should continue, 1 to go back to the menu
 */
int process_multi_mode_timer(MultiMode *mm);


/**
 * @brief Process the input from the serial port for multiplayer
 * 
 * @param mm Pointer to the MultiMode object
 * @param byte Pointer to the byte received from the serial port
 * @return 0 if the game should continue, 1 to go back to the menu
 */
int process_multi_mode_sp(MultiMode *mm, uint8_t byte);

#endif /* _MULTIMODE_H_ */
