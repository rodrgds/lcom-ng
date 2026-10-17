#include "sprite.h"
#include "pixmap.h"
#include "video_gr.h"
#include "buffer.h"
#include "graphics.h" //rename later
//#include "stdlib.h"
//#include "smart_vector.h"

static unsigned long transparentColor = 0;///<transparent color

defineSmartVector(spritePtr);
spritePtr_SV *drawList;

spritePtr createStaticSprite(int x, int y,int xC,int yC,int pixmapIndex)
{
	spritePtr sp; ALLOC(Sprite,1,sp);
	StaticSprite* ss; ALLOC(StaticSprite,1,ss);
	sp->xCenter=xC;
	sp->yCenter=yC;
	ss->index=pixmapIndex;
	sp->isStatic=1;
	sp->ptr = ss;
	sp->x=x;
	sp->y=y;
	return sp;
}
spritePtr createAnimatedSprite(int x, int y,int xC,int yC,int beginIndex,animationFrame_SV* animation )
{
	spritePtr sp; ALLOC(Sprite,1,sp);
	AnimatedSprite* as; ALLOC(AnimatedSprite,1,as);
	as->currentIndex=beginIndex;
	as->animation=animation;
	sp->xCenter=xC;
	sp->yCenter=yC;
	sp->isStatic=0;
	sp->ptr = as;
	sp->x=x;
	sp->y=y;
	return sp;
}

void animatedSprite_playAnimation(Sprite *sp)
{
	if (sp->isStatic) ; //can't animate static sprites
	else{
		AnimatedSprite* as = (AnimatedSprite *) sp->ptr;
		as->currentIndex= (as->currentIndex+1)%as->animation->size;
	}
}

void initSpritesDrawList()
{
	drawList=spritePtr_SV_init(20,5);
}
void deleteSprite(Sprite *spPtr)
{
	if (spPtr->isStatic==0) //if it is an animated sprite
	{
		AnimatedSprite* as = (AnimatedSprite *) spPtr->ptr;
		//free(as->animationIndexes);//should use an array defined somewhere!
		free(as);
	}
	else {StaticSprite * ss =  (StaticSprite *) spPtr->ptr; free(ss); }
	free(spPtr);
}

Sprite* findSpriteAux;
char findSprite(Sprite **spPtr)
{
	if (*spPtr==findSpriteAux) return 1;
	return 0;
}
void removeSpriteFromDrawList(Sprite *spPtr)
{
	findSpriteAux=spPtr;
	int index = spritePtr_SV_find(drawList,findSprite);
	if(index!=-1) SV_REMOVE(drawList,index);
}

void draw_sprite_onScreen(Sprite *sp)//256 colors version
{
	unsigned int pixmapIndex=0;
	if (sp->isStatic) pixmapIndex= ((StaticSprite *) sp->ptr)->index;
	else{
	AnimatedSprite* as = (AnimatedSprite *) sp->ptr;
	if (as->animation->size<=as->currentIndex) { return;} //big error
	pixmapIndex = as->animation->elems[as->currentIndex];
	}
	Pixmap* pm = getPixmap(pixmapIndex);
	//for now consider sprite position = screen position
	int xi = sp->x;
	int yi =sp->y;
	int width=pm->width;

	unsigned char* pmData = &(pm->pixmap[width*pm->height-1]);//iterate in reverse
	int x = xi+width-1;
	int y = yi+pm->height-1;
	for(;y>=yi;y--)
		for(x = xi+width-1;x>=xi;x--)
		{
			if (*pmData!=transparentColor) setPixel((y*H_RES)+x,*pmData);
			pmData--;
		}
}

void draw_sprite_onBuffer(Sprite *sp)//256 colors version
{
	unsigned int pixmapIndex=0;
	if (sp->isStatic) pixmapIndex= ((StaticSprite *) sp->ptr)->index;
	else{
		AnimatedSprite* as = (AnimatedSprite *) sp->ptr;
		if (as->animation->size<=as->currentIndex) { return;} //big error
			pixmapIndex = as->animation->elems[as->currentIndex];
	}
	Pixmap* pm = getPixmap(pixmapIndex);
	//for now consider sprite position = screen position
	int xi =sp->x  -/*offset*/ sp->xCenter;
	//int yi;
	int xmin = xi;
	int yi =sp->y-/*offset*/ sp->yCenter;
	int width=pm->width;


	int xLextra=0;
	if (xmin<0) {xLextra=xmin; xmin=0;}

	int y = yi+pm->height-1;
	if (yi<0)yi=0;
	//if (xi<0) xi=0;
	//if (xi<0) xi=0;

	unsigned char* pmData = &(pm->pixmap[width*pm->height-1]);//iterate in reverse

	//posicao canto inferior direito
	int xRextra=0;
	 xi=xi+width-1; if (xi>=H_RES){ xRextra=xi-H_RES+1; xi=H_RES-1;}
	int x = xi;

	if (y>=V_RES) {
	pmData = (pmData - (width*(y-V_RES+1)) );
		y=V_RES-1;
			;}


	for(;y>=yi;y--){
		pmData = (pmData-xRextra);
		for(x = xi;x>=xmin;x--)
		{
			if (*pmData!=transparentColor) buffer_drawPixel((y*H_RES)+x,*pmData);
			pmData--;
		}
		pmData = (pmData+xLextra);
	}
}

void addSprite2DrawList(Sprite *spPtr)
{
	SV_ADD(drawList,spPtr,spritePtr);
}

/** @brief order sprites*/
char sortSprites(spritePtr s1,spritePtr s2)
{ return s1->y>s2->y;}
void drawSprites()
{
	SV_SORT(drawList,sortSprites,spritePtr);
	long i=drawList->size-1;
	for (;i>=0;i--) draw_sprite_onBuffer( drawList->elems[i]);
}

void freeDrawList()
{
	long i=drawList->size-1;
	for (;i>=0;i--) deleteSprite(drawList->elems[i]);
	free(drawList);
}
