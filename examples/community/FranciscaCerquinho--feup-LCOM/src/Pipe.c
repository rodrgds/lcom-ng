#include "Pipe.h"


int i=0;

Pipe* newPipe(unsigned int x, unsigned int y)
{
	Pipe *newPipe;
	newPipe=(Pipe *)malloc(sizeof(Pipe));

	newPipe->x=x;
	newPipe->y=599;
	newPipe->image=loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/pipe.bmp");
	return newPipe;

}


Bitmap* loadPipe()
{
	Bitmap *new=loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/pipe.bmp");
	return new;
}


void drawPipe(Pipe* player, char *buffer, Bitmap *bmp)
{
	drawBitmap(bmp, player->x,player->y ,ALIGN_RIGHT, buffer);
}

