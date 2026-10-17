#ifndef __MAINMENUSTATE_H
#define __MAINMENUSTATE_H

#include <stdio.h>
#include "Bitmap.h"
#include "mouse.h"
#include "Button.h"

/** @defgroup MainMenuState MainMenuState
 * @{
 * functions regarding MainMenuState manipulation
 */

typedef enum{
	PLAY_CHOSEN, EXIT_CHOSEN
}MainMenuAction;

typedef struct{

	int play;
	int overPlay;
	int overExit;

	Bitmap* menuImage;
	Bitmap* pokeball;
	Box* playButton;
	Box* exitButton;


}MainMenuState;

/**
 * @brief Creates a new main menu state
 *
 * @return Non NULL pointer to the new main menu state
 */
MainMenuState* newMainMenuState();

/**
 * @brief draws a pokeball next to the menu options
 *
 * @param main menu state
 */
void drawPokeball(MainMenuState* state);

/**
 * @brief verifies if the user selected the play option
 *
 * @param main menu state
 */
int verifyStateChange(MainMenuState* state);

/**
 * @brief updates the main menu
 *
 * @param main menu state
 * @param keyboard scancode
 *
 * @return int saying if the game is to be exited or not
 */
int updateMainMenuState(MainMenuState* state,  unsigned long scancode);

/**
 * @brief draws the main menu state
 *
 * @param main menu state to be drawn
 */
void drawMenu(MainMenuState* state);
/**
 * @brief deletes the main menu state
 *
 * @param main menu state to be deleted
 */
void deleteMainMenuState(MainMenuState* state);
/**@}*/
#endif
