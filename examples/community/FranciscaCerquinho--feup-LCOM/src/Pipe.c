#include "Pipe.h"


int i=0;

Pipe* newPipe(unsigned int x, unsigned int y)
{
	// newPipe is called once per timer tick; load the bitmap only once instead
	// of reopening the file and leaking its pixels every frame.
	static Bitmap *pipeImage = NULL;
	Pipe *newPipe;
	newPipe=(Pipe *)malloc(sizeof(Pipe));

	if (pipeImage == NULL)
		pipeImage = loadBitmap("/home/lcom/lcom1617-t1g04/proj/images/pipe.bmp");

	newPipe->x=x;
	newPipe->y=599;
	newPipe->image=pipeImage;
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

