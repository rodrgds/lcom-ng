#pragma once

/** @defgroup GameState GameState
 * @{
 * Functions for Game State
 */

#include "bitmap.h"
#include "labyrinth.h"

typedef struct {
	int done; // specifies if the state is done
	int win; // specifies if the player win
	int lose; // specifies if the player lose
	int counter; // only for correct a bug
	int timer; // specifies the time remaining
	int nigth; // specifies the night game mode
	int mouse; // specifies the mouse or keyboard game mode
	Labyrinth* lab; // specifies the labyrinth
	Bitmap* background; // specifies the background image in Game Mode
	Rectangle* rect; // specifies the rectangle controlled by keyboard
} GameState;

/**
 * @brief Creates a new Game State
 *
 * @param mouse Keyboard or Mouse interaction with the rectangle
 * @param night Normal or Nigth game mode
 * @return Non NULL pointer to the state
 */
GameState* newGameState(int mouse, int nigth);
/**
 * @brief Updates a Game State
 *
 * @param state Pointer to GameState
 * @param scancode Scancode read by the Keyboard
 */
void updateGameState(GameState* state, unsigned long scancode);
/**
 * @brief Draws a Game State
 *
 * @param state Pointer to GameState
 */
void drawGameState(GameState* state);
/**
 * @brief Deletes a Game State
 *
 * @param state Pointer to GameState
 */
void deleteGameState(GameState* state);
