#include "cowboy.h"


Cowboy* newCowboy(unsigned int opponent){
	Cowboy* cowboy = (Cowboy*) malloc(sizeof(Cowboy));
	cowboy->opponent = opponent;

	if (opponent){
		cowboy->alive = loadBitmap("right_cowboy.bmp");
		cowboy->shooting = loadBitmap("right_shootin.bmp");
		cowboy->dead = loadBitmap("right_dead.bmp");
		cowboy->x = get_hres()*0.82;
		cowboy->y = (get_vres()*0.5);
	}
	else {
		cowboy->alive = loadBitmap("left_cowboy.bmp");
		cowboy->shooting = loadBitmap("left_shootin.bmp");
		cowboy->dead = loadBitmap("left_dead.bmp");
		cowboy->x = get_hres()*0.125;
		cowboy->y = (get_vres()*0.5);
	}


	cowboy->w = cowboy->alive->bitmapInfoHeader.width;
	cowboy->h = cowboy->alive->bitmapInfoHeader.height;

	cowboy->state = 0;

	return cowboy;
}

void updateCowboy(Cowboy* cowboy, int hor, int vert){
	int vx, vy;

	if(hor > 0) vx = 8;
	if(hor < 0) vx = -8;
	if(hor == 0) vx = 0;

	//remember -> sum positive, cowboy goes down, sum negative, cowboy goes up ONLY IN VERTICAL
	if(vert > 0) vy = -8;
	if(vert < 0) vy = 8;
	if(vert == 0) vy = 0;

	cowboy->x += vx;
	cowboy->y += vy;

	if (cowboy->opponent){
		if(cowboy->x < 525) cowboy->x = 525;
		if((cowboy->x + cowboy->w) > 780) cowboy->x = (780 - cowboy->w);

		if(cowboy->y < 126) cowboy->y = 126;
		if((cowboy->y + cowboy->h) > 573) cowboy->y = (573 - cowboy->h);
	}
	else{
		if(cowboy->x < 20) cowboy->x = 20;
		if((cowboy->x + cowboy->w) > 275) cowboy->x = (275 - cowboy->w);

		if(cowboy->y < 126) cowboy->y = 126;
		if((cowboy->y + cowboy->h) > 573) cowboy->y = (573 - cowboy->h);
	}
}

void drawCowboy(Cowboy* cowboy){
	switch(cowboy->state){
	case 0:
		drawBitmapT(cowboy->alive, cowboy->x, cowboy->y, ALIGN_LEFT);
		break;
	case 1:
		drawBitmapT(cowboy->shooting, cowboy->x, cowboy->y, ALIGN_LEFT);
		break;
	case 2:
		drawBitmapT(cowboy->dead, cowboy->x, cowboy->y, ALIGN_LEFT);
		break;
	default:
		break;
	}
}

void deleteCowboy(Cowboy* cowboy){
	deleteBitmap(cowboy->alive);
	deleteBitmap(cowboy->shooting);
	deleteBitmap(cowboy->dead);

	free(cowboy);
}
