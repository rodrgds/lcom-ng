#include "hero.h"
#include "Bitmap.h"
#include "utils.h"
#include "mouse.h"

struct Hero {
	Bitmap *hero_image;
	int x, y;
	double speed_y;
	int number_of_arrows;
	int balloons_destroyed;
	hero_state h_state;
};

hero_t* createHero()
{
	hero_t *h = (hero_t *) malloc( sizeof(hero_t) );
	h->hero_image = loadBitmap("/home/lcom/proj/src/resources/hero_rest_pos.bmp");
	h->x = HERO_STARTING_POS_X;
	h->y = HERO_STARTING_POS_Y;
	h->number_of_arrows = STARTING_ARROWS;
	h->h_state = REST_WITH_ARROW;
	h->balloons_destroyed = 0;
	return h;
}


/* --------------------- GET -----------------------*/

int getNumberOfArrows(hero_t *h)
{
	return h->number_of_arrows;
}

int getHeroXPos(hero_t *h)
{
	return h->x;
}

int getHeroYPos(hero_t *h)
{
	return h->y;
}

int getNumberOfBalloonsDestroyed(hero_t *h) {
	return h->balloons_destroyed;
}

/* --------------------- SET ----------------------*/

void setHeroPoweringUp(hero_t *h)
{
	if (h->number_of_arrows <= 0)
		return;
	h->h_state = POWERING_UP;
	h->hero_image = loadBitmap("/home/lcom/proj/src/resources/hero_mid_position.bmp");
}

void setHeroFullPower(hero_t *h)
{
	if (h->number_of_arrows <= 0)
		return;
	h->h_state = FULL_POWER;
	h->hero_image = loadBitmap("/home/lcom/proj/src/resources/hero_power_position.bmp");
}

void setHeroRestingWithoutArrow(hero_t * h)
{
	h->h_state = REST_NO_ARROW;
	h->hero_image = loadBitmap("/home/lcom/proj/src/resources/hero_no_arrow.bmp");
}

void setHeroRestingWithArrow(hero_t * h) {
	h->h_state = REST_WITH_ARROW;
	h->hero_image = loadBitmap("/home/lcom/proj/src/resources/hero_rest_pos.bmp");
}

void setHeroWithoutAnyArrow(hero_t *h) {
	h->h_state = WITHOUT_ANY_ARROW;
	h->hero_image = loadBitmap("/home/lcom/proj/src/resources/hero_without_arrows.bmp");
}

void setHeroArrowNumber(hero_t *h, int number) {
	h->number_of_arrows = number;
}


/* --------------------- BOOLEAN ------------------------ */

int heroMovementInsideScreen(hero_t *h) {
	int heroNextPos_Y = h->y - mouse_t.pos_y;
	if (heroNextPos_Y <= 662 && heroNextPos_Y >= 69) {
		return 1;
	}
	return 0;
}

int isHeroFullPower(hero_t *h) {
	if (h->h_state == FULL_POWER)
		return 1;
	return 0;
}

int isHeroRestingWithoutArrow(hero_t *h) {
	if (h->h_state == REST_NO_ARROW)
		return 1;
	return 0;
}

int isHeroRestingWithArrow(hero_t *h) {
	if (h->h_state == REST_WITH_ARROW)
		return 1;
	return 0;
}

/* ---------------------------------------------------*/

void releaseArrow(hero_t *h)
{
	h->number_of_arrows--;
}

void heroDestroyedBalloon(hero_t *h) {
	h->balloons_destroyed = h->balloons_destroyed + 1;
}

void deleteHero(hero_t *h)
{
	free(h);
}

void drawHero(hero_t *h)
{
	drawBitmap(h->hero_image,h->x,h->y,ALIGN_CENTER);
}

void updateHeroPosition(hero_t *h)
{
	h->y = h->y - mouse_t.pos_y;
}






