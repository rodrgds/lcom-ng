#include "Pokeball.h"

#include <time.h>

#define BACKGROUND_SPEED    3

Pokeball* newPoke(int x,int y){
	Pokeball* pb=(Pokeball*)malloc(sizeof(Pokeball));
	pb->x=x;
	pb->y=y;
	pb->val=1;
	pb->vel=BACKGROUND_SPEED;
	pb->poke=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/poke.bmp");
	pb->ht=pb->poke->bitmapInfoHeader.height;
	pb->wh=pb->poke->bitmapInfoHeader.width;
	pb->use=rand()%2;
	return pb;
}

void updatePoke(Pokeball* pb){
	pb->y += pb->vel;
	if (pb->y >= getVerResolution()) {
		pb->y = 0;
		pb->use = rand() % 2;
		pb->val=1;
	}
}

void drawPoke(Pokeball* pb){
	drawBitmap(pb->poke, pb->x, pb->y, ALIGN_LEFT);
}

void deletePoke(Pokeball* pb){
	if(pb==NULL)
		return;
	deleteBitmap(pb->poke);
	free(pb);
}

void updatePokeUse(Pokeball* pb){
	pb->use=rand()%2;
}

void updatePokeSpeed(Pokeball* pb,int incSpeed){
	if(incSpeed==1)
		pb->vel++;
}
