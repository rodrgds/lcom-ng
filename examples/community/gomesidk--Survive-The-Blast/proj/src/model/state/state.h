#ifndef _STATE_H_
#define _STATE_H_

#include <lcom/lcf.h>
#include "key_pressed.h"
#include "cursor.h"
#include "multiMode.h"
#include "singleMode.h"
#include "menu.h"
#include "spriteLoader.h"

/**
 * @file state.h
 * @brief State management for the game
 * 
 * This module provides functions to manage the game state, including creating, updating, and drawing the current state of the game.
 * It handles transitions between different game modes such as menu, single player mode, and multiplayer mode.
 */

struct state_imp;
typedef struct state_imp State;

typedef enum {
  MENU,
  SINGLE_MODE,
  MULTI_MODE,
  EXIT
} GameState;

/**
 * @brief Creates a new State object
 * 
 * @param loader Pointer to the SpriteLoader object
 * @param c Pointer to the Cursor object
 * @return Pointer to the created State object, or NULL if the creation failed
 */
State *create_state(SpriteLoader *loader, Cursor *c);

/**
 * @brief Destroys a State object, freeing all allocated memory
 * 
 * @param state Pointer to the State object to be destroyed
 */
void destroy_state(State *state);

/**
 * @brief Updates the state of the game based on keyboard and mouse input
 * 
 * @param state Pointer to the State object
 * @param keys Array of boolean values representing the state of the keys
 * @param c Pointer to the Cursor object
 */
void update_state(State *state, KeyPressed *key, Cursor *c, uint8_t sp_byte);

/**
 * @brief Updates the state of the game based on the serial port input
 * 
 * @param state Pointer to the State object
 * @param sp_byte Byte received from the serial port
 */
void update_state_sp(State *state, uint8_t sp_byte);

/**
 * @brief Updates the state of the game based on the input from the keyboard
 * 
 * @param state Pointer to the State object
 * @param keys Array of boolean values representing the state of the keys
 * @return 0 if the game should continue, 1 if the game should exit
 */
void update_state_kbd(State *state, KeyPressed *key);

/**
 * @brief Updates the state of the game based on the input from the mouse
 * 
 * @param state Pointer to the State object
 * @param c Pointer to the Cursor object
 * @return 0 if the game should continue, 1 if the game should exit
 */
void update_state_mouse(State *state, Cursor *c);

/**
 * @brief Updates the state based on the game logic for other entities
 * 
 * @param state Pointer to the State object
 */
void update_state_timer(State *state);

/**
 * @brief Draws the current state of the game on the screen
 * 
 * @param state Pointer to the State object
 */
void draw_state(State *state);

/**
 * @brief Checks if the game is over
 * 
 * @param state Pointer to the State object
 * @return 1 if the game is over, 0 otherwise
 */
int is_game_over(State *state);

#endif

