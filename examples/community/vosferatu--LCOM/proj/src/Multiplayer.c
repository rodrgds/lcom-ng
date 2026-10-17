#include "Multiplayer.h"
#include "kbdlab3.h"
#include "keyboard.h"
#include "mouse.h"
#include <stdlib.h>
#include <math.h>

static int once = 1;

Multiplayer* newMultiplayer(){

	Multiplayer* state = (Multiplayer*) malloc(sizeof(Multiplayer));

	state->done = 0;

	state->background = loadBitmap("background.bmp");
	state->board = loadBitmap("game_board.bmp");


    state->player1Bullet = NULL;
    state->player2Bullet = NULL;

    state->player1Cowboy = newCowboy(0); //left cowboy
    state->player2Cowboy = newCowboy(1); //right cowboy

    state->shield = newShield(1); //it's a cactus

    state->scoretable = newTable(1);//opponent use resetTable upon LEVEL CHANGE

    state->level = 0;

    once = 1;

	return state;

}


void updateMultiplayer(Multiplayer* state, unsigned long scancode){


	if (state->level == 1){
		if(scancode == KEY_ESC){
			state->done = 1;
		}
		return;
	}

	if((state->scoretable->playerScore == 5 || state->scoretable->PCScore == 5) && (state->level == 0) && (once == 1)){
		deleteShield(state->shield);
		state->shield = NULL;
		state->shield = newShield(0);
		once = 0;
	}

	if (state->scoretable->playerScore == 12 || state->scoretable->PCScore == 12){
		state->level = 1;
		if (state->scoretable->playerScore > state->scoretable->PCScore)
			state->player2Cowboy->state = 2;
		else state->player1Cowboy->state = 2;

		return;
	}


	int move1 = 0;
	int move2 = 0;

	if(scancode == KEY_ESC){
		state->done = 1;
		return; //END GAME
	} else if(scancode == KEY_S){//PLAYER 1--------------------------------------------------
		updateCowboy(state->player1Cowboy, 0,1);
		move1 = 1;
	} else if(scancode == KEY_Z){
		updateCowboy(state->player1Cowboy, -1,0);
		move1 = 1;
	} else if(scancode == KEY_X){
		updateCowboy(state->player1Cowboy, 0,-1);
		move1 = 1;
	} else if(scancode == KEY_C){
		updateCowboy(state->player1Cowboy, 1,0);
		move1 = 1;
	} else if(scancode == KEY_Q){
		if(state->player1Bullet == NULL){
			player1Fire(state, 1);
		}
	} else if(scancode == KEY_W){
		if(state->player1Bullet == NULL){
			player1Fire(state, 0);
		}
	} else if(scancode == KEY_E){
		if(state->player1Bullet == NULL){
			player1Fire(state, -1);
		}
	} else if(scancode == KEY_L){//PLAYER 2-----------------------------------------------------
		updateCowboy(state->player2Cowboy, 0,1);
		move2 = 1;
	} else if(scancode == KEY_COMMA){
		updateCowboy(state->player2Cowboy, -1,0);
		move2 = 1;
	} else if(scancode == KEY_DOT){
		updateCowboy(state->player2Cowboy, 0,-1);
		move2 = 1;
	} else if(scancode == KEY_MINUS){
		updateCowboy(state->player2Cowboy, 1,0);
		move2 = 1;
	} else if(scancode == KEY_I){
		if(state->player2Bullet == NULL){
			player2Fire(state, 1);
		}
	} else if(scancode == KEY_O){
		if(state->player2Bullet == NULL){
			player2Fire(state, 0);
		}
	} else if(scancode == KEY_P){
		if(state->player2Bullet == NULL){
			player2Fire(state, -1);
		}
	}

	//MOVEMENT---------------------------------------------------------------------------------------------------

	if(move1 && state->player1Cowboy->state == 1){
		state->player1Cowboy->state = 0;
	}

	if(move2 && state->player2Cowboy->state == 1){
		state->player2Cowboy->state = 0;
	}

	updateShield(state->shield);

	if(state->player1Bullet != NULL) player1Collisions(state);

	if(state->player1Bullet != NULL) updateBullet(state->player1Bullet);

	if(state->player2Bullet != NULL) player2Collisions(state);

	if(state->player2Bullet != NULL) updateBullet(state->player2Bullet);

}


void drawMultiplayer(Multiplayer* state){
	if(state->done) return;

	drawBitmap(state->background, 0, 0, ALIGN_LEFT);
	drawBitmapT(state->board, 0, 100, ALIGN_LEFT);

	drawCowboy(state->player2Cowboy);
	drawCowboy(state->player1Cowboy);

	drawShield(state->shield);

	if(state->player1Bullet != NULL) drawBullet(state->player1Bullet);
	if(state->player2Bullet != NULL) drawBullet(state->player2Bullet);

    drawTable(state->scoretable);
}


void deleteMultiplayer(Multiplayer* state){
	deleteBitmap(state->background);
	deleteBitmap(state->board);

	deleteBullet(state->player1Bullet);
	deleteBullet(state->player2Bullet);
	deleteCowboy(state->player1Cowboy);
	deleteCowboy(state->player2Cowboy);
	deleteTable(state->scoretable);
	deleteShield(state->shield);

	free(state);
}



void player1Collisions(Multiplayer* state){
	int hitEnemy;
	int hitShield;

	hitEnemy = !((state->player2Cowboy->x > (state->player1Bullet->x + state->player1Bullet->w)) ||
			((state->player2Cowboy->x + state->player2Cowboy->w) < state->player1Bullet->x) ||
			(state->player2Cowboy->y > (state->player1Bullet->y + state->player1Bullet->h)) ||
			((state->player2Cowboy->y + state->player2Cowboy->h) < state->player1Bullet->y));

	hitShield = !((state->shield->x > (state->player1Bullet->x + state->player1Bullet->w)) ||
			((state->shield->x + state->shield->w) < state->player1Bullet->x) ||
			(state->shield->y > (state->player1Bullet->y + state->player1Bullet->h)) ||
			((state->shield->y + state->shield->h) < state->player1Bullet->y));

	if(hitShield){
		state->player1Bullet = NULL;
		state->player1Cowboy->state = 0;
		return;
	}

	if(hitEnemy){
		state->player1Bullet = NULL;
		state->scoretable->playerScore++;
		state->player1Cowboy->state = 0;
		return;
	}

	if(state->player1Bullet->y < 126){
		state->player1Bullet->vy = - state->player1Bullet->vy;
		return;
	}

	if((state->player1Bullet->y + state->player1Bullet->h) > 573){//h_res margin
		state->player1Bullet->vy = - state->player1Bullet->vy;
		return;
	}

	if((state->player1Bullet->x + state->player1Bullet->w) > 780){//v_res-margin
		state->player1Bullet = NULL;
		state->player1Cowboy->state = 0;
		return;
	}

}


void player1Fire(Multiplayer* state, int direction){

	int vx;
	float vy;

	if (direction == 0) vy = 0;
	if (direction > 0) vy = 7;
	if (direction < 0) vy = -7;
	vx = 7;
	if(state->scoretable->playerScore > 4){
		vx +=2;
		if(vy > 0) vy+=2;
		if(vy<0) vy-= 2;
	}


	state->player1Bullet = newBullet(state->player1Cowboy, vx, vy);
	state->player1Cowboy->state = 1; //shooting bitmap

}


void player2Fire(Multiplayer* state, int direction){

	int vx;
	float vy;

	vx = -7;

	if (direction == 0) vy = 0;
	if (direction > 0) vy = 7;
	if (direction < 0) vy = -7;

	if(state->scoretable->PCScore > 4){
		vx -=2;
		if(vy > 0) vy+=2;
		if(vy<0) vy-= 2;
	}


	state->player2Bullet = newBullet(state->player2Cowboy, vx, vy);
	state->player2Cowboy->state = 1; //shooting bitmap

}


void player2Collisions(Multiplayer* state) {

	int hitEnemy;
	int hitShield;

	hitEnemy = !( ((state->player1Cowboy->x + state->player1Cowboy->w) < state->player2Bullet->x)  ||
			(state->player1Cowboy->x > (state->player2Bullet->x + state->player2Bullet->w))	||
			((state->player1Cowboy->y + state->player1Cowboy->h) < state->player2Bullet->y) 	||
			(state->player1Cowboy->y > (state->player2Bullet->y + state->player2Bullet->h))
	);

	hitShield = !( ((state->shield->x + state->shield->w) < state->player2Bullet->x)  ||
			(state->shield->x > (state->player2Bullet->x + state->player2Bullet->w))	||
			((state->shield->y + state->shield->h) < state->player2Bullet->y) 	||
			(state->shield->y > (state->player2Bullet->y + state->player2Bullet->h))
	);

	if(hitShield){
		state->player2Bullet = NULL;
		state->player2Cowboy->state = 0;
		return;
	}

	if(hitEnemy){
		state->player2Bullet = NULL;
		state->scoretable->PCScore++;
		state->player2Cowboy->state = 0;
		return;
	}

	if(state->player2Bullet->y < 126){
		state->player2Bullet->vy = (- state->player2Bullet->vy);
		return;
	}

	if((state->player2Bullet->y + state->player2Bullet->h) > 573){//h_res margin
		state->player2Bullet->vy = (- state->player2Bullet->vy);
		return;
	}

	if((state->player2Bullet->x) < 20){//v_res-margin
		state->player2Bullet = NULL;
		state->player2Cowboy->state = 0;
		return;
	}

}
