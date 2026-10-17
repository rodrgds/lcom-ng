#ifndef __MAINMENUSTATE_H
#define __MAINMENUSTATE_H

#include "bitmap.h"
#include "rectangle.h"

/** @defgroup MainMenuState MainMenuState
 * @{
 *
 * Functions for controlling MainMenuState
 */

/* Represents a Menu Action */
typedef enum {
	PLAY_GAME, PRACTICE, EXIT_GAME, MPLAYER
} MenuAction;


/*! Represents the MainMenuState */
typedef struct {
	int done;
	Bitmap* background;
	int mouseOnPlay;
	int mouseOnExit;
	int mouseOnPractice;
	Rectangle* playButton;
	Rectangle* practiceButton;
	Rectangle* exitButton;
	MenuAction action;
} MainMenuState;

/**
 * @brief Creates a new MainMenuState; allocates it
 * @return a pointer to the struct MainMenuState created
 */
MainMenuState* newMainMenuState();

/**
 * @brief updates elements of the MainMenuState
 * @param state a pointer to MainMenuState
 * @param scancode key pressed by user (if any)
 */
void updateMainMenuState(MainMenuState* state, unsigned long scancode);

/**
 * @brief draws bitmaps of the MainMenuState
 * @param state a pointer to MainMenuState
 */
void drawMainMenuState(MainMenuState* state);

/**
 * @brief deletes MainMenustate passed and frees from memory
 * @param state a pointer to MainMenuState
 */
void deleteMainMenuState(MainMenuState* state);


/** @} */

#endif /* __MAINMENUSTATE_H */
