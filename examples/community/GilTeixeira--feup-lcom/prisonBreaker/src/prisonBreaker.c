#include <minix/drivers.h>
#include "video_gr.h"
#include "kbd.h"
#include "game.h"
#include "mouse.h"
#include "prisonBreaker.h"
#include "bitmap.h"

PrisonBreaker* initPrisonBreaker() {
	PrisonBreaker* prisonBreaker = (PrisonBreaker*) malloc(
			sizeof(PrisonBreaker));

	prisonBreaker->IRQ_SET_KBD = kbd_subscribe_int();
	prisonBreaker->IRQ_SET_TIMER = timer_subscribe_int();
	prisonBreaker->IRQ_SET_MOUSE = mouse_subscribe_int();

	enable_stream_mode();

	prisonBreaker->draw = 1;
	prisonBreaker->scancode = 0;

	prisonBreaker->timer = initTimer();
	prisonBreaker->mouse = initMouse();

	prisonBreaker->gameState = initGameState();



	return prisonBreaker;
}

void updatePrisonBreaker(PrisonBreaker* prisonBreaker) {

	int ipc_status, r;

	message msg;

	if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
		printf("driver_receive failed with: %d", r);
		return;
	}

	if (is_ipc_notify(ipc_status)) { /* received notification */
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE: /* hardware interrupt notification */
			if (msg.NOTIFY_ARG & prisonBreaker->IRQ_SET_KBD) { /* keyboard interrupt */
				kbd_handler();
				prisonBreaker->scancode = globalCode;
				gameStateUpdateKeyboard(prisonBreaker->gameState,
						prisonBreaker->scancode, prisonBreaker->timer,
						prisonBreaker->mouse);

			}
			if (msg.NOTIFY_ARG & prisonBreaker->IRQ_SET_TIMER) { /* timer interrupt */
				timerHandler(prisonBreaker->timer);

				gameStateUpdate(prisonBreaker->gameState, prisonBreaker->timer,
						prisonBreaker->mouse);
				prisonBreaker->draw = 1;
			}
			if (msg.NOTIFY_ARG & prisonBreaker->IRQ_SET_MOUSE) { /* mouse interrupt */
				mouseHandler(prisonBreaker->mouse);
				gameStateUpdateMouse(prisonBreaker->gameState,
						prisonBreaker->mouse);


			}

			break;
		default:
			break;
		}
	}


}

void drawPrisonBreaker(PrisonBreaker* prisonBreaker) {
	displayGameState(prisonBreaker->gameState, prisonBreaker->timer);

}

void stopPrisonBreaker(PrisonBreaker* prisonBreaker) {
	kbd_unsubscribe_int();
	timer_unsubscribe_int();
	mouse_unsubscribe_int();

	stopTimer(prisonBreaker->timer);
	stopMouse(prisonBreaker->mouse);
	stopGameState(prisonBreaker->gameState);

	free(prisonBreaker);

}

