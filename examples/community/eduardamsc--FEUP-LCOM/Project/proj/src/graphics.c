#include "graphics.h"
#include <stdlib.h>
#include "i8042.h"
#include "i8254.h"
#include "stdio.h"

void initMouse() {
	mouse.x = H_RES/2;
	mouse.y = V_RES/2;
	mouse.leftButton = 0;
	mouse.rightButton = 0;
	mouse.midButton = 0;
	mouse.packetIndex = 0;
	mouse.packetReady = 0;
	mouse.mouseUpdated = 0;
}

void initGame(GameState* state) {
	car.x = 10;
	car.y = state->laneY[1];
	car.width = state->car->bitmapInfoHeader.width;
	car.height = state->car->bitmapInfoHeader.height;

	rock.lane = rand() % 3;
	rock.x = H_RES;
	rock.y = state->laneY[rock.lane];
	rock.width = state->rock->bitmapInfoHeader.width;
	rock.height = state->rock->bitmapInfoHeader.height;
	rock.destroyed = 0;
	rock.ready = 1;
	do {
		coin1.lane = rand() % 3;
	} while(coin1.lane == rock.lane);
	coin1.x = H_RES;
	coin1.y = state->laneY[coin1.lane];
	coin1.width = state->coin->bitmapInfoHeader.width;
	coin1.height = state->coin->bitmapInfoHeader.height;
	coin1.destroyed = 0;
	coin1.ready = 1;


	cone.lane = rand() % 3;
	cone.y = state->laneY[cone.lane];
	cone.x = H_RES;
	cone.width = state->cone->bitmapInfoHeader.width;
	cone.height = state->cone->bitmapInfoHeader.height;
	cone.destroyed = 0;
	cone.ready = 0;

	do {
		coin2.lane = rand() % 3;
	} while(coin2.lane == cone.lane);
	coin2.x = H_RES;
	coin2.y = state->laneY[coin2.lane];
	coin2.width = state->coin->bitmapInfoHeader.width;
	coin2.height = state->coin->bitmapInfoHeader.height;
	coin2.destroyed = 0;
	coin2.ready = 0;

	hole.lane = rand() % 3;
	hole.y = state->laneY[hole.lane];
	hole.x = H_RES;
	hole.width = state->hole->bitmapInfoHeader.width;
	hole.height = state->hole->bitmapInfoHeader.height;
	hole.destroyed = 0;
	hole.ready = 0;

	do {
		coin3.lane = rand() % 3;
	} while(coin3.lane == hole.lane);
	coin3.x = H_RES;
	coin3.y = state->laneY[coin3.lane];
	coin3.width = state->coin->bitmapInfoHeader.width;
	coin3.height = state->coin->bitmapInfoHeader.height;
	coin3.destroyed = 0;
	coin3.ready = 0;
}

int checkCollision() {
	int car_xf = car.x + car.width;
	int car_yf = car.y + car.height;
	int rock_xf = rock.y + rock.width;
	int rock_yf = rock.y + rock.height;
	int cone_xf = cone.x + cone.width;
	int cone_yf = cone.y + cone.height;
	int hole_xf = hole.x + hole.width;
	int hole_yf = hole.y + hole.height;
	int coin1_xf = coin1.x + coin1.width;
	int coin1_yf = coin1.y + coin1.height;
	int coin2_xf = coin2.x + coin2.width;
	int coin2_yf = coin2.y + coin2.height;
	int coin3_xf = coin3.x + coin3.width;
	int coin3_yf = coin3.y + coin3.height;


	if(rock.destroyed == 0)
		if((car_xf > rock.x && car.x < rock_xf) && !(car_yf < rock.y) && !(car.y > rock_yf)) {
			rock.destroyed = 1;
			return 1;
		}
	if(cone.destroyed == 0)
		if((car_xf > cone.x && car.x < cone_xf) && !(car_yf < cone.y) && !(car.y > cone_yf)) {
			cone.destroyed = 1;
			return 1;
		}
	if(hole.destroyed == 0)
		if((car_xf > hole.x && car.x < hole_xf) && !(car_yf < hole.y) && !(car.y > hole_yf)) {
			hole.destroyed = 1;
			return 1;
		}

	if(coin1.destroyed == 0)
		if((car_xf > coin1.x && car.x < coin1_xf) && !(car_yf < coin1.y) && !(car.y > coin1_yf)) {
			coin1.destroyed = 1;
			return 2;
		}

	if(coin2.destroyed == 0)
		if((car_xf > coin2.x && car.x < coin2_xf) && !(car_yf < coin2.y) && !(car.y > coin2_yf)) {
			coin2.destroyed = 1;
			return 2;
		}

	if(coin3.destroyed == 0)
		if((car_xf > coin3.x && car.x < coin3_xf) && !(car_yf < coin3.y) && !(car.y > coin3_yf)) {
			coin3.destroyed = 1;
			return 2;
		}

	return 0;
}

void updateMouse(ObstacleCourse* course) {
	if (mouse.packet[0] & LEFTB)
		mouse.leftButton = 1;
	else
		mouse.leftButton = 0;

	if (mouse.packet[0] & RIGHTB)
		mouse.rightButton = 1;
	else
		mouse.rightButton = 0;

	if (mouse.packet[0] & MIDB)
		mouse.midButton = 1;
	else
		mouse.midButton = 0;

	if (mouse.packet[0] & XSIGN) {
		mouse.x = mouse.x - ((mouse.packet[1] ^= 0xFF) + 1);
		if (mouse.x < 0)
			mouse.x = 0;

	} else {
		mouse.x = mouse.x + mouse.packet[1];
		if (mouse.x > H_RES)
			mouse.x = H_RES;

	}
	if (mouse.packet[0] & YSIGN) {
		mouse.y = mouse.y + ((mouse.packet[2] ^= 0xFF) + 1);
		if (mouse.y < 0)
			mouse.y = 0;
	} else {
		mouse.y = mouse.y - mouse.packet[2];
		if (mouse.y > V_RES)
			mouse.y = V_RES;
	}

	mouse.mouseUpdated = 1;
}

int mouseInButton(Rectangle* rect) {
	return (mouse.x > rect->xi && mouse.y > rect->yi && mouse.x < rect->xf
			&& mouse.y < rect->yf);
}

void mouseMove(GameState* state) {
	if (mouse.packet[0] & YSIGN) {
		car.y += ((mouse.packet[2]) + 1);
	} else {
		car.y -= mouse.packet[2];
	}
	mouse.mouseUpdated = 0;
}

void drawScore(GameState* state) {
	int digit1 = state->points % 10;
	drawDigit(state, digit1, 600 + state->scoreWidth + 150, 15);
	int digit2 = (state->points /10) % 10;
	drawDigit(state, digit2, 600 + state->scoreWidth + 90,15);
	int digit3 = (state->points / 100) % 10;
	drawDigit(state, digit3, 600 + state->scoreWidth + 30,15);
}

void drawDate(GameState* state) {
	int y = 620;
	int digit1 =  *(state->year) % 10;
	int digit2 = (*(state->year) /10) % 10;
	int digit3 = (*(state->year) / 100) % 10;
	int digit4 = (*(state->year) / 1000) % 10;
	drawDigit(state, digit1, 180 + 180,y);
	drawDigit(state, digit2, 180 + 120,y);
	drawDigit(state, digit3, 180 + 60,y);
	drawDigit(state, digit4, 180 + 0,y);

	digit1 =  *(state->month) % 10;
	digit2 = (*(state->month) /10) % 10;
	drawDigit(state, digit1, 480 + 60,y);
	drawDigit(state, digit2, 480,y);

	digit1 =  *(state->day) % 10;
	digit2 = (*(state->day) /10) % 10;
	drawDigit(state, digit1, 660 + 60,y);
	drawDigit(state, digit2, 660,y);

	digit1 = *(state->hour) % 10;
	digit2 = (*(state->hour) / 10) % 10;
	drawDigit(state, digit1, 260 + 60,y + 80);
	drawDigit(state, digit2, 260,y + 80);

	digit1 = *(state->minute) % 10;
	digit2 = (*(state->minute) / 10) % 10;
	drawDigit(state, digit1, 420 + 60,y + 80);
	drawDigit(state, digit2, 420,y + 80);

	digit1 = *(state->seconds) % 10;
	digit2 = (*(state->seconds) / 10) % 10;
	drawDigit(state, digit1, 600 + 60,y + 80);
	drawDigit(state, digit2, 600,y + 80);
}

void drawDigit(GameState* state, int digit, int deviation, int y) {
	switch (digit) {
	case 0:
		drawBitmap(state->zero, deviation, y, ALIGN_LEFT);
		break;
	case 1:
		drawBitmap(state->um, deviation, y, ALIGN_LEFT);
		break;
	case 2:
		drawBitmap(state->dois, deviation, y, ALIGN_LEFT);
		break;
	case 3:
		drawBitmap(state->tres, deviation, y, ALIGN_LEFT);
		break;
	case 4:
		drawBitmap(state->quatro, deviation, y, ALIGN_LEFT);
		break;
	case 5:
		drawBitmap(state->cinco, deviation, y, ALIGN_LEFT);
		break;
	case 6:
		drawBitmap(state->seis, deviation, y, ALIGN_LEFT);
		break;
	case 7:
		drawBitmap(state->sete, deviation, y, ALIGN_LEFT);
		break;
	case 8:
		drawBitmap(state->oito, deviation, y, ALIGN_LEFT);
		break;
	case 9:
		drawBitmap(state->nove, deviation, y, ALIGN_LEFT);
		break;
	}
}

