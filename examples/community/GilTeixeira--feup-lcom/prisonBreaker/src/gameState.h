#pragma once

#include "game.h"
#include "kbd.h"

/** @defgroup GameState
 * @{
 * GameState Struct and functions for manipulating the GameState
 */

typedef enum {
	MAINMENU, LOSEMENU, GAME
} State;

typedef struct {
	short currState;

	Game * game;

	int done;

	Bitmap* LoseMenuBitmap;
	Bitmap* MainMenuBitmap;

} GameState;

/**
 * @brief Initializes the GameState setting it to the MainMenu
 *
 * @return The Initialized GameState
 */
GameState* initGameState();

/**
 * @brief Displays The Current GameState
 *
 * @param gameState The current Gamestate
 * @param timer The current Timer
 */
void displayGameState(GameState* gameState, Timer* timer);

/**
 * @brief Updates the GameState in function of Timer interrupt
 *
 * @param gameState The GameState to be updated
 * @param timer The updated Timer
 * @param mouse The current mouse
 */
void gameStateUpdate(GameState* gameState, Timer* timer, Mouse* mouse);

/**
 * @brief Updates the GameState in function of Keyboard interrupt
 *
 * @param gameState The GameState to be updated
 * @param scancode The scancode read from the interrupt
 * @param timer The updated Timer
 * @param mouse The current mouse
 */
void gameStateUpdateKeyboard(GameState* gameState, unsigned long scancode, Timer* timer, Mouse* mouse);

/**
 * @brief  Updates the GameState in function of Mouse interrupt
 *
 * @param gameState The GameState to be updated
 * @param mouse The updated Mouse
 */
void gameStateUpdateMouse(GameState* gameState, Mouse* mouse);

/**
 * @brief Free the GameState and all the resources used by it
 *
 * @param gameState The GameState to be freed
 */
void stopGameState(GameState* gameState);
