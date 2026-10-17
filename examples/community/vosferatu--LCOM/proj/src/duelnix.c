#include "duelnix.h"
#include <minix/drivers.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "bitmap.h"
#include "vbe.h"
#include "keyboard.h"
#include "i8042.h"
#include "kbdlab3.h"
#include "cowboy.h"
#include "i8042.h"
#include "utils.h"
#include "mouse.h"
#include "rtc.h"
#include "MainMenuState.h"
#include "PracticeState.h"
#include "GameState.h"
#include "Multiplayer.h"


void checkIfStateIsDone(Duelnix* game);
void deleteCurrentState(Duelnix* game);

Duelnix* initDuelnix(){
	Duelnix* duelnix = (Duelnix*) malloc(sizeof(Duelnix));

	duelnix->IRQ_SET_KBD = keyboard_subscribe_int();
	duelnix->IRQ_SET_TIMER = timer_subscribe_int();
	duelnix->IRQ_SET_MOUSE = mouse_subscribe_int();
	duelnix->IRQ_SET_RTC = rtc_subscribe_int();

	//timer_set_square(0,60);
	duelnix->done = 0;
	duelnix->draw = 1;
	duelnix->scancode = 0;
	duelnix->timer = newTimer();
	newMouse();
	newDate();

	duelnix->currentState = MAIN_MENU_STATE;
	duelnix->state = newMainMenuState();

	return duelnix;
}

void updateDuelnix(Duelnix* duelnix){
	int ipc_status, r;
	message msg;

	resetTimerTickedFlag(duelnix->timer);

	if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
		return;
	}
	if (is_ipc_notify(ipc_status)) { // received notification
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE: // hardware interrupt notification
			if (msg.NOTIFY_ARG & duelnix->IRQ_SET_KBD) { // keyboard interrupt
				unsigned long scan = keyboard_c_handler();
				//unsigned long scan = kbd_handler();
				if (scan != TWO_BYTE_SCAN)
					duelnix->scancode = scan;
			}
			if(msg.NOTIFY_ARG & duelnix->IRQ_SET_TIMER){// timer interrupt
				timerHandler(duelnix->timer);
			}
			if(msg.NOTIFY_ARG & duelnix->IRQ_SET_MOUSE){// mouse interrupt
				updateMouse();
			}
			if(msg.NOTIFY_ARG & duelnix->IRQ_SET_RTC){//rtc interrupt
				rtc_int_handler();
			}
			break;
		default:
			break; // no other notifications expected: do nothing
		}
	}

	if(duelnix->timer->ticked){
		getMouse()->draw = 1;

		switch(duelnix->currentState){
			case MAIN_MENU_STATE:
				updateMainMenuState(duelnix->state, duelnix->scancode);
				break;
			case PRACTICE_STATE:
				updatePracticeState(duelnix->state, duelnix->scancode);
				break;
			case GAME_STATE:
				updateGameState(duelnix->state, duelnix->scancode);
				break;
			case MULTIPLAYER:
				updateMultiplayer(duelnix->state, duelnix->scancode);
				break;
			default:
				break;
		}

		checkIfStateIsDone(duelnix);

		duelnix->draw = 1;
		duelnix->scancode = 0;
	}

	checkIfStateIsDone(duelnix);
}

void drawDuelnix(Duelnix* duelnix){
	clear_screen();
	switch(duelnix->currentState){
		case MAIN_MENU_STATE:
			drawMainMenuState(duelnix->state);
			break;
		case PRACTICE_STATE:
			if(((PracticeState*) (duelnix->state))->done == 0){
				drawPracticeState(duelnix->state);
			}
			break;
		case GAME_STATE:
			if(((GameState*) (duelnix->state))->done == 0){
				drawGameState(duelnix->state);
			}
			break;
		case MULTIPLAYER:
			if(((Multiplayer*) (duelnix->state))->done == 0){
				drawMultiplayer(duelnix->state);
			}
			break;
		default:
			break;
	}
	duelnix->draw = 0;
}

void endDuelnix(Duelnix* duelnix){
	deleteCurrentState(duelnix);

	clear_mouse_buffer();

	keyboard_unsubscribe_int();
	timer_unsubscribe_int();
	mouse_subscribe_int();
	rtc_unsubscribe_int();

	deleteTimer(duelnix->timer);
	disableMouse();
	deleteMouse();
	deleteDate();

	free(duelnix);
}

void changeState(Duelnix* game, State newState){
	deleteCurrentState(game);
	game->currentState = newState;

	switch (game->currentState){
		case MAIN_MENU_STATE:
			game->state = newMainMenuState();
			break;
		case PRACTICE_STATE:
			game->state = newPracticeState();
			break;
		case GAME_STATE:
			game->state = newGameState();
			break;
		case MULTIPLAYER:
			game->state = newMultiplayer();
			break;
		default:
			break;
	}

	game->draw = 1;
}

void checkIfStateIsDone(Duelnix* game){
	switch (game->currentState){
		case MAIN_MENU_STATE:
			if(((MainMenuState*) (game->state))->done){
				MenuAction action = ((MainMenuState*) (game->state))->action;

				switch(action){
					case PLAY_GAME:
						changeState(game, GAME_STATE);
						break;
					case EXIT_GAME:
						game->done = 1;
						break;
					case PRACTICE:
						changeState(game, PRACTICE_STATE);
						break;
					case MPLAYER:
						changeState(game, MULTIPLAYER);
					default:
						break;
				}
			}

			break;
		case PRACTICE_STATE:
			if(((PracticeState*) (game->state))->done){
					changeState(game, MAIN_MENU_STATE);
			}
			break;
		case GAME_STATE:
			if(((GameState*) (game->state))->done){
				changeState(game, MAIN_MENU_STATE);
			}
			break;
		case MULTIPLAYER:
			if(((Multiplayer*) (game->state))->done){
				changeState(game, MAIN_MENU_STATE);
			}
			break;
		default:
			break;
	}
}

void deleteCurrentState(Duelnix* game){
	switch (game->currentState){
		case MAIN_MENU_STATE:
			deleteMainMenuState(game->state);
			break;
		case PRACTICE_STATE:
			deletePracticeState(game->state);
			break;
		case GAME_STATE:
			deleteGameState(game->state);
			break;
		case MULTIPLAYER:
			deleteMultiplayer(game->state);
			break;
		default:
			break;
	}
}
