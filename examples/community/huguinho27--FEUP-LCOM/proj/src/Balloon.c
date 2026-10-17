#include "Balloon.h"
#include "Bitmap.h"

struct Balloon
{
	Bitmap *balloon_image; 	//bitmap representing a balloon
	int x; 					//x position of the balloon
	int y; 					//y position of the balloon
	balloon_state b_state; 	//state of the balloon
	int active;				//for 2nd level to know if the balloon was activated
};

balloon_t* createBalloon(int xIN, int yIN)
{
	balloon_t *b = (balloon_t *) malloc(sizeof(balloon_t));
	b->balloon_image = loadBitmap("/home/lcom/proj/src/resources/solid_balloon.bmp");
	b->b_state = INTACT;
	b->x = xIN;
	b->y = yIN;
	b->active = 0;
	return b;
}

/* ------------------ BOOLEAN ----------------- */

int isBalloonActive(balloon_t *b) {
	if (b->active == 1)
		return 1;
	return 0;
}

int isBalloonDestroyed(balloon_t *b)
{
	if (b->b_state == DESTROYED)
		return 1;
	return 0;
}

int isBalloonBlown(balloon_t *b)
{
	if (b->b_state == BLOWN)
		return 1;
	return 0;
}

int isBalloonIntact(balloon_t *b) {
	if (b->b_state == INTACT)
		return 1;
	return 0;
}

/* ------------------ GET ------------------ */

int getBalloonPos_X(balloon_t *b)
{
	return b->x;
}

int getBalloonPos_Y(balloon_t *b)
{
	return b->y;
}

/* ------------------- SET ----------------- */

void setBalloonActive(balloon_t *b) {
	b->active = 1;
}

void setBalloonHit(balloon_t *b)
{
	if (b->y > 780)
	{
		destroyBalloon(b);
		return;
	}

	b->b_state = BLOWN;
	b->balloon_image = loadBitmap("/home/lcom/proj/src/resources/blown_balloon.bmp");
}

void resetBalloonPosition(balloon_t *b)
{
	if (b->y < -50)
		b->y = 1000;
}

/* --------------------------------------------*/

void updateBalloonPosition(balloon_t *b)
{
	if (b->b_state == BLOWN && b->y >= 768){
		destroyBalloon(b);
		return;
	}
	if (b->b_state == INTACT)
		b->y -= 1;
	else
		b->y += 3;
}

void drawBalloon(balloon_t *b)
{
	if (b->b_state != DESTROYED)
		drawBitmap(b->balloon_image, b->x, b->y, ALIGN_CENTER);
}



void destroyBalloon(balloon_t *b)
{
	b->b_state = DESTROYED;
}
