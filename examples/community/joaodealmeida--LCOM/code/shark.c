#include "shark.h"
#include "stdlib.h"
#include "data_pixmap.h"
#include "pixmap.h"
#include "sprite.h"
#include <stdlib.h>

//#define FREEZE_SHARK

/** @defgroup shark shark
 * @{*/

//CONSTANTS
//area used by the sharks
//limits do not have to be equal to resolution
const int X_MAX = 1044;///<sharks area right side bound
const int X_MIN = -20;///<sharks area left side bound
const int Y_MAX = 788;///<sharks area lower bound
const int Y_MIN = -20;///<sharks area upper bound
const unsigned int MAX_NUMBER_OF_SHARKS = 20;///<sharks area upper bound
const unsigned short numberOfSharksInGameStart = 3.1f;///<number of sharks to appear in the beggining of the game
//const float defaultRad=3.0f;///<default Collision radius given to sharks
const float defaultSpeed=2.25f;///<default sharks speed without random factors
animationFrame_SV * sharkAnimation;///<shark animation

shark_SV *sharks;


void createSharks()
{
	Pixmap pm;
	pm.pixmap = read_xpm(tuba, &pm.width, &pm.height);
	addPixmap(&pm);
	pm.pixmap = read_xpm(tuba2, &pm.width, &pm.height);
		addPixmap(&pm);

	sharkAnimation = animationFrame_SV_init(2,0);
	SV_ADD(sharkAnimation,1,animationFrame);//add 1st frame
	SV_ADD(sharkAnimation,2,animationFrame);//add 2nd frame

	sharks = shark_SV_init(MAX_NUMBER_OF_SHARKS,5);

	unsigned short i = numberOfSharksInGameStart;
	for ( ;i>0;i--)
	{
		shark s = {};
		s.colRad=pm.width/2-10;//defaultRad;
		s.position= Vector2(rand()*100%X_MAX,1 );//s.position= Vector2(i*100%X_MAX,i*100%X_MAX*200%Y_MAX );
		s.vSpeed= v2_rotateACW(v2_One,i*35);
		v2mulRef(&s.vSpeed,defaultSpeed);
		s.sp = createAnimatedSprite(s.position.x,s.position.y
				,pm.width/2, pm.height/2,0,sharkAnimation);
		SV_ADD(sharks,s,shark);
		addSprite2DrawList(s.sp);
	}
}

void createAshark()
{
	Pixmap *pm = getPixmap(sharkAnimation->elems[0]);
	shark s = {};
	s.colRad=pm->width/2-4;//defaultRad;
	s.position= Vector2(rand()%X_MAX,0);
	s.vSpeed= v2_rotateACW(v2_One,(rand()%360)+1);
	v2mulRef(&s.vSpeed,defaultSpeed);
	s.sp = createAnimatedSprite(s.position.x,s.position.y
			,pm->width/2,pm->height/2,0,sharkAnimation);
	SV_ADD(sharks,s,shark);
	addSprite2DrawList(s.sp);
}

vector2 scopePosition={};
void updateShark(shark *s,char animate)
{
	//causes same prob as buffer aparently. freq. updates mess up sync
#ifndef FREEZE_SHARK
	//v2_rotateACWref(&(s->vSpeed),rand()%4>1?rand()%4:-rand()%4);
	
	//update shark position
	s->position.x+=s->vSpeed.x;
	s->position.y+=s->vSpeed.y;
#endif
	//check if position is inside map limits
	if (s->position.x<X_MIN)
	{
	s->position.x=X_MIN;
	s->vSpeed.x*=-1;//invert x speed
	}
	if (s->position.y<Y_MIN)
	{
	s->position.y=Y_MIN;
	s->vSpeed.y*=-1;//invert y speed
	}
	if (s->position.x>=X_MAX)
	{
	s->position.x=X_MAX;
	s->vSpeed.x*=-1;//invert x speed
	}
	if (s->position.y>=Y_MAX)
	{
	s->position.y=Y_MAX;
	s->vSpeed.y*=-1;//invert y speed
	}
	//update sprite
	s->sp->x = (int) s->position.x;
	s->sp->y = (int) s->position.y;
	//update animation
	//animated sprites missing
	if (animate) animatedSprite_playAnimation(s->sp);
}

void updateSharks(char animate,vector2 scopePos)
{
	if (sharks->size<MAX_NUMBER_OF_SHARKS && rand()%200==0)createAshark() ;
	scopePosition=scopePos;

	long shark2RemoveIndex;
	//remove only one, less information to send on multiplayer
	if /*while*/ ((shark2RemoveIndex = shark_SV_find(sharks,checkRemoveShark) )!=-1)
		removeShark(shark2RemoveIndex);
	//using find to map update by always returning false(0) on update Func
	int i = 0;
	for(; i < sharks->size ; i++)
		updateShark(&sharks->elems[i],animate);
}

char checkRemoveShark(shark* s)
{
	if(	pointInsideCircle(scopePosition,s->position,s->colRad)) return 1;//missing check here
	return 0;
}

shark_SV* getSharks() {return sharks;}

void deleteSharks()
{
	int i = 0;
	for(; i < sharks->size ; i++)
			removeShark(i);

	SV_FREE(sharks);
	SV_FREE(sharkAnimation);
}

void removeShark(unsigned long index)
{
	shark freeShark={};//it's going to be a copy but that's not a prob
	SV_REMOVE_TO(sharks,index,freeShark);
	removeSpriteFromDrawList(freeShark.sp);
	deleteSprite(freeShark.sp);
}

/** @} end of shark */
