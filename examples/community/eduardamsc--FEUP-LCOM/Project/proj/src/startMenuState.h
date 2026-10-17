#pragma once

#include "bitmap.h"
#include "rectangle.h"

typedef enum {
	PLAY_CHOSEN, EXIT_CHOSEN
} StartMenuAction;

typedef struct {
	int done;

	Bitmap* startMenu;

	Rectangle* playButton;
	Rectangle* scoreButton;
	Rectangle* exitButton;
	int mouseOnPlay;
	int mouseOnScore;
	int mouseOnExit;

	StartMenuAction action;
} StartMenuState;

/**
 * @brief Initializes main menu state
 * @return The struct with the main menu state
 */
StartMenuState* newStartMenuState();

/**
 * @brief Updates game according to the button pressed in the main menu
 * @param state The struct with the current menu state
 * @param scancode
 */
void updateStartMenuState(StartMenuState* state, unsigned long scancode);

/**
 * @brief Draws main menu
 * @param state The struct with the current menu state
 */
void drawStartMenuState(StartMenuState* state);

/**
 * @brief Frees all resources used by the main menu
 * @param state The struct with the current menu state
 */
void deleteStartMenuState(StartMenuState* state);
