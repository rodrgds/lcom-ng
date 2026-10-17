#include "Background.h"


Background* newBackground(unsigned int x, unsigned int y)

{

	Background *newB;

	newB=(Background *)malloc(sizeof(Background));

	newB->x=x;

	newB->y=y;

	newB->bitmap="/home/lcom/lcom1617-t1g04/proj/images/background.bmp";


	return newB;
}


Bitmap* loadBackground()
{
	Bitmap *new=loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/background.bmp");
	return new;
}



void drawBackground(Background* player, char *buffer, Bitmap *bmp)
{
	drawBitmap(bmp,player->x,player->y,ALIGN_LEFT, buffer);
}


void DestroyBitmap(Background* newBack)
{
	free(newBack->bitmap);
	free(newBack);
}

