#ifndef _SINGLEMODE_H_
#define _SINGLEMODE_H_

#include <lcom/lcf.h>
#include "spriteLoader.h"
#include "cursor.h"
#include "menu.h"
#include "key_pressed.h"
#include "sm_game.h"
#include "sm_died.h"
#include "score.h"

/**
 * @file singleMode.h
 * 
 * @brief Single player mode management
 * 
 * This module provides functions to create, manage, and draw the single player mode in the game.
 * It handles the changes between the game state and the died state, as well as processing input from the keyboard, mouse and timer.
 */

struct singleMode_imp;
typedef struct singleMode_imp SingleMode;

typedef enum {
  GAME,
  DIED
} Sm_State;

/**
 * @brief Creates a new SingleMode object, which contains a player and a grid of walls
 * 
 * @param loader Pointer to the SpriteLoader object
 * @return Pointer to the created SingleMode object, or NULL if the creation failed
 */
SingleMode *create_singleMode(SpriteLoader *loader);

/**
 * @brief Destroys a SingleMode object, freeing all allocated memory
 * 
 * @param sm Pointer to the SingleMode object to be destroyed
 */
void destroy_singleMode(SingleMode *sm);

/**
 * @brief Draws the SingleMode object on the screen
 * 
 * @param sm Pointer to the SingleMode object to be drawn
 */
void draw_singleMode(SingleMode *sm);

/**
 * @brief Processes the input from the keyboard
 * 
 * @param sm Pointer to the SingleMode object
 * @param key Pointer to the KeyPressed object containing the state of the keys
 * @return 0 if the game should continue, 1 go back to the menu
 */
int process_single_mode_kbd(SingleMode *sm, KeyPressed *key);

/**
 * @brief Processes the input from the mouse
 * 
 * @param sm Pointer to the SingleMode object
 * @param c Pointer to the Cursor object
 * @return 0 if the game should continue, 1 go back to the menu
 */
int process_single_mode_mouse(SingleMode *sm, Cursor *c);

/**
 * @brief Processes the timer events in the SingleMode object
 * 
 * @param sm Pointer to the SingleMode object
 * @return 0 if the game should continue, 1 go back to the menu
 */
int process_single_mode_timer(SingleMode *sm);




#endif /* _SINGLEMODE_H_ */

