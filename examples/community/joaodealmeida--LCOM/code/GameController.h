#ifndef __GameController_H
#define __GameController_H

#include "boat.h"

//GAME "STATES"
#define GAME_QUIT 0
#define GAME_RUNNING 1 //indicates that the game is still running
//STATES IN WICH THE GAME IS RUNNING SHOULD BE ODD AND THE OTHERS EVEN
#define GAME_INGAME 3
#define GAME_GAMEOVER 5
#define GAME_SYNC 7
#define GAME_RESTART 10

typedef struct {
//	int KeyboardIRQ;
//	int TimerIRQ;
//	char restart;
	unsigned int state;
	//char gameover;
	char draw;
	//int scancode;
	char shootButtonState;
	boat playerBoat;
	boat multiplayerBoat;
} GameController;

GameController* initGameController();
void updateGame();
void drawGame();
void stopGame(GameController *game);
void initDevices();///<subscribe stuff
void stopDevices();///<unsubscribe stuff
/**
 * return zero if succeeds, -1 otherwise
 */
char gameSeedInit();///<initializae random seed before game.

#endif


