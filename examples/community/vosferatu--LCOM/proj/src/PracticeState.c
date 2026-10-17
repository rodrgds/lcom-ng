#include "PracticeState.h"
#include "kbdlab3.h"
#include "keyboard.h"
#include "mouse.h"
#include "graphics.h"
#include <stdlib.h>
#include <math.h>

static int once = 1;

PracticeState* newPracticeState(){
	PracticeState* state = (PracticeState*) malloc(sizeof(PracticeState));

	state->done = 0;
	state->background = loadBitmap("practice.bmp");
	state->board = loadBitmap("game_board.bmp");
    state->readyButton = newRectangle(328,20,452,48);
    state->fireButton = newRectangle(200, 100, 800, 600);
    state->mouseOnReady = 0;

    state->bullet = NULL;

    state->cowboy = newCowboy(0); //left cowboy
    state->target = newTarget();
    state->shield = newShield(1); //it's a cactus
    state->scoretable = newTable(0);//no opponent
    once = 1;
	return state;
}

void updatePracticeState(PracticeState* state, unsigned long scancode){
	//check the scancode

	if(state->scoretable->playerScore == 5 && once){
		deleteShield(state->shield);
		state->shield = NULL;
		state->shield = newShield(0);
		once = 0;
	}

	//printf("score: %d\n", state->scoretable->playerScore);

	if (state->scoretable->playerScore > 11){
		state->done = 1;
		return;
	}


	int move; move = 0;

	if(state->bullet != NULL) checkCollisions(state);

	if(scancode == KEY_ESC){
		state->done = 1;
		return;
	} else if(scancode == KEY_W){
		updateCowboy(state->cowboy, 0,1);
		move = 1;
	} else if(scancode == KEY_A){
		updateCowboy(state->cowboy, -1,0);
		move = 1;
	} else if(scancode == KEY_S){
		updateCowboy(state->cowboy, 0,-1);
		move = 1;
	} else if(scancode == KEY_D){
		updateCowboy(state->cowboy, 1,0);
		move = 1;
	} else updateCowboy(state->cowboy, 0, 0);

	if(move && state->cowboy->state == 1){
		state->cowboy->state = 0;
	}

	updateTarget(state->target);

	updateShield(state->shield);

	if(state->bullet != NULL) updateBullet(state->bullet);

	if(mouseInsideRect(state->fireButton) && getMouse()->leftButtonReleased && state->bullet == NULL){
		playerFire(state);
		getMouse()->leftButtonReleased = 0;
	}

	//mouse in ready
	if (mouseInsideRect(state->readyButton)){
		state->mouseOnReady = 1;

		if (getMouse()->leftButtonReleased){
			state->done = 1;
			getMouse()->leftButtonReleased = 0;
		}
	}
	else state->mouseOnReady = 0;
}

void drawPracticeState(PracticeState* state){

	if(state->done)
		return;

	drawBitmap(state->background, 0, 0, ALIGN_LEFT);
	drawBitmapT(state->board, 0, 100, ALIGN_LEFT);

	drawCowboy(state->cowboy);
	drawTarget(state->target);
	drawShield(state->shield);

	if(state->bullet != NULL) drawBullet(state->bullet);

    if (state->mouseOnReady)
        drawRectangle(state->readyButton);

    drawTable(state->scoretable);
}


void deletePracticeState(PracticeState* state){
	deleteBitmap(state->background);
	deleteBitmap(state->board);

	deleteCowboy(state->cowboy);
	deleteTarget(state->target);
	deleteShield(state->shield);
	deleteTable(state->scoretable);
	deleteRectangle(state->fireButton);
	deleteRectangle(state->readyButton);
	deleteBullet(state->bullet);

	free(state);
}

void playerFire(PracticeState* state){
	//check this function stability
	int vx;
	float vy;
	float mouseX = getMouse()->x;
	float mouseY = getMouse()->y;
	float cowboyX = state->cowboy->x + state->cowboy->w;
	float cowboyY = state->cowboy->y + 28;
	float slope;

	slope = ((cowboyY - mouseY)/(cowboyX - mouseX))*8;

	vx = 8;
	vy = slope;

	state->bullet = newBullet(state->cowboy, vx, vy);
	state->cowboy->state = 1; //shooting bitmap

}

void checkCollisions(PracticeState* state){
	int hitTarget;
	int hitShield;

	hitTarget = !((state->target->x > (state->bullet->x + state->bullet->w)) ||
			((state->target->x + state->target->w) < state->bullet->x) ||
			(state->target->y > (state->bullet->y + state->bullet->h)) ||
			((state->target->y + state->target->h) < state->bullet->y));

	hitShield = !((state->shield->x > (state->bullet->x + state->bullet->w)) ||
			((state->shield->x + state->shield->w) < state->bullet->x) ||
			(state->shield->y > (state->bullet->y + state->bullet->h)) ||
			((state->shield->y + state->shield->h) < state->bullet->y));

	if(hitShield){
		state->bullet = NULL;
		state->cowboy->state = 0;
		return;
	}

	if(hitTarget){
		state->bullet = NULL;
		state->target->hit = 1;
		state->scoretable->playerScore++;
		state->cowboy->state = 0;
		return;
	}

	if(state->bullet->y < 126){
		state->bullet->vy = - state->bullet->vy;
		return;
	}

	if((state->bullet->y + state->bullet->h) > 573){//h_res margin
		state->bullet->vy = - state->bullet->vy;
		return;
	}

	if((state->bullet->x + state->bullet->w) > 780){//v_res-margin
		state->bullet = NULL;
		state->cowboy->state = 0;
		return;
	}
}
