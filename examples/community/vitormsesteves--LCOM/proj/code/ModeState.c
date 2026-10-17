#include "ModeState.h"

#include "video_gr.h"
#include "keyboard.h"
#include "mouse.h"

ModeState* newModeState() {
	ModeState* state = (ModeState*) malloc(sizeof(ModeState));
	state->done = 0;
	state->action=0;
	state->background = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/GAME_MODE.bmp");
	state->NormalMouse = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/Choose/BLUE_BUTTONS/NORMAL_WITH_MOUSE_BLUE.bmp");
	state->onNormalMouse = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/Choose/RED_BUTTONS/NORMAL_WITH_MOUSE_RED.bmp");
	state->NormalKeyboard = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/Choose/BLUE_BUTTONS/NORMAL_WITH_KBD_BLUE.bmp");
	state->onNormalKeyboard = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/Choose/RED_BUTTONS/NORMAL_WITH_KBD_RED.bmp");
	state->NightMouse = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/Choose/BLUE_BUTTONS/NIGHT_WITH_MOUSE_BLUE.bmp");
	state->onNightMouse = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/Choose/RED_BUTTONS/NIGHT_WITH_MOUSE_RED.bmp");
	state->NightKeyboard = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/Choose/BLUE_BUTTONS/NIGHT_WITH_KBD_BLUE.bmp");
	state->onNightKeyboard = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/Choose/RED_BUTTONS/NIGHT_WITH_KBD_RED.bmp");


	int x1 = getH_res() / 2 - 112;
	int y1 = 100;
	state->NormalMouseButton = newRectangle(x1, y1, 224, 82);
	y1 = 300;
	state->NightMouseButton = newRectangle(x1, y1, 224, 82);
	y1 = 200;
	state->NormalKeyboardButton = newRectangle(x1, y1, 224, 82);
	y1 = 400;
	state->NightKeyboardButton = newRectangle(x1, y1, 224, 82);

	state->mouseOnNormalMouse = 0;
	state->mouseOnNormalKeyboard = 0;
	state->mouseOnNightMouse = 0;
	state->mouseOnNightKeyboard = 0;

	return state;
}

void updateModeState(ModeState* state, unsigned long scancode) {


	if (scancode == KEY_DOWN(KEY_ESC)) {
		state->action = 4;
		state->done = 1;
	}


	if (rectangleInside(getMouse()->rect, state->NormalMouseButton)) {
		state->mouseOnNormalMouse = 1;
		if (getMouse()->leftButtonDown) {
			state->action = 0;
			state->done = 1;
		}
	} else
		state->mouseOnNormalMouse = 0;


	if (rectangleInside(getMouse()->rect, state->NormalKeyboardButton)) {
		state->mouseOnNormalKeyboard = 1;

		// and left mouse button has been released
		if (getMouse()->leftButtonDown) {
			state->action = 1;
			state->done = 1;
		}
	} else
		state->mouseOnNormalKeyboard = 0;


	if (rectangleInside(getMouse()->rect, state->NightMouseButton)) {
			state->mouseOnNightMouse = 1;

			// and left mouse button has been released
			if (getMouse()->leftButtonDown) {
				state->action = 2;
				state->done = 1;
			}
		} else
			state->mouseOnNightMouse = 0;


	if (rectangleInside(getMouse()->rect, state->NightKeyboardButton)) {
			state->mouseOnNightKeyboard = 1;

			// and left mouse button has been released
			if (getMouse()->leftButtonDown) {
				state->action = 3;
				state->done = 1;
			}
		} else
			state->mouseOnNightKeyboard = 0;

}

void drawModeState(ModeState* state) {
	drawBitmap(state->background, 0, 0, ALIGN_LEFT);
	if (state->mouseOnNormalMouse){
		//drawRectangle(state->playButton, rgb(255, 0, 0), getGraphicsBuffer());
		drawBitmap(state->onNormalMouse, getH_res() / 2, 100, ALIGN_CENTER);
	}
	else
		drawBitmap(state->NormalMouse, getH_res() / 2, 100, ALIGN_CENTER);
	if (state->mouseOnNormalKeyboard){
		//drawRectangle(state->exitButton, rgb(255, 0, 0), getGraphicsBuffer());
		drawBitmap(state->onNormalKeyboard, getH_res() / 2, 200, ALIGN_CENTER);
	}
	else
		drawBitmap(state->NormalKeyboard, getH_res() / 2, 200, ALIGN_CENTER);
	if (state->mouseOnNightMouse){
		//drawRectangle(state->exitButton, rgb(255, 0, 0), getGraphicsBuffer());
		drawBitmap(state->onNightMouse, getH_res() / 2, 300, ALIGN_CENTER);
	}
	else
		drawBitmap(state->NightMouse, getH_res() / 2, 300, ALIGN_CENTER);
	if (state->mouseOnNightKeyboard){
		//drawRectangle(state->exitButton, rgb(255, 0, 0), getGraphicsBuffer());
		drawBitmap(state->onNightKeyboard, getH_res() / 2, 400, ALIGN_CENTER);
	}
	else
		drawBitmap(state->NightKeyboard, getH_res() / 2, 400, ALIGN_CENTER);
}

void deleteModeState(ModeState* state) {
	deleteBitmap(state->background);
	deleteBitmap(state->NormalMouse);
	deleteBitmap(state->onNormalMouse);
	deleteBitmap(state->NormalKeyboard);
	deleteBitmap(state->onNormalKeyboard);
	deleteBitmap(state->NightMouse);
	deleteBitmap(state->onNightMouse);
	deleteBitmap(state->NightKeyboard);
	deleteBitmap(state->onNightKeyboard);
	deleteRectangle(state->NormalMouseButton);
	deleteRectangle(state->NightMouseButton);
	deleteRectangle(state->NormalKeyboardButton);
	deleteRectangle(state->NightKeyboardButton);

	free(state);
}
