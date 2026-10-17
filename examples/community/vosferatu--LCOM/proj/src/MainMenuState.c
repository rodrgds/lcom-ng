#include "MainMenuState.h"

#include "graphics.h"
#include "rectangle.h"
#include "kbdlab3.h"
#include "keyboard.h"
#include "mouse.h"
#include "utils.h"

MainMenuState* newMainMenuState() {
    MainMenuState* state = (MainMenuState*) malloc(sizeof(MainMenuState));

    state->done = 0;
    state->background = loadBitmap("menu.bmp");
    //values checked in image
    state->playButton = newRectangle(346,278,472,312);
    state->mouseOnPlay = 0;
    state->practiceButton = newRectangle(284,324,534,356);
    state->mouseOnPractice = 0;
    state->exitButton = newRectangle(346,370,472,402);
    state->mouseOnExit = 0;

    return state;
}

void updateMainMenuState(MainMenuState* state, unsigned long scancode) {
    //ESC = quit
    if (scancode == KEY_ESC) {
        state->action = EXIT_GAME;
        state->done = 1;
    }

    if (scancode == KEY_SPACE) {
        state->action = MPLAYER;
        state->done = 1;
    }

    //mouse in play
	if (mouseInsideRect(state->playButton)){
		state->mouseOnPlay = 1;

		if (getMouse()->leftButtonReleased){
			state->action = PLAY_GAME;
			state->done = 1;
			getMouse()->leftButtonReleased = 0;
		}
	}
	else state->mouseOnPlay = 0;

	//mouse in exit
	if (mouseInsideRect(state->exitButton)){
		state->mouseOnExit = 1;

		if (getMouse()->leftButtonReleased){
			state->action = EXIT_GAME;
			state->done = 1;
			getMouse()->leftButtonReleased = 0;
		}
	}
	else state->mouseOnExit = 0;

	//mouse in practice
	if (mouseInsideRect(state->practiceButton)){
		state->mouseOnPractice = 1;

		if (getMouse()->leftButtonReleased){
			state->action = PRACTICE;
			state->done = 1;
			getMouse()->leftButtonReleased = 0;
		}
	}
	else state->mouseOnPractice = 0;

}

void drawMainMenuState(MainMenuState* state) {
    drawBitmap(state->background, 0, 0, ALIGN_LEFT);

    if (state->mouseOnPlay)
        drawRectangle(state->playButton);
    else if (state->mouseOnExit)
        drawRectangle(state->exitButton);
    	else if(state->mouseOnPractice)
    		drawRectangle(state->practiceButton);
}

void deleteMainMenuState(MainMenuState* state) {
    deleteBitmap(state->background);
    deleteRectangle(state->playButton);
    deleteRectangle(state->exitButton);
    deleteRectangle(state->practiceButton);

    free(state);
}
