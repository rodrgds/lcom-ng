#include "video_gr.h"
#include "mouse.h"
#include "keyboard.h"
#include "i8042.h"
#include "bitmap.h"
#include "mouse-maze.h"
#include <stdint.h>
#include <machine/int86.h>

#include "MainMenuState.h"
#include "GameState.h"
#include "WinState.h"
#include "LoseState.h"
#include "ModeState.h"

const int FPS = 60;
int m = 0;
int n = 0;

void handler_rtc(MouseMaze* maze) {
	unsigned long ev = rtc_check();
	if (maze->currentState == GAME_STATE) {
		if (ev & RTC_AF) {
			((GameState*) (maze->state))->lose = 1;
			((GameState*) (maze->state))->done = 1;
		}
		if (ev & RTC_UF) {
			((GameState*) (maze->state))->timer--;
		}
	}
}

void checkIfStateIsDone(MouseMaze* game);
void deleteCurrentState(MouseMaze* game);

MouseMaze* startMouseMaze() {
	MouseMaze* maze = (MouseMaze*) malloc(sizeof(MouseMaze));
	maze->irq_set_kb = kbd_subscribe();
	maze->irq_set_mouse = mouse_subscribe();
	maze->irq_set_timer = timer_subscribe_int();
	maze->irq_set_rtc = rtc_subscribe();
	timer_set_square(0, FPS);
	maze->validation = 0;
	maze->scan_code = 0;
	maze->timer = newTimer();
	newMouse();
	maze->currentState = MAIN_MENU_STATE;
	maze->state = newMainMenuState();
	return maze;
}

void stopMouseMaze(MouseMaze* maze) {
	deleteCurrentState(maze);
	mouse_unsubscribe();
	kbd_unsubscribe();
	timer_unsubscribe_int();
	rtc_unsubscribe();
	deleteMouse();
	mouse_read(&maze->clean);
	deleteTimer(maze->timer);
	free(maze);
}

void drawMouseMaze(MouseMaze* maze) {
	cleanScreen();
	switch (maze->currentState) {
	case MAIN_MENU_STATE:
		drawMainMenuState(maze->state);
		drawMouse();
		break;
	case MODE_STATE:
		drawModeState(maze->state);
		drawMouse();
		break;
	case GAME_STATE:
		drawGameState(maze->state);
		if (!m)
			drawMouse();
		break;
	case WIN_STATE:
		drawWinState(maze->state);
		break;
	case LOSE_STATE:
		drawLoseState(maze->state);
		break;
	default:
		break;
	}
}

void updateMouseMaze(MouseMaze* maze) {
	int ipc_status;
	message msg;

	resetTimer(maze->timer);

	int receive;
	receive = driver_receive(ANY, &msg, &ipc_status);
	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:
			if (msg.NOTIFY_ARG & maze->irq_set_timer) {
				timer_handler(maze->timer);
			}
			if (msg.NOTIFY_ARG & maze->irq_set_mouse) {
				handlerMouse();
			}
			if (msg.NOTIFY_ARG & maze->irq_set_kb) {
				maze->scan_code = kbc_read();
			}
			if (msg.NOTIFY_ARG & maze->irq_set_rtc) {
				handler_rtc(maze);
			}
			break;
		default:
			break;
		}
	}
	if (maze->timer->ticked) {
		switch (maze->currentState) {
		case MAIN_MENU_STATE:
			updateMainMenuState(maze->state, maze->scan_code);
			break;
		case MODE_STATE:
			updateModeState(maze->state, maze->scan_code);
			break;
		case GAME_STATE:
			updateGameState(maze->state, maze->scan_code);
			break;
		case WIN_STATE:
			updateWinState(maze->state, maze->scan_code);
			break;
		case LOSE_STATE:
			updateLoseState(maze->state, maze->scan_code);
			break;
		default:
			break;
		}
		maze->scan_code = 0;
		drawMouseMaze(maze);
		flipScreen();
	}
	checkIfStateIsDone(maze);
}

void changeState(MouseMaze* maze, State newState) {
// deleting current state
	deleteCurrentState(maze);

// changing current state
	maze->currentState = newState;

// creating new state
	switch (maze->currentState) {
	case MAIN_MENU_STATE:
		maze->state = newMainMenuState();
		break;
	case MODE_STATE:
		maze->state = newModeState();
		break;
	case GAME_STATE:
		maze->state = newGameState(m, n);
		break;
	case WIN_STATE:
		maze->state = newWinState();
		break;
	case LOSE_STATE:
		maze->state = newLoseState();
	default:
		break;
	}
}

void checkIfStateIsDone(MouseMaze* maze) {
	switch (maze->currentState) {
	case MAIN_MENU_STATE:
		if (((MainMenuState*) (maze->state))->done) {
			int action = ((MainMenuState*) (maze->state))->action;

			switch (action) {
			case PLAY_CHOSEN:
				changeState(maze, MODE_STATE);
				break;
			case EXIT_CHOSEN:
				maze->validation = 1;
				break;
			}
		}
		break;
	case MODE_STATE:
		if (((ModeState*) (maze->state))->done) {
			int action = ((ModeState*) (maze->state))->action;

			switch (action) {
			case NORMAL_MOUSE:
				m = 0;
				n = 0;
				changeState(maze, GAME_STATE);
				break;
			case NORMAL_KEYBOARD:
				m = 1;
				n = 0;
				changeState(maze, GAME_STATE);
				break;
			case NIGHT_MOUSE:
				m = 0;
				n = 1;
				changeState(maze, GAME_STATE);
				break;
			case NIGHT_KEYBOARD:
				m = 1;
				n = 1;
				changeState(maze, GAME_STATE);
				break;
			case EXIT:
				changeState(maze, MAIN_MENU_STATE);
				break;
			}
		}
		break;
	case GAME_STATE:
		if (((GameState*) (maze->state))->done) {
			if (((GameState*) (maze->state))->win)
				changeState(maze, WIN_STATE);
			else if (((GameState*) (maze->state))->lose)
				changeState(maze, LOSE_STATE);
			else
				changeState(maze, MODE_STATE);
		}
		break;
	case WIN_STATE:
		if (((WinState*) (maze->state))->done)
			changeState(maze, MAIN_MENU_STATE);
		break;
	case LOSE_STATE:
		if (((LoseState*) (maze->state))->done)
			changeState(maze, MAIN_MENU_STATE);
	default:
		break;
	}
}

void deleteCurrentState(MouseMaze* maze) {
	switch (maze->currentState) {
	case MAIN_MENU_STATE:
		deleteMainMenuState(maze->state);
		break;
	case MODE_STATE:
		deleteModeState(maze->state);
		break;
	case GAME_STATE:
		deleteGameState(maze->state);
		break;
	case WIN_STATE:
		deleteWinState(maze->state);
		break;
	case LOSE_STATE:
		deleteLoseState(maze->state);
		break;
	default:
		break;
	}
}
