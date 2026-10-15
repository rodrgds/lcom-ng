#ifndef _SM_DIED_H
#define _SM_DIED_H

#include <lcom/lcf.h>
#include "sprite.h"
#include "spriteLoader.h"
#include "cursor.h"
#include "key_pressed.h"
#include "score.h"
#include "button.h"

/**
 * @file sm_died.h
 * @brief Single Mode died state management
 * 
 * This module provides functions to create, destroy, draw, and process input for the Single Mode died state.
 * It manages the display of the "You Died" screen, including the title, score, and buttons to play again or return to the menu.
 */

struct SmDied_imp;
typedef struct SmDied_imp SmDied;

/**
 * @brief Creates a new SmDied object
 * 
 * @param loader Pointer to the SpriteLoader object
 * @return Pointer to the created SmDied object, or NULL if the creation failed
 */
SmDied* create_sm_died(SpriteLoader *loader, Score *score);

/**
 * @brief Destroys a SmDied object, freeing all allocated memory
 * 
 * @param d Pointer to the SmDied object to be destroyed
 */
void destroy_sm_died(SmDied *d);

/**
 * @brief Draws the SmDied screen
 * 
 * @param d Pointer to the SmDied object to be drawn
 */
void draw_sm_died(SmDied *d);

/**
 * @brief Processes the input from the cursor in the SmDied state
 * 
 * @param c Pointer to the Cursor object
 * @return 1 if the game should exit, 0 otherwise, 2 to play again
 */
int process_sm_died_mouse(SmDied *d, Cursor *c);

/**
 * @brief Processes the input from the keyboard in the SmDied state
 * 
 * @param d Pointer to the SmDied object
 * @param key Pointer to the KeyPressed object containing the state of the keys
 * @return 1 if the game should exit, 0 otherwise
 */
int process_sm_died_kbd(SmDied *d, KeyPressed *key);

#endif
