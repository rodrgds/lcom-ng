#include "WinState.h"
#include "keyboard.h"

WinState* newWinState() {
	WinState* state = (WinState*) malloc(sizeof(WinState));

	state->done = 0;
	state->background1 = loadBitmap(
			"/home/lcom/lcom1415-t3g07/proj/rec/images/YOU_WIN_0S.bmp");
	state->background2 = loadBitmap(
			"/home/lcom/lcom1415-t3g07/proj/rec/images/YOU_WIN_100s.bmp");
	return state;
}

void updateWinState(WinState* state, unsigned long scancode) {
	if (state->counter < 60)
		state->counter++;
	else
		state->counter = 0;
	if (scancode == KEY_DOWN(KEY_ENTER))
		state->done = 1;
}

void drawWinState(WinState* state) {
	if (state->counter < 30)
		drawBitmap(state->background1, 0, 0, ALIGN_LEFT);
	else
		drawBitmap(state->background2, 0, 0, ALIGN_LEFT);
}

void deleteWinState(WinState* state) {
	deleteBitmap(state->background1);
	deleteBitmap(state->background2);
	free(state);
}
