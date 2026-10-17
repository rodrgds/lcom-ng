#include "scope.h"
#include "pixmap.h"
#include "sprite.h"
#include "buffer.h"
//#include "test5.h"
#include "data_pixmap.h"
#include "video_gr.h"


static spritePtr sp;

vector2 getScopePosition()
{
	return Vector2(sp->x,sp->y);
}

void initScope(){
	Pixmap pm;
	pm.pixmap = read_xpm(scope, &pm.width, &pm.height);
	sp = createStaticSprite(50, 50,
			pm.width/2, pm.height/2,addPixmap(&pm));
}

void drawScope(){
	draw_sprite_onBuffer(sp);
}

void updateScope(float xDelta, float yDelta){
	int w= getPixmap(0)->width;
	int h= getPixmap(0)->height;

	sp->y+=(int)(yDelta);
	sp->x+=(int)(xDelta);
	if (sp->x<1) sp->x=1;
	if (sp->y<1) sp->y=1;
	if (sp->x >= H_RES) sp->x=H_RES;
	if (sp->y >= V_RES) sp->y=V_RES;
}

void freeScope(){
	deleteSprite(sp);
}

