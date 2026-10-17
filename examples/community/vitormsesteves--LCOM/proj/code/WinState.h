#pragma once

#include "bitmap.h"

/** @defgroup WinState WinState
 * @{
 * Functions for Win State
 */

typedef struct {
	int done; // specifies if the state is done
	int counter; // used to change background
	Bitmap* background1; // specifies the background image 1 in Win Menu
	Bitmap* background2; // specifies the background image 2 in Win Menu
} WinState;

/**
 * @brief Creates a new Win State
 *
 * @return Non NULL pointer to the state
 */
WinState* newWinState();

/**
 * @brief Updates a Win State
 *
 * @param state Pointer to LoseState
 * @param scancode Scancode read by the Keyboard
 */
void updateWinState(WinState* state, unsigned long scancode);

/**
 * @brief Draws a Win State
 *
 * @param state Pointer to WinState
 */
void drawWinState(WinState* state);

/**
 * @brief Deletes a Win State
 *
 * @param state Pointer to WinState
 */
void deleteWinState(WinState* state);
