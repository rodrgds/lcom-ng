#include "GameState.h"
#include "keyboard.h"
#include "mouse.h"
#include "rtc.h"
#include "rectangle.h"
#include "video_gr.h"

GameState* newGameState(int mouse, int nigth) {
	GameState* state = (GameState*) malloc(sizeof(GameState));
	rtc_write_alarm(20);
	rtc_enable();
	state->timer = 20;
	state->done = 0;
	state->win = 0;
	state->lose = 0;
	state->nigth = nigth;
	state->mouse = mouse;
	state->counter = 0;
	state->background = loadBitmap(
			"/home/lcom/lcom1415-t3g07/proj/rec/images/game-background.bmp");
	state->lab = newLabyrinth();
	state->rect = newRectangle(120, 330, 10, 10);
	resetMouse();
	return state;
}

void updateGameState(GameState* state, unsigned long scancode) {
	if (state->counter <= 15) {
		state->counter++;
	}
	if (state->mouse) {
		if (scancode == KEY_DOWN(KEY_D))
			moveRectangle(state->rect, 5, 0);
			//getMouse()->x += 10;
		else if (scancode == KEY_DOWN(KEY_A))
			moveRectangle(state->rect, -5, 0);
			//getMouse()->x -= 10;
		else if (scancode == KEY_DOWN(KEY_W))
			moveRectangle(state->rect, 0, -10);
			//getMouse()->y -= 10;
		else if (scancode == KEY_DOWN(KEY_S))
			moveRectangle(state->rect, 0, 10);
			//getMouse()->y += 10;
	}
	if (scancode == KEY_DOWN(KEY_ESC))
		state->done = 1;
	else if (scancode == KEY_DOWN(KEY_P))
		if (state->nigth)
			state->nigth = 0;
		else
			state->nigth = 1;
	else if (scancode == KEY_DOWN(KEY_M))
		if (state->mouse)
			state->mouse = 0;
		else
			state->mouse = 1;
	setRecLocation(getMouse()->rect, getMouse()->x, getMouse()->y);
	if (!state->mouse) {
		if (insideLab(state->lab, getMouse()->rect)) {
			resetMouse();
			if (state->counter >= 15) {
				state->lose = 1;
				state->done = 1;
			}
		}
	} else {
		if (insideLab(state->lab, state->rect)) {
			resetMouse();
			if (state->counter >= 15) {
				state->lose = 1;
				state->done = 1;
			}
		}
	}
	if (!state->mouse) {
		if (rectanglesColliding(getMouse()->rect, state->lab->finish)) {
			state->win = 1;
			state->done = 1;
		}
	} else {
		if (rectanglesColliding(state->rect, state->lab->finish)) {
			state->win = 1;
			state->done = 1;
		}
	}
}

void drawGameState(GameState* state) {
	drawBitmap(state->background, 0, 0, ALIGN_LEFT);
	drawLabyrinth(state->lab);
	if (state->nigth){
		if (!state->mouse){
		see_circle(getMouse()->x, getMouse()->y, 50, rgb(0, 0, 0),
				getGraphicsBuffer());
		}
		else
		{
		see_circle(state->rect->x, state->rect->y, 50, rgb(0, 0, 0),
							getGraphicsBuffer());
		}
	}
	draw_rectangle(0, 10, state->timer * 40, 40, rgb(255, 255, 0),
			getGraphicsBuffer());
	if (state->mouse){
		drawRectangle(state->rect, rgb(0,0,255), getGraphicsBuffer());
	}
}

void deleteGameState(GameState* state) {
	rtc_disable();
	deleteRectangle(state->rect);
	deleteLabyrinth(state->lab);
	deleteBitmap(state->background);
	free(state);
}
