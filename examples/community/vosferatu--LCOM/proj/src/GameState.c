#include "GameState.h"
#include "kbdlab3.h"
#include "keyboard.h"
#include "mouse.h"
#include <stdlib.h>
#include <math.h>


GameState* newGameState(){
	GameState* state = (GameState*) malloc(sizeof(GameState));

	state->done = 0;
	state->background = loadBitmap("background.bmp");
	state->board = loadBitmap("game_board.bmp");


    state->fireButton = newRectangle(200, 100, 800, 600);

    state->playerBullet = NULL;
    state->pcBullet = NULL;

    state->playerCowboy = newCowboy(0); //left cowboy
    state->pcCowboy = newCowboy(1); //right cowboy

    state->shield = newShield(1); //it's a cactus

    state->scoretable = newTable(1);//opponent use resetTable upon LEVEL CHANGE

    state->countdown = newTable(0);
    state->countdown->playerScore = 30; //sets the counter to 30

    state->level = 0;

    state->ticks = 0;

	state->gameWon = loadBitmap("gamewon.bmp");

	if (state->gameWon == NULL) printf("was null\n");

	return state;
}

void updateGameState(GameState* state, unsigned long scancode){

	state->ticks++;

	if(scancode == KEY_ESC){
		state->done = 1;
	}

	if(state->level == 3){
		if(scancode == KEY_ESC){
			state->done = 1;
		}
		return;
	}

	if (state->level == 4){
		if(scancode == KEY_ENTER){
			state->done = 1;
		}
		return;
	}

	if((state->scoretable->playerScore == 8) && (state->level == 0) && (state->scoretable->PCScore < 8)){
		state->level = 1;
		resetTable(state->scoretable);
		deleteShield(state->shield);
		state->shield = NULL;
		state->shield = newShield(0);
	}

	if((state->scoretable->PCScore == 8) && (state->level == 0) && (state->scoretable->playerScore < 8)){
		state->level = 3;//you lost
		state->playerCowboy->state = 2;
		return;
	}

	if((state->scoretable->playerScore == 8) && (state->level == 1) && (state->scoretable->PCScore < 8)){
		state->level = 2;
		resetTable(state->scoretable);
		state->shield->vy = 6;
	}

	if((state->scoretable->PCScore == 8) && (state->level == 1) && (state->scoretable->playerScore < 8)){
		state->level = 3;//you lost
		state->playerCowboy->state = 2;
		return;
	}

	if ((state->level == 2) && ((state->ticks % 60) == 0) ){
		state->countdown->playerScore--;
	}

	if ((state->level == 2) && (state->countdown->playerScore == 0)){
		if (state->scoretable->playerScore > state->scoretable->PCScore){
			state->pcCowboy->state = 2;
			state->level = 4;//you won
		}
		else {
			state->level = 3;//you lost
			state->playerCowboy->state = 2;
		}
		return;
	}


	updatePlayer(state, scancode);

	updatePC(state);//update program controlled cowboy

	updateShield(state->shield);

	if(state->playerBullet != NULL) updateBullet(state->playerBullet);

	if(state->pcBullet != NULL) updateBullet(state->pcBullet);

}

void drawGameState(GameState* state){
	if(state->done) return;

	drawBitmap(state->background, 0, 0, ALIGN_LEFT);
	drawBitmapT(state->board, 0, 100, ALIGN_LEFT);

	drawCowboy(state->playerCowboy);
	drawCowboy(state->pcCowboy);

	drawShield(state->shield);

	if(state->playerBullet != NULL) drawBullet(state->playerBullet);
	if(state->pcBullet != NULL) drawBullet(state->pcBullet);

    drawTable(state->scoretable);

    if (state->level == 2) drawTable(state->countdown);

	if (state->level == 4){
		drawBitmapT(state->gameWon, 400, 200, ALIGN_CENTER);
	}
}

void updatePlayer(GameState* state, unsigned long scancode){
	int move; move = 0;

	if(state->playerBullet != NULL) checkPlayerCollisions(state);

	if(scancode == KEY_ESC){
		state->done = 1;
		return;
	}else if(scancode == KEY_W){
		updateCowboy(state->playerCowboy, 0,1);
		if (rand() % 2 == 0)
			updateCowboy(state->pcCowboy, 0, -1);
		else updateCowboy(state->pcCowboy, 0, 1);
		move = 1;
	} else if(scancode == KEY_A){
		updateCowboy(state->playerCowboy, -1,0);
		if (rand() % 2 == 0)
			updateCowboy(state->pcCowboy, 1, 0);
		else updateCowboy(state->pcCowboy, -1, 0);
		move = 1;
	} else if(scancode == KEY_S){
		updateCowboy(state->playerCowboy, 0,-1);
		if (rand() % 2 == 0)
			updateCowboy(state->pcCowboy, 0, 1);
		else updateCowboy(state->pcCowboy, 0, -1);
		move = 1;
	} else if(scancode == KEY_D){
		updateCowboy(state->playerCowboy, 1,0);
		if (rand() % 2 == 0)
			updateCowboy(state->pcCowboy, -1, 0);
		else updateCowboy(state->pcCowboy, 1, 0);
		move = 1;
	} else	updateCowboy(state->playerCowboy, 0, 0);

	if(move && state->playerCowboy->state == 1){
		state->playerCowboy->state = 0;
	}

	if(mouseInsideRect(state->fireButton) && getMouse()->leftButtonReleased && state->playerBullet == NULL){
		playerFireGun(state);
		getMouse()->leftButtonReleased = 0;
	}

}

void checkPlayerCollisions(GameState* state){

	int hitEnemy;
	int hitShield;

	hitEnemy = !((state->pcCowboy->x > (state->playerBullet->x + state->playerBullet->w)) ||
			((state->pcCowboy->x + state->pcCowboy->w) < state->playerBullet->x) ||
			(state->pcCowboy->y > (state->playerBullet->y + state->playerBullet->h)) ||
			((state->pcCowboy->y + state->pcCowboy->h) < state->playerBullet->y));

	hitShield = !((state->shield->x > (state->playerBullet->x + state->playerBullet->w)) ||
			((state->shield->x + state->shield->w) < state->playerBullet->x) ||
			(state->shield->y > (state->playerBullet->y + state->playerBullet->h)) ||
			((state->shield->y + state->shield->h) < state->playerBullet->y));

	if(hitShield){
		state->playerBullet = NULL;
		state->playerCowboy->state = 0;
		return;
	}

	if(hitEnemy){
		state->playerBullet = NULL;
		state->scoretable->playerScore++;
		state->playerCowboy->state = 0;
		return;
	}

	if(state->playerBullet->y < 126){
		state->playerBullet->vy = - state->playerBullet->vy;
		return;
	}

	if((state->playerBullet->y + state->playerBullet->h) > 573){//h_res margin
		state->playerBullet->vy = - state->playerBullet->vy;
		return;
	}

	if((state->playerBullet->x + state->playerBullet->w) > 780){//v_res-margin
		state->playerBullet = NULL;
		state->playerCowboy->state = 0;
		return;
	}

}

void playerFireGun(GameState* state){
	int vx;
	float vy;
	float mouseX = getMouse()->x;
	float mouseY = getMouse()->y;

	if (mouseX < 200) {
		mouseX += 200;
	}
	float cowboyX = state->playerCowboy->x + state->playerCowboy->w;
	float cowboyY = state->playerCowboy->y + 28;
	float slope;

	slope = ((cowboyY - mouseY)/(cowboyX - mouseX))*7;

	vx = 7;
	vy = slope;

	state->playerBullet = newBullet(state->playerCowboy, vx, vy);
	state->playerCowboy->state = 1; //shooting bitmap
}

void updatePC(GameState* state){

	//fire-----------------------------------------------------------
	if(state->pcBullet == NULL){
		pcFire(state);
	}

	//movement-------------------------------------------------------

	if (rand() % 20 == 0){
		if(state->playerCowboy->x > 120)
			updateCowboy(state->pcCowboy, 1,0);
		else updateCowboy(state->pcCowboy, -1,0);

		if(state->playerCowboy->y > 350)
			updateCowboy(state->pcCowboy, 0, -1);
		else updateCowboy(state->pcCowboy, 0, 1);
		state->pcCowboy->state = 0;
		/*if (rand () % 2)
			updateCowboy(state->pcCowboy, 0, rand()%3 - 1);
		else updateCowboy(state->pcCowboy, rand()%3 - 1, 0); *///this makes it flicker
	}

	//collisions----------------------------------------------------
	if(state->pcBullet != NULL) pcCollisions(state);



}

void pcFire(GameState* state){

	int mult;
	if(state->level == 0) mult = 5;
	if (state->level == 1) mult = 7;
	if (state->level == 2) mult = 9;

	int vx;
	float vy;
	float mouseX = getMouse()->x;
	float mouseY = getMouse()->y;
	float cowboyX = state->playerCowboy->x + state->playerCowboy->w;
	float cowboyY = state->playerCowboy->y + 28;
	float slope;

	slope = ((cowboyY - mouseY)/(cowboyX - mouseX))*mult;

	vx = -mult;//bullet travels right to left
	if (rand() % 2 == 0)
		vy = slope;
	else vy = (- slope);

	state->pcBullet = newBullet(state->pcCowboy, vx, vy);
	state->pcCowboy->state = 1; //shooting bitmap

}

void pcCollisions(GameState* state){

	int hitEnemy;
	int hitShield;

	hitEnemy = !( ((state->playerCowboy->x + state->playerCowboy->w) < state->pcBullet->x)  ||
			(state->playerCowboy->x > (state->pcBullet->x + state->pcBullet->w))	||
			((state->playerCowboy->y + state->playerCowboy->h) < state->pcBullet->y) 	||
			(state->playerCowboy->y > (state->pcBullet->y + state->pcBullet->h))
	);

	hitShield = !( ((state->shield->x + state->shield->w) < state->pcBullet->x)  ||
			(state->shield->x > (state->pcBullet->x + state->pcBullet->w))	||
			((state->shield->y + state->shield->h) < state->pcBullet->y) 	||
			(state->shield->y > (state->pcBullet->y + state->pcBullet->h))
	);

	if(hitShield){
		state->pcBullet = NULL;
		state->pcCowboy->state = 0;
		return;
	}

	if(hitEnemy){
		state->pcBullet = NULL;
		state->scoretable->PCScore++;
		state->pcCowboy->state = 0;
		return;
	}

	if(state->pcBullet->y < 126){
		state->pcBullet->vy = (- state->pcBullet->vy);
		return;
	}

	if((state->pcBullet->y + state->pcBullet->h) > 573){//h_res margin
		state->pcBullet->vy = (- state->pcBullet->vy);
		return;
	}

	if((state->pcBullet->x) < 20){//v_res-margin
		state->pcBullet = NULL;
		state->pcCowboy->state = 0;
		return;
	}
}

void deleteGameState(GameState* state){
	deleteBitmap(state->background);
	deleteBitmap(state->board);
	deleteBitmap(state->gameWon);

	deleteRectangle(state->fireButton);
	deleteBullet(state->playerBullet);
	deleteBullet(state->pcBullet);
	deleteCowboy(state->playerCowboy);
	deleteCowboy(state->pcCowboy);
	deleteTable(state->scoretable);
	deleteTable(state->countdown);
	deleteShield(state->shield);

	free(state);
}
