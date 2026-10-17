#include "LoseState.h"
#include "keyboard.h"

LoseState* newLoseState() {
	LoseState* state = (LoseState*) malloc(sizeof(LoseState));

	state->done = 0;
	state->background1 = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/YOU_LOSE_NORMAL_0S.bmp");
	state->background2 = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/YOU_LOSE_NORMAL_100S.bmp");
	return state;
}

void updateLoseState(LoseState* state, unsigned long scancode) {
	if (state->counter < 60)
		state->counter++;
	else
		state->counter=0;
	if (scancode == KEY_DOWN(KEY_ENTER))
		state->done = 1;
}

void drawLoseState(LoseState* state) {
	if (state->counter < 30)
		drawBitmap(state->background1, 0, 0, ALIGN_LEFT);
	else
		drawBitmap(state->background2, 0, 0, ALIGN_LEFT);
}

void deleteLoseState(LoseState* state) {
	deleteBitmap(state->background1);
	deleteBitmap(state->background2);
	free(state);
}
