#include "arrow.h"
#include "Bitmap.h"

struct Arrow
{
	Bitmap *arrow_image; 	//bitmap representing an arrow
	int x; 					//x position of the arrow
	int y; 					//y position of the arrow
	int inside_screen;			//if 1 it is still on screen, 0 not
};

arrow_t* createArrow(int xIN, int yIN)
{
	arrow_t *arrow = (arrow_t *) malloc(sizeof(arrow_t));
	arrow->arrow_image = loadBitmap("/home/lcom/proj/src/resources/arrow.bmp");
	arrow->x = xIN;
	arrow->y = yIN;
	arrow->inside_screen = 1;
	return arrow;
}
/* --------------------------- get ----------------------- */

int getBeingUsed(arrow_t *a)
{
	return a->inside_screen;
}

int getArrowPos_X(arrow_t *a)
{
	return a->x;
}

int getArrowPos_Y(arrow_t *a)
{
	return a->y;
}

/* ----------------------------------------------------------*/

int isArrowInsideScreen(arrow_t *a)
{
	return a->inside_screen;
}

void drawArrow(arrow_t *a)
{
	drawBitmap(a->arrow_image, a->x, a->y, ALIGN_CENTER);
}

void updateArrowPosition(arrow_t *a)
{
	if ((a->x+25) > 1024)
	{
		a->inside_screen = 0;
		return;
	}
	a->x += 3;
}
