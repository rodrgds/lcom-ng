#include <minix/drivers.h>
#include <minix/driver.h>
#include "obstacleCourse.h"
#include "graphics.h"
#include "video_gr.h"
#include "keyboard.h"
#include "timer.h"
#include "i8042.h"
#include "utilities.h"
#include "mouse.h"
#include "startMenuState.h"
#include "gameState.h"

void checkIfStateIsDone(ObstacleCourse* course);
void deleteCurrentState(ObstacleCourse* course);
void changeState(ObstacleCourse* course, State newState);

ObstacleCourse* startObstacleCourse() {
	ObstacleCourse* course = (ObstacleCourse*) malloc(sizeof(ObstacleCourse));


	course->IRQ_SET_MOUSE = mouse_subscribe_int();
	course->IRQ_SET_TIMER = timer_subscribe_int();
	mouse_write(ENABLE_DATA_REPORT);
	course->IRQ_SET_KBD = keyboard_subscribe_int();
	course->currentState = MAIN_MENU_STATE;
	course->state = newStartMenuState();

	course->done = 0;
	course->draw = 1;
	course->scancode = 0;
	course->notFirst = 0;

	course->cursor = loadBitmap(getImagePath("cursor"));
	initMouse();
	return course;
}

void updateObstacleCourse(ObstacleCourse* course) {
	int ipc_status;
	message msg;
	int r;
	unsigned long lixo;

	/* Get a request message. */
	if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
		printf("driver_receive failed with: %d", r);
	}
	if (is_ipc_notify(ipc_status)) { /* received notification */
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE: /* hardware interrupt notification */
			//TIMER INTERRUPT
			if (msg.NOTIFY_ARG & course->IRQ_SET_TIMER) {
				switch(course->currentState) {
				case MAIN_MENU_STATE:
					updateStartMenuState(course->state, course->scancode);
					break;
				case GAME_STATE:
					updateGameState(course->state, course->scancode);
					break;
				}
				course->scancode = 0;
				course->draw = 1;
				flipBuffer();
			}
			//KEYBOARD INTERRUPT
			if (msg.NOTIFY_ARG & course->IRQ_SET_KBD) {
				course->scancode = keyboard_read();
			}
			//MOUSE INTERRUPT
			if (msg.NOTIFY_ARG & course->IRQ_SET_MOUSE) {
				if(course->notFirst) {
					mouse_reader(course);
					if(mouse.packetReady) {
						mouse.packetReady = 0;
						updateMouse(course);
					}
				}
				else {
					mouse_read(&lixo);
					course->notFirst = 1;
				}
			}
			break;
		default:
			break; /* no other notifications expected: do nothing */
		}
	} else { /* received a standard message, not a notification */
		/* no standard messages expected: do nothing */
	}
	checkIfStateIsDone(course);

}

void drawObstacleCourse(ObstacleCourse* course) {
	switch (course->currentState) {
	case MAIN_MENU_STATE:
		drawStartMenuState(course->state);
		drawBitmap(course->cursor, mouse.x, mouse.y, ALIGN_LEFT);
		break;
	case GAME_STATE:
		drawGameState(course->state);
		break;
	default:
		break;
	}
	course->draw = 0;
}

void stopObstacleCourse(ObstacleCourse* course) {
	keyboard_unsubscribe_int();
	mouse_unsubscribe_int();
	timer_unsubscribe_int();
	deleteCurrentState(course);
	free(course);
}


void changeState(ObstacleCourse* course, State newState) {
	// deleting current state
	deleteCurrentState(course);

	// changing current state
	course->currentState = newState;

	// creating new state
	switch (course->currentState) {
	case MAIN_MENU_STATE:
		course->state = newStartMenuState();
		break;
	case GAME_STATE:
		course->state = newGameState();
		break;
	}
	course->draw = 1;
}

void checkIfStateIsDone(ObstacleCourse* course) {
	switch (course->currentState) {
	case MAIN_MENU_STATE:
		if (((StartMenuState*) (course->state))->done) {
			int action = ((StartMenuState*) (course->state))->action;
			switch (action) {
			case PLAY_CHOSEN:
				changeState(course, GAME_STATE);
				break;
			case EXIT_CHOSEN:
				course->done = 1;
				break;
			}
		}
		break;
	case GAME_STATE:
		if(((GameState*) (course->state))->done)
			changeState(course, MAIN_MENU_STATE);
	default:
		break;
	}
}

void deleteCurrentState(ObstacleCourse* course) {
	switch (course->currentState) {
	case MAIN_MENU_STATE:
		deleteStartMenuState(course->state);
		break;
	case GAME_STATE:
		deleteGameState(course->state);
	}
}
