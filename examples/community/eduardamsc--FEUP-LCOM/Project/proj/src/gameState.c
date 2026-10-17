#include "gameState.h"
#include <minix/drivers.h>
#include "utilities.h"
#include "i8042.h"
#include "graphics.h"
#include "rtc.h"
#include "obstacleCourse.h"


GameState* newGameState() {
	GameState* state = (GameState*) malloc(sizeof(GameState));

	state->done = 0;

	state->backgroundX = 0;
	state->velocity = 4;
	state->laneY[0] = 168;
	state->laneY[1] = 328;
	state->laneY[2] = 488;
	state->lives = 5;
	state->points = 0;
	state->gameover = 0;
	state->gotRtc = 0;

	state->day = (unsigned long*) malloc(sizeof(unsigned long));
	state->month = (unsigned long*) malloc(sizeof(unsigned long));
	state->year = (unsigned long*) malloc(sizeof(unsigned long));
	state->hour = (unsigned long*) malloc(sizeof(unsigned long));
	state->minute = (unsigned long*) malloc(sizeof(unsigned long));
	state->seconds = (unsigned long*) malloc(sizeof(unsigned long));

	state->background = loadBitmap(getImagePath("backgroundNoStripes"));
	state->riscas = loadBitmap(getImagePath("riscas"));
	state->car = loadBitmap(getImagePath("car"));
	state->rock = loadBitmap(getImagePath("rock"));
	state->rock2 = loadBitmap(getImagePath("rock2"));
	state->cone = loadBitmap(getImagePath("cone"));
	state->cone2 = loadBitmap(getImagePath("cone2"));
	state->heart = loadBitmap(getImagePath("heart"));
	state->coin = loadBitmap(getImagePath("coin"));
	state->hole = loadBitmap(getImagePath("hole"));
	state->hole2 = loadBitmap(getImagePath("hole2"));
	state->score = loadBitmap(getImagePath("score"));
	state->gameoverbmp = loadBitmap(getImagePath("gameOver"));


	state->zero = loadBitmap(getImagePath("0"));
	state->um = loadBitmap(getImagePath("1"));
	state->dois = loadBitmap(getImagePath("2"));
	state->tres = loadBitmap(getImagePath("3"));
	state->quatro = loadBitmap(getImagePath("4"));
	state->cinco = loadBitmap(getImagePath("5"));
	state->seis = loadBitmap(getImagePath("6"));
	state->sete = loadBitmap(getImagePath("7"));
	state->oito = loadBitmap(getImagePath("8"));
	state->nove = loadBitmap(getImagePath("9"));

	state->scoreWidth = state->score->bitmapInfoHeader.width;
	initGame(state);
	return state;
}

void moveBackground(GameState* state) {
	state->backgroundX -= state->velocity;
	if(rock.ready) rock.x -= state->velocity;
	if(cone.ready) cone.x -= state->velocity;
	if(hole.ready) hole.x -= state->velocity;
	if(coin1.ready) coin1.x -= state->velocity;
	if(coin2.ready) coin2.x -= state->velocity;
	if(coin3.ready) coin3.x -= state->velocity;

	int backgroundWidth = state->riscas->bitmapInfoHeader.width;
	if(state->backgroundX + backgroundWidth <= H_RES)
		state->backgroundX = 0;

	//TODO fazer uma funçao generica para estes 3 ifs se der tempo
	if(!coin1.ready && !rock.ready && hole.x + car.width + hole.width + 100 <= H_RES) {
		rock.lane = rand() % 3;
		rock.x = H_RES;
		rock.y = state->laneY[rock.lane];
		rock.destroyed = 0;
		rock.ready = 1;
		do {
			coin1.lane = rand() % 3;
		} while(coin1.lane == rock.lane);
		coin1.x = H_RES;
		coin1.y = state->laneY[coin1.lane];
		coin1.destroyed = 0;
		coin1.ready = 1;
	}

	if(!coin2.ready && !cone.ready && rock.x + car.width + rock.width + 100 <= H_RES) {
		cone.lane = rand() % 3;
		cone.x = H_RES;
		cone.y = state->laneY[cone.lane];
		cone.destroyed = 0;
		cone.ready = 1;
		do {
			coin2.lane = rand() % 3;
		} while(coin2.lane == cone.lane);
		coin2.x = H_RES;
		coin2.y = state->laneY[coin2.lane];
		coin2.destroyed = 0;
		coin2.ready = 1;
	}

	if(!coin3.ready && !hole.ready && coin2.x + car.width + coin2.width + 100 <= H_RES) {
		hole.lane = rand() % 3;
		hole.x = H_RES;
		hole.y = state->laneY[hole.lane];
		hole.destroyed = 0;
		hole.ready = 1;
		do {
			coin3.lane = rand() % 3;
		} while(coin3.lane == hole.lane);
		coin3.x = H_RES;
		coin3.y = state->laneY[coin3.lane];
		coin3.destroyed = 0;
		coin3.ready = 1;
	}
}

void updateGameState(GameState* state, unsigned long scancode) {
	if(scancode == ESC)
		state->done = 1;

	if(!state->gameover) {
		if (scancode == KEY_W) {
			if(car.y <= state->laneY[1])
				car.y -= (car.y - state->laneY[0]);
			else
				car.y -= (car.y - state->laneY[1]);
		} else if (scancode == KEY_S) {
			if(car.y >= state->laneY[1])
				car.y += (state->laneY[2] - car.y);
			else
				car.y += (state->laneY[1] - car.y);
		}

		if(mouse.mouseUpdated)
			mouseMove(state);

		if (car.y < state->laneY[0]) car.y = state->laneY[0];
		if (car.y > state->laneY[2]) car.y = state->laneY[2];

		moveBackground(state);

		int collisionType = checkCollision();
		if(collisionType == 1) {
			state->lives--;
		} else if (collisionType == 2) {
			state->points += 1;
		}
		if(rock.x + rock.width <= 0) rock.ready = 0;
		if(coin1.x + coin1.width <= 0) coin1.ready = 0;
		if(cone.x + cone.width <= 0) cone.ready = 0;
		if(coin2.x + coin2.width <= 0) coin2.ready = 0;
		if(hole.x + hole.width <= 0) hole.ready = 0;
		if(coin3.x + coin3.width <= 0) coin3.ready = 0;
		if(state->lives == 0) {
			state->gameover = 1;
		}
	} else if(!state->gotRtc) {
		getDate(state->day, state->month, state->year, state->hour, state->minute, state->seconds);
		state->gotRtc = 1;
	}
}

void drawGameState(GameState* state) {
	drawBitmap(state->background, 0, 0, ALIGN_LEFT);
	drawBitmap(state->riscas, state->backgroundX, 290, ALIGN_LEFT);
	drawBitmap(state->riscas, state->backgroundX, 450, ALIGN_LEFT);
	if(rock.ready) {
		if(rock.destroyed == 0)
			drawBitmap(state->rock, rock.x, rock.y, ALIGN_LEFT);
		else drawBitmap(state->rock2, rock.x, rock.y, ALIGN_LEFT);
	}
	if(coin1.destroyed == 0 && coin1.ready)
		drawBitmap(state->coin, coin1.x, coin1.y, ALIGN_LEFT);
	if(cone.ready) {
		if(cone.destroyed == 0 && cone.ready)
			drawBitmap(state->cone, cone.x, cone.y, ALIGN_LEFT);
		else drawBitmap(state->cone2, cone.x, cone.y, ALIGN_LEFT);
	}
	if(coin2.destroyed == 0 && coin2.ready)
		drawBitmap(state->coin, coin2.x, coin2.y, ALIGN_LEFT);

	if(hole.ready) {
		if(hole.destroyed == 0)
			drawBitmap(state->hole, hole.x, hole.y, ALIGN_LEFT);
		else drawBitmap(state->hole2, hole.x, hole.y, ALIGN_LEFT);
	}

	if(coin3.destroyed == 0 && coin3.ready)
		drawBitmap(state->coin, coin3.x, coin3.y, ALIGN_LEFT);

	drawBitmap(state->car, car.x, car.y, ALIGN_LEFT);
	int i;
	for(i = 0; i < state->lives; i++) {
		drawBitmap(state->heart, 25 + i*(state->heart->bitmapInfoHeader.width + 10), 25, ALIGN_LEFT);
	}
	drawBitmap(state->score, 600, 10, ALIGN_LEFT);
	drawScore(state);

	if(state->gameover) {
		drawBitmap(state->gameoverbmp, 0, 0, ALIGN_LEFT);
		drawDate(state);
	}

}

void deleteGameState(GameState* state) {
	deleteBitmap(state->background);
	deleteBitmap(state->riscas);
	deleteBitmap(state->car);
	deleteBitmap(state->rock);
	deleteBitmap(state->rock2);
	deleteBitmap(state->cone);
	deleteBitmap(state->cone2);
	deleteBitmap(state->hole);
	deleteBitmap(state->hole2);
	deleteBitmap(state->coin);
	deleteBitmap(state->heart);
	deleteBitmap(state->zero);
	deleteBitmap(state->um);
	deleteBitmap(state->dois);
	deleteBitmap(state->tres);
	deleteBitmap(state->quatro);
	deleteBitmap(state->cinco);
	deleteBitmap(state->seis);
	deleteBitmap(state->sete);
	deleteBitmap(state->oito);
	deleteBitmap(state->nove);
	free(state);
}
