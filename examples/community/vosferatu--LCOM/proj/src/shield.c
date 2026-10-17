#include "shield.h"
#include "graphics.h"
#include <stdlib.h>

Shield* newShield(unsigned int cactus){
	Shield* shield = (Shield*) malloc(sizeof(Shield));

	if (cactus){
		shield->form = loadBitmap("cactus.bmp");
		shield->vy = 0;
	}
	else {
		shield->form = loadBitmap("car.bmp");
		shield->vy = 4;
	}

	shield->w = shield->form->bitmapInfoHeader.width;
	shield->h = shield->form->bitmapInfoHeader.height;

	shield->x = (get_hres()*0.5) - (shield->w/2);
	shield->y = (get_vres()*0.5);

	return shield;
}

void updateShield(Shield* shield){
	if(shield->vy != 0){
		shield->y += shield->vy;

		if((shield->y + shield->h) > 573)
			shield->vy = -shield->vy;
		if(shield->y < 126)
			shield->vy = -shield->vy;
	}
}

void drawShield(Shield* shield){
	drawBitmapT(shield->form, shield->x, shield->y, ALIGN_LEFT);
}

void deleteShield(Shield* shield){
	deleteBitmap(shield->form);

	free(shield);
}
