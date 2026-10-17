#include "bullet.h"
#include <stdlib.h>

Bullet* newBullet(Cowboy* cowboy, int vx, float vy){
	Bullet* bullet = (Bullet*) malloc(sizeof(Bullet));

	if (cowboy->opponent){
		bullet->image = loadBitmap("right_shot.bmp");
		bullet->w = bullet->image->bitmapInfoHeader.width;
		bullet->h = bullet->image->bitmapInfoHeader.height;

		bullet->x = cowboy->x - bullet->w;

	}
	else {
		bullet->image = loadBitmap("left_shot.bmp");
		bullet->w = bullet->image->bitmapInfoHeader.width;
		bullet->h = bullet->image->bitmapInfoHeader.height;
		bullet->x = cowboy->x + cowboy->w;
	}

	bullet->y = cowboy->y + 28;//placing in sprite

	bullet->vx = vx;
	bullet->vy = vy;//y velocity

	return bullet;
}

void updateBullet(Bullet* bullet){
	//incomplete, maybe change the vy in game function
	bullet->x += bullet->vx;
	bullet->y += bullet->vy;
}

void drawBullet(Bullet* bullet){
	drawBitmapT(bullet->image, bullet->x, bullet->y, ALIGN_LEFT);
}

void deleteBullet(Bullet* bullet){

	if(bullet != NULL) deleteBitmap(bullet->image);

	free(bullet);
}
