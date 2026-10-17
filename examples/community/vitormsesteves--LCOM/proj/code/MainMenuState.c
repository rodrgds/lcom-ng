#include "MainMenuState.h"

#include "video_gr.h"
#include "keyboard.h"
#include "mouse.h"

MainMenuState* newMainMenuState() {
	MainMenuState* state = (MainMenuState*) malloc(sizeof(MainMenuState));

	state->done = 0;
	state->action=0;
	state->background = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/MAIN_TITLE.bmp");
	state->play = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/Main/PlayButtonBlue.bmp");
	state->onPlay = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/Main/PlayButtonRed.bmp");
	state->exit = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/Main/ExitButtonBlue.bmp");
	state->onExit = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/Main/ExitButtonRed.bmp");

	int x1 = 50;
	int x2 = 524;
	int y1 = 400;
	state->playButton = newRectangle(x1, y1, 224, 82);
	state->mouseOnPlay = 0;

	state->exitButton = newRectangle(x2, y1, 224, 82);
	state->mouseOnExit = 0;

	return state;
}

void updateMainMenuState(MainMenuState* state, unsigned long scancode) {

	// if ESC has been pressed, quit
	if (scancode == KEY_DOWN(KEY_ESC)) {
		state->action = 1;
		state->done = 1;
	}

	// if mouse is inside the play button rectangle (boundaries)
	if (rectangleInside(getMouse()->rect, state->playButton)) {
		state->mouseOnPlay = 1;
		if (getMouse()->leftButtonDown) {
			state->action = 0;
			state->done = 1;
		}
	} else
		state->mouseOnPlay = 0;

	// if mouse is inside the exit button rectangle (boundaries)
	if (rectangleInside(getMouse()->rect, state->exitButton)) {
		state->mouseOnExit = 1;

		// and left mouse button has been released
		if (getMouse()->leftButtonDown) {
			state->action = 1;
			state->done = 1;
		}
	} else
		state->mouseOnExit = 0;
}

void drawMainMenuState(MainMenuState* state) {
	drawBitmap(state->background, 0, 0, ALIGN_LEFT);
	if (state->mouseOnPlay){
		drawBitmap(state->onPlay, 50, 400, ALIGN_LEFT);
	}
	else
		drawBitmap(state->play, 50, 400, ALIGN_LEFT);
	if (state->mouseOnExit){
		drawBitmap(state->onExit, 524, 400, ALIGN_LEFT);
	}
	else
		drawBitmap(state->exit, 524, 400, ALIGN_LEFT);
}

void deleteMainMenuState(MainMenuState* state) {
	deleteBitmap(state->background);
	deleteBitmap(state->play);
	deleteBitmap(state->onPlay);
	deleteBitmap(state->exit);
	deleteBitmap(state->onExit);
	deleteRectangle(state->playButton);
	deleteRectangle(state->exitButton);

	free(state);
}
