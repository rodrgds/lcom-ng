#include <minix/drivers.h>
#include "MainMenuState.h"
#include "keyboard.h"
#include "interface.h"
#include "mouse.h"
#include "vbe.h"
#include "Bitmap.h"
#include "Button.h"
#include "i8254.h"

MainMenuState* newMainMenuState() {
    MainMenuState* state = (MainMenuState*) malloc(sizeof(MainMenuState));

    state->menuImage = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/menubackground.bmp");
    state->pokeball = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/poke.bmp");
    state->playButton = newBox(330, 300, 470, 390);
    state->exitButton = newBox(330, 410, 470, 500);

    return state;
}

int verifyStateChange(MainMenuState* state){
	if(state->play)
		return 1;
	else
		return 0;
}

void drawPokeball(MainMenuState* state){
	if(state->overPlay){
		drawBitmap(state->pokeball,308,320,ALIGN_CENTER);
	}else if(state->overExit){
		drawBitmap(state->pokeball,308,434,ALIGN_CENTER);
	}
}

int updateMainMenuState(MainMenuState* state, unsigned long scancode) {
	int exit = 0;
	int exitK = 0;
	int exitR = 0;
	state->overPlay=0;
	state->overExit=0;
	//unsigned char code = kbd_code();

	Mouse* m = getMouse();


	 if (scancode == VAL_ESC) {
	 exitK = 1;
	 }


	// if mouse is inside the play button rectangle (boundaries)
	if (mouseInsideBox(state->playButton)) {
		state->overPlay=1;
		if (m->lb_pressed) {
			state->play = 1;
		} else {
			state->play = 0;
		}
	}

	// if mouse is inside the exit button rectangle (boundaries)
	if (mouseInsideBox(state->exitButton)) {
		state->overExit=1;

		if (m->lb_pressed) {
			exitR = 1;
		} else {
			exitR = 0;
		}
	}

	if (exitR == 1 || exitK == 1) {
		exit = 1;
		mouse_unsubscribe();
	}

	return exit;
}


void drawMenu(MainMenuState* state){
	drawBitmap(state->menuImage,0,0,ALIGN_LEFT);
}

void deleteMainMenuState(MainMenuState* state) {
    deleteBitmap(state->menuImage);
    deleteBox(state->playButton);
    deleteBox(state->exitButton);

    free(state);
}


