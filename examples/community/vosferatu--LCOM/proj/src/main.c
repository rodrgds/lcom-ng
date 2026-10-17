#include "graphics.h"
#include "duelnix.h"
#include "vbe.h"
#include "mouse.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rtc.h"

int main(int argc, char **argv) {

	srand(time(NULL));

	sef_startup();

	vg_init(VBE_GAME_MODE);

	Duelnix* game = (Duelnix*) initDuelnix();

	while(!game->done){

		updateDuelnix(game);

		if(!game->done){
			if(game->draw)
				drawDuelnix(game);

			drawDate();

			if(getMouse()->draw){
				//flip_Mouse();
				drawMouse();

				flip_Display();
			}
		}
	}

	endDuelnix(game);

	vg_exit();

	return 0;
}
