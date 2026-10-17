#include "Obstacles.h"

#include <time.h>

#define BACKGROUND_SPEED    3

Obstacles *newObstacle(int x,int y){

	Obstacles *Obs=(Obstacles*)malloc(sizeof(Obstacles));
	Obs->x=x;
	Obs->y=y;
	Obs->use=0;
	Obs->type=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/obs120.bmp");
	Obs->ht=Obs->type->bitmapInfoHeader.height;
	Obs->wh=Obs->type->bitmapInfoHeader.width;
	Obs->vel=BACKGROUND_SPEED;
	return Obs;
}

void updateObstacle(Obstacles* obs){
	obs->y += obs->vel;
	if (obs->y >= getVerResolution()) {
		obs->y = 0;
		obs->use = rand() % 2;
	}
}

void drawObstacle(Obstacles* obs) {
	drawBitmap(obs->type, obs->x, obs->y, ALIGN_LEFT);
}

void deleteObstacle(Obstacles* obs) {
	if (obs == NULL)
		return;
	deleteBitmap(obs->type);
	free(obs);
}

void updateObstacleUse(Obstacles* obs) {
	obs->use = rand() % 2;
}

void updateObstacleSpeed(Obstacles* obs,int incSpeed){
	if(incSpeed==1)
		obs->vel++;
}
