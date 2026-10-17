#include "Mario.h"

mario* newMario(unsigned int x, unsigned int y)
{
	mario *new;
	new=(mario *)malloc(sizeof(mario));

	new->x=x;
	new->y=y;

	return new;
}

Bitmap* loadMario(char *source)
{
	Bitmap *new=loadBitmap(source);
	return new;
}




void drawMario(mario* player, char *buffer, Bitmap *bmp)
{
	drawBitmap(bmp, player->x,player->y ,ALIGN_CENTER, buffer);
}


void DestroyMario(mario *auxMario)
{
	free(auxMario->imageOne);
	free(auxMario->imageTwo);
	free(auxMario);
}

