#include "target.h"

Target* newTarget(){
	Target* target = (Target*) malloc(sizeof(Target));

	target->hit = 0;

	target->bullseye = loadBitmap("target.bmp");

	target->x = get_hres()*0.875;
	target->y = (get_vres()*0.5);


	target->w = target->bullseye->bitmapInfoHeader.width;
	target->h = target->bullseye->bitmapInfoHeader.height;

	return target;
}

void updateTarget(Target* target){
	if(target->hit){
		target-> x = 525 + (rand() % 300);
		target-> y = 125 + (rand() % 500);

		if(target->x < 525) target->x = 525;
		if((target->x + target->w) > 780) target->x = (780 - target->w);

		if(target->y < 126) target->y = 126;
		if((target->y + target->h) > 573) target->y = (573 - target->h);

		target->hit = 0;
	}
}

void drawTarget(Target* target){
	drawBitmapT(target->bullseye, target->x, target->y, ALIGN_LEFT);
}

void deleteTarget(Target* target){
	deleteBitmap(target->bullseye);

	free(target);
}
