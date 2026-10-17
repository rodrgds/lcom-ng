#include <minix/drivers.h>
#include "Game.h"
#include "keyboard.h"
#include "timer.h"
#include "Bitmap.h"
#include "interface.h"
#include "MainMenuState.h"
#include "mouse.h"
#include "keyboard.h"
#include "i8254.h"
#include "GameState.h"
#include "FinalState.h"

unsigned int scoreAux=0;

Game* startGame(){

	Game* game =(Game*)malloc(sizeof(Game));

	//subscribing
	timer_set_square(0,60);
	game->set_kb=kbd_subscribe();
	game->set_mouse=mouse_subscribe();

	game->currentState = MAIN_MENU_STATE;
	game->state = newMainMenuState();
	game->counter = 0;
	game->mainScore = 0;
	return game;
}

int updateGame(Game* g) {
	int ipc_status;
	message msg;
	int exit = 0;
	int play = 0;
	int exitGameState = 0;



	if (driver_receive(ANY, &msg, &ipc_status) == 0) {
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				// TIMER interruption
				if (msg.NOTIFY_ARG == g->set_timer) {
					g->counter++;
				}

				// KEYBOARD interruption
				if (msg.NOTIFY_ARG & g->set_kb) {
					g->scancode = kbd_code_scan();
				}

				// MOUSE interruption
				if (msg.NOTIFY_ARG == g->set_mouse) {
					updateMouse();
				}

				break;
			default:
				break;
			}
		}
	}

	switch (g->currentState) {
	case MAIN_MENU_STATE:
		exit = updateMainMenuState(g->state, g->scancode);
		play = verifyStateChange(g->state);
		drawGame(g);
		refresh();
		break;
	case GAME_STATE:
		exitGameState = updateGameState(g->state, g->scancode, g->counter);
		drawGame(g);
		refresh();
		break;
	case FINAL_STATE:
		exit = updateFinalState(g->state,g->scancode,g->mainScore, scoreAux);
		play = verifyFinalStateChange(g->state);
		drawGame(g);
		refresh();
		break;
	default:
		break;
	}

	if(exitGameState!=0){
		scoreAux=exitGameState;
		g->state = newFinalState();
		g->currentState = FINAL_STATE;
		exitGameState=0;
	}
	if (play) {
		scoreAux=0;
		g->mainScore=0;
		g->counter=0;
		g->state = newGameState();
		g->currentState = GAME_STATE;
		g->set_timer = timer_subscribe_int();
		mouse_unsubscribe();
		play = 0;
	}
	return exit;

}

void drawGame(Game* g) {
	switch (g->currentState) {
	case MAIN_MENU_STATE:
		drawMenu(g->state);
		drawMouse();
		drawPokeball(g->state);
		break;
	case GAME_STATE:
		drawGameState(g->state);
		break;
	case FINAL_STATE:
		drawFinalMenu(g->state);
		break;
	default:
		break;
	}

}

void deleteState(Game* g){
	switch(g->currentState){
	case MAIN_MENU_STATE:
			deleteMainMenuState(g->state);
			break;
	case GAME_STATE:
			deleteGameState(g->state);
			break;
	case FINAL_STATE:
			deleteFinalState(g->state);
			break;
	default:
			break;
	}
}

void deleteGame(Game* g){
	deleteState(g);

	free(g);
}
