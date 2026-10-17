#include "boat.h"
#include "data_pixmap.h"
#include "pixmap.h"
//#include "vector2.h"

/** @defgroup boat boat
 * @{*/

static const float speed=3.0f;///<Boat default speed

//area used by the sharks
//limits do not have to be equal to resolution
static const int X_MAX = 1024;///<boat area right side bound
static const int X_MIN = 0;///<boat area left side bound
static const int Y_MAX = 768;///<boat area lower bound
static const int Y_MIN = 0;///<boat area upper bound


boat createBoat()
{
	Pixmap pm;
	pm.pixmap = read_xpm(ship,&pm.width,&pm.height);
	boat b1;
	b1.position = Vector2(800,500);
	b1.colRad = pm.width/2-5;
	b1.sp = createStaticSprite((int)b1.position.x,(int)b1.position.y,
			pm.width/2, pm.height/2,addPixmap(&pm));
	addSprite2DrawList(b1.sp);
	return b1;
}

void updateBoat(boat *b, char movHor, char movVert)
{
	//get pixmap size
	int w= getPixmap(1)->width;
	int h= getPixmap(1)->height;

	//find boat speed
	vector2 vSpeed = {movHor,movVert};
	if (normalizeRef(&vSpeed)!=0);//snap to length 1
	v2mulRef(&vSpeed,speed);
	//update boat position
	b->position.x+=vSpeed.x;
	b->position.y+=vSpeed.y;
	//check if position is inside map limits
	if (b->position.x<X_MIN) b->position.x=X_MIN;
	if (b->position.y<Y_MIN) b->position.y=Y_MIN;
	if (b->position.x>=X_MAX) b->position.x=X_MAX;
	if (b->position.y>=Y_MAX) b->position.y=Y_MAX;
	//update sprite
	b->sp->x = (int) b->position.x;
	b->sp->y = (int) b->position.y;
}

void testLimits(boat *b,int xDelta, int yDelta){
	int w= getPixmap(1)->width;
	int h= getPixmap(1)->height;

	b->sp->y +=yDelta;
	b->sp->x +=xDelta;
	if (b->sp->x < 1) b->sp->x=1;
	if (b->sp->y < 1) b->sp->y=1;
	if (b->sp->x >= 1024-w) b->sp->x=1024-w;
	if (b->sp->y >= 768-h) b->sp->y=768-h;
}
void drawBoat(boat *b)
{
draw_sprite_onBuffer(b->sp);
}

void freeBoat(boat *b)
{
	removeSpriteFromDrawList(b->sp);
	deleteSprite(b->sp);
	//free(b); not really needed
}

/** @} end of boat */
