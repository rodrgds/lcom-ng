#pragma once

/** @defgroup MainMenuState MainMenuState
 * @{
 * Functions for Main Menu State
 */

#include "bitmap.h"
#include "rectangle.h"

typedef enum {
	PLAY_CHOSEN, EXIT_CHOSEN
} MainMenuAction;

typedef struct {
	int done; // specifies if the state is done
	Bitmap* background; // specifies the background image in Main Menu
	Bitmap* play; // specifies the button image play
	Bitmap* onPlay; // specifies the button image play with mouse inside
	Bitmap* exit; // specifies the button image exit
	Bitmap* onExit; // specifies the button image exit with mouse inside
	int mouseOnPlay; // specifies if the mouse is inside of play button
	Rectangle* playButton; // specifies the play rectangle
	int mouseOnExit; // specifies if the mouse is inside of exit button
	Rectangle* exitButton; // specifies the exit rectangle

	int action; // specifies what button was pressed
} MainMenuState;

/**
 * @brief Creates a new Main Menu State
 *
 * @return Non NULL pointer to the state
 */
MainMenuState* newMainMenuState();
/**
 * @brief Updates a Main Menu State
 *
 * @param state Pointer to MainMenuState
 * @param scancode Scancode read by the Keyboard
 */
void updateMainMenuState(MainMenuState* state, unsigned long scancode);
/**
 * @brief Draws a Main Menu State
 *
 * @param state Pointer to MainMenuState
 */
void drawMainMenuState(MainMenuState* state);
/**
 * @brief Deletes a Main Menu State
 *
 * @param state Pointer to GameState
 */
void deleteMainMenuState(MainMenuState* state);
