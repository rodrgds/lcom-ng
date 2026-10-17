#include "startMenuState.h"
#include <minix/drivers.h>
#include <minix/driver.h>
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "utilities.h"
#include "i8042.h"

StartMenuState* newStartMenuState() {
	StartMenuState* state = (StartMenuState*) malloc(sizeof(StartMenuState));

	state->done = 0;
	state->startMenu = loadBitmap(getImagePath("startmenu"));

	int xi = 735;
	int xf = 961;
	int yi = 39;
	int yf = 155;
	state->playButton = newRectangle(xi, yi, xf, yf);
	state->mouseOnPlay = 0;

	/*
	 * espaco para codigo para o botao score que ainda nao existe
	 */

	yi = 326;
	yf = 440;
	state->exitButton = newRectangle(xi, yi, xf, yf);
	state->mouseOnExit = 0;

	return state;
}

void updateStartMenuState(StartMenuState* state, unsigned long scancode) {
	// if ESC has been pressed, quit
	if (scancode == ESC) {
		state->action = EXIT_CHOSEN;
		state->done = 1;
	}

	// if mouse is inside the play button rectangle (boundaries)
	if (mouseInButton(state->playButton))
		state->mouseOnPlay = 1;
	else state->mouseOnPlay = 0;

	// if mouse is inside the exit button rectangle (boundaries)
	if (mouseInButton(state->exitButton))
		state->mouseOnExit = 1;
	else state->mouseOnExit = 0;

	// if left clicking on play
	if(mouse.leftButton && state->mouseOnPlay) {
		state->action = PLAY_CHOSEN;
		state->done = 1;
	}

	// if left clicking on exit
	if(mouse.leftButton && state->mouseOnExit) {
		state->action = EXIT_CHOSEN;
		state->done = 1;
	}
}

void drawStartMenuState(StartMenuState* state) {
	drawBitmap(state->startMenu, 0, 0, ALIGN_LEFT);
}

void deleteStartMenuState(StartMenuState* state) {
	deleteBitmap(state->startMenu);
	deleteRectangle(state->playButton);
	deleteRectangle(state->exitButton);

	free(state);
}
