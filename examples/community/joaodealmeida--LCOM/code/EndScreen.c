#include "EndScreen.h"
#include "pixmap.h"
#include "sprite.h"
#include "buffer.h"
//#include "test5.h"
#include "data_pixmap.h"
#include "collision.h"
#include "video_gr.h"
#include "video_gr.h"
#include "multiplayerSync.h"

static spritePtr sp,sp2,sp3;
ColisionRadius buttonsRadius;
void initEndScreen()
{
	Pixmap pm,pm1,pm2;
	pm.pixmap = read_xpm(gameover, &pm.width, &pm.height);
	sp = createStaticSprite(H_RES/2, 100,  pm.width/2, pm.height/2,addPixmap(&pm));

	if (multiplayer_getUserType()==USER_TYPE_NONE) {
	pm1.pixmap = read_xpm(repeat_button, &pm1.width, &pm1.height);
	sp2 = createStaticSprite(H_RES/2, 278, pm1.width/2, pm1.height/2,addPixmap(&pm1));
	}

	pm2.pixmap = read_xpm(shutdown_button, &pm2.width, &pm2.height);
	sp3 = createStaticSprite(H_RES/2, 500,pm2.width/2, pm2.height/2,addPixmap(&pm2));

	buttonsRadius = pm2.width/2;
}

void drawEndScreen()
{
	draw_sprite_onBuffer(sp);
	if (multiplayer_getUserType()==USER_TYPE_NONE)
		draw_sprite_onBuffer(sp2);
	draw_sprite_onBuffer(sp3);
}

char endGameChoice(vector2 mousePos)
{
	if (multiplayer_getUserType()==USER_TYPE_NONE)
		if (pointInsideCircle(mousePos,Vector2(sp2->x,sp2->y),buttonsRadius))
			return RESTART_GAME;
	if (pointInsideCircle(mousePos,Vector2(sp3->x,sp3->y),buttonsRadius)) return QUIT_GAME;
	return 0;
}

void freeEndScreen(){
	deleteSprite(sp);
	if (multiplayer_getUserType()==USER_TYPE_NONE)
		deleteSprite(sp2);
	deleteSprite(sp3);
}
