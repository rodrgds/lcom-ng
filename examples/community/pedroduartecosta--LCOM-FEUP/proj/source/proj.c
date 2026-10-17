#include <minix/drivers.h>
#include "timer.h"
#include "pixmap.h"
#include "interface.h"
#include "Bitmap.h"
#include "Player.h"
#include "Game.h"
#include "GameState.h"
#include "Collisions.h"
#include "score.h"

int main(int argc, char **argv) {

	int exit=0;
	sef_startup();
	initGame(0x114);
	int score;
	Game* g = (Game*) startGame();
	drawGame(g);

	while(!exit){

		exit=updateGame(g);
	}
	exitGame();
	deleteGame(g);

	return 0;
}
