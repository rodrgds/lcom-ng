#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <time.h>
#include <stdlib.h>

#include "video_gr.h"
#include "i8042.h"
#include "i8254.h"

#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "game.h"
#include "circle.h"
#include "cursor.h"

#include "queue.h"

#include "game_core_st.h"

static int timer_id;
static int kbd_id;
static int mouse_id;
static int rtc_id;

int init_game(char *file) {

	linkStart(file);

	game_menu();

	return 0;
}

int game_menu() {

	srand(time(NULL));

	//Queue where we will save the circles
	queue circles_menu = q_new();

	//Cursor
	Cursor * cursor = NULL;

	//Subscribe timer0
	timer_id = timer_subscribe_int();
	if (timer_id == -1)
		return 1;

	//Subscribe keyboard
	kbd_id = keyboard_subscribe_int();
	if (kbd_id == -1)
		return 1;

	//Subscribe mouse
	mouse_id = mouse_subscribe_int();
	if (mouse_id == -1)
		return 1;

	//Subscribe rtc
	rtc_id = rtc_subscribe_int();
	if (rtc_id == -1)
		return 1;

	//Stream mode
	if (M_writeCommand(STREAM_MODE))
		return 1;

	//Enable data reporting
	if (M_writeCommand(ENABLE_DATA))
		return 1;

	enable_update_int();

	if (vg_init(0x105) == NULL)
		printf("Error entering graphics mode\n");

	int ipc_status, r;
	message msg;
	int irq_set_kbd = BIT(kbd_id);
	int irq_set_mouse = BIT(mouse_id);
	int irq_set_timer = BIT(timer_id);
	int irq_set_rtc = BIT(rtc_id);

	int nrIntTimer;
	int stop;

	restart:
	cursor = create_cursor();
	nrIntTimer = 0;
	stop = 0;

	//Start clock with 60Hz
	timer_set_frequency(0, 60);

	while (!stop) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			return 1;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_rtc) {

					rtc_int_handler();

					//do nothing
				}
				if (msg.NOTIFY_ARG & irq_set_kbd) {

					keyboard_int_handler();

					//Get last code
					unsigned long currentCode = getLastCode();

					//Check if it needs to print the code
					if (currentCode == TWOBYTE)
						continue;

					//ESC key was pressed, stop the loop
					if (currentCode == ESC) {
						if(getCurrentState() == MENU2)
							setCurrentState(MENU1);
					}

				}
				if (msg.NOTIFY_ARG & irq_set_timer) {

					//Create new circle every next_circle frames
					if (nrIntTimer % 40 == 0) {

						//Create circle and add it to array
						Circle * new_circle = menu_create_circle();

						//Push circle to queue
						enqueue(circles_menu, new_circle);
					}

					//Temporary queues
					queue tmp_q1 = q_new();

					while (!empty(circles_menu)) {

						Circle* ticked = NULL;

						dequeue(circles_menu, &ticked);

						if (!tick_menu_circle(ticked))
							enqueue(tmp_q1, ticked);
					}

					//Recreate "circles_menu" queue
					circles_menu = tmp_q1;

					event_t mouse_over = (event_t) { ACTIVATE, 0x00, NULL , cursor };
					mosu_handler(&mouse_over);

					//Re-apply the cursor again to make sure it isn't erased by other circles that may appear
					draw_cursor(cursor);

					vg_nextFrame();

					nrIntTimer++;

				}
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					mouse_int_handler();

					if (getCounter() == 0) {

						updateCoord();

						int dx = getX(), dy = getY();

						update_cursor(cursor, dx, dy);

						//Calls Game_Core state machine
						if(getLB())	{
							printf("LB DOWN");
							event_t key_p = (event_t) { LEFT_B, 0x00, NULL , cursor };
							mosu_handler(&key_p);
						}

						if(getCurrentState() == END_GAME)
							stop = 1;
						else if (getCurrentState() == IN_GAME_EASY)	{

							while (!empty(circles_menu)) {
								Circle* ticked = NULL;
								dequeue(circles_menu, &ticked);
								destroy_circle(ticked);
							}

							destroy_cursor(cursor);

							game_mode_easy();

							setCurrentState(MENU1);

							goto restart;
						}
						else if (getCurrentState() == IN_GAME_HARD)
						{
							while (!empty(circles_menu)) {
								Circle* ticked = NULL;
								dequeue(circles_menu, &ticked);
								destroy_circle(ticked);
							}

							destroy_cursor(cursor);

							game_mode_hard();

							event_t save_score = (event_t) { SAVE, 0x00, NULL, NULL};
							mosu_handler(&save_score);

							goto restart;
						}
					}
				}
				break;
			default:
				break;
			}
		} else {
		}
	}

	vg_exit();

	//Free allocated space for cursor
	destroy_cursor(cursor);

	//Disable data reporting
	if (M_writeCommand(DISABLE_DATA))
		return 1;

	//Unsubscribe keyboard
	if (keyboard_unsubscribe_int() != 0)
		return 1;

	//Unsubscribe mouse
	if (mouse_unsubscribe_int() != 0)
		return 1;

	//Unsubscribe timer0
	if (timer_unsubscribe_int() != 0)
		return 1;

	//Unsubscribe rtc
	if (rtc_unsubscribe_int() != 0)
		return 1;

	return 0;
}

int game_mode_easy() {

	//Queue where we will save the circles
	queue circles = q_new();
	queue past_circles = q_new();

	//Cursor
	Cursor * cursor_easy = create_cursor();

	int ipc_status, r;
	message msg;
	int irq_set_kbd = BIT(kbd_id);
	int irq_set_mouse = BIT(mouse_id);
	int irq_set_timer = BIT(timer_id);
	int irq_set_rtc = BIT(rtc_id);

	int nrIntTimer = 0;
	int stop = 0;

	//Start clock with 60Hz
	timer_set_frequency(0, 60);

	int next_circle = 60;

	Circle * circle1 = menu_create_circle();
	Circle * circle2 = menu_create_circle();
	Circle * circle3 = menu_create_circle();

	circle1->r = 60;
	circle2->r = 60;
	circle3->r = 60;

	circle1->x = 512;
	circle1->y = 384;

	circle2->x = 512;
	circle2->y = 384;

	circle3->x = 512;
	circle3->y = 384;

	circle1->c = 2;
	circle2->c = 54;
	circle3->c = 4;

	int count = 3;

	while (!stop) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			return 1;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_rtc) {

					rtc_int_handler();

					//do nothing
				}
				if (msg.NOTIFY_ARG & irq_set_kbd) {

					keyboard_int_handler();

					//do nothing
				}
				if (msg.NOTIFY_ARG & irq_set_timer) {

					switch(count) {
						case 3:
							vg_sprite(tres, 487, 374, 1);
							break;
						case 2:
							vg_sprite(dois, 487, 374, 1);
							break;
						case 1:
							vg_sprite(um, 487, 374, 1);
							break;
						default:
							vg_sprite(um, 487, 374, 0);
							break;
					}

					int i;

					switch(count) {
						case 3:
							for(i = 60; i > circle3->r; i--) {
								vg_circle(circle3->x, circle3->y, i, 0);
							}

							if (tick_menu_circle(circle3))
								count--;
							break;
						case 2:
							for(i = 60; i > circle2->r; i--) {
								vg_circle(circle2->x, circle2->y, i, 0);
							}

							if (tick_menu_circle(circle2))
								count--;
							break;
						case 1:
							for(i = 60; i > circle1->r; i--) {
								vg_circle(circle1->x, circle1->y, i, 0);
							}

							if (tick_menu_circle(circle1))
								count--;
							break;
						default:
							break;
					}

					//Re-apply the cursor again to make sure it isn't erased by other circles that may appear
					draw_cursor(cursor_easy);

					vg_nextFrame();

					if(count == 0)
						stop = 1;

				}
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					mouse_int_handler();

					if (getCounter() == 0) {

						updateCoord();

						int dx = getX(), dy = getY();

						update_cursor(cursor_easy, dx, dy);
					}
				}
				break;
			default:
				break;
			}
		} else {
		}
	}

	//Reset the variables
	nrIntTimer = 0;
	stop = 0;

	while (!stop) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			return 1;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_rtc) {

					rtc_int_handler();

					//do nothing
				}
				if (msg.NOTIFY_ARG & irq_set_kbd) {

					keyboard_int_handler();

					//Get last code
					unsigned long currentCode = getLastCode();

					//Check if it needs to print the code
					if (currentCode == TWOBYTE)
						continue;

					//ESC key was pressed, stop the loop
					if (currentCode == ESC) {
						setCurrentState(END_GAME);
						stop = 1;
					}

					if (!(currentCode & BREAK)) {

						if (!empty(circles)) {
							//Temporary circle
							Circle* clicked = NULL;

							//Gets the first circle in queue
							dequeue(circles, &clicked);

							//Calls Game_Core state machine
							event_t key_p = (event_t) { KEY_PRESS, currentCode, clicked, cursor_easy };
							mosu_handler(&key_p);

							//No life left?
							if (getCurrentState() == END_GAME)
								stop = 1;

							enqueue(past_circles, clicked);
						}
					}
				}
				if (msg.NOTIFY_ARG & irq_set_timer) {

					//Create new circle every next_circle frames
					if (nrIntTimer % next_circle == 0) {

						//Create circle and add it to array
						Circle * new_circle = create_circle(0);

						//Push circle to queue
						enqueue(circles, new_circle);
					}

					//Temporary queues
					queue tmp_q1 = q_new();
					queue tmp_q2 = q_new();

					//Tick the main and outer circles from the "circles" queue
					while (!empty(circles)) {

						Circle* ticked = NULL;

						dequeue(circles, &ticked);

						if (ticked != NULL) {
							if (tick_mainCircle(ticked)) {

								//Calls Game_Core state machine
								event_t missed = (event_t) { MISSED, 0x00, ticked, cursor_easy };
								mosu_handler(&missed);

								//No life left?
								if (getCurrentState() == END_GAME)
									stop = 1;

								enqueue(past_circles, ticked);
							}
							else {
								tick_outerCircle(ticked);
								enqueue(tmp_q1, ticked);
							}
						}
					}

					//Recreate "circles" queue
					circles = tmp_q1;

					//Tick the "circles_past" queue
					while (!empty(past_circles)) {

						Circle* ticked = NULL;
						dequeue(past_circles, &ticked);

						if (ticked != NULL) {
							if (tick_endCircle(ticked)) {
								destroy_circle(ticked);
								continue;
							} else
								enqueue(tmp_q2, ticked);
						}
					}

					//Recreate "circles_past" queue
					past_circles = tmp_q2;

					//Draw rectangle with player's life indication
					vg_drawRectangle(getPlayer().life);

					//Re-apply the cursor again to make sure it isn't erased by other circles that may appear
					draw_cursor(cursor_easy);

					vg_nextFrame();

					//Speed up!
					if (nrIntTimer % 300 == 0) {
						if (next_circle > 30)
							next_circle -= 5;
						else if (next_circle > 10)
							next_circle--;
					}

					nrIntTimer++;

				}
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					mouse_int_handler();

					if (getCounter() == 0) {

						updateCoord();

						int dx = getX(), dy = getY();

						update_cursor(cursor_easy, dx, dy);
					}
				}
				break;
			default:
				break;
			}
		} else {
		}
	}

	//Free allocated memory
	while (!empty(past_circles)) {
		Circle* ticked = NULL;
		dequeue(past_circles, &ticked);
		destroy_circle(ticked);
	}

	while (!empty(circles)) {
		Circle* ticked = NULL;
		dequeue(circles, &ticked);
		destroy_circle(ticked);
	}

	//Free allocated space for cursor
	destroy_cursor(cursor_easy);

	return 0;
}

int game_mode_hard() {

	//Queue where we will save the circles
	queue circles = q_new();
	queue past_circles = q_new();

	//Cursor
	Cursor * cursor_hard = create_cursor();

	int ipc_status, r;
	message msg;
	int irq_set_kbd = BIT(kbd_id);
	int irq_set_mouse = BIT(mouse_id);
	int irq_set_timer = BIT(timer_id);
	int irq_set_rtc = BIT(rtc_id);

	int nrIntTimer = 0;
	int stop = 0;

	//Start clock with 60Hz
	timer_set_frequency(0, 60);

	int next_circle = 60;

	Circle * circle1 = menu_create_circle();
	Circle * circle2 = menu_create_circle();
	Circle * circle3 = menu_create_circle();

	circle1->r = 60;
	circle2->r = 60;
	circle3->r = 60;

	circle1->x = 512;
	circle1->y = 384;

	circle2->x = 512;
	circle2->y = 384;

	circle3->x = 512;
	circle3->y = 384;

	circle1->c = 2;
	circle2->c = 54;
	circle3->c = 4;

	int count = 3;

	while (!stop) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			return 1;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_rtc) {

					rtc_int_handler();

					//do nothing
				}
				if (msg.NOTIFY_ARG & irq_set_kbd) {

					keyboard_int_handler();

					//do nothing
				}
				if (msg.NOTIFY_ARG & irq_set_timer) {

					switch(count) {
						case 3:
							vg_sprite(tres, 487, 374, 1);
							break;
						case 2:
							vg_sprite(dois, 487, 374, 1);
							break;
						case 1:
							vg_sprite(um, 487, 374, 1);
							break;
						default:
							vg_sprite(um, 487, 374, 0);
							break;
					}

					int i;

					switch(count) {
					case 3:
						for(i = 60; i > circle3->r; i--)
						{
							vg_circle(circle3->x, circle3->y, i, 0);
						}

						if (tick_menu_circle(circle3))
							count--;
						break;
					case 2:
						for(i = 60; i > circle2->r; i--)
						{
							vg_circle(circle2->x, circle2->y, i, 0);
						}

						if (tick_menu_circle(circle2))
							count--;
						break;
					case 1:
						for(i = 60; i > circle1->r; i--)
						{
							vg_circle(circle1->x, circle1->y, i, 0);
						}

						if (tick_menu_circle(circle1))
							count--;
						break;
					default:
						break;
					}

					//Re-apply the cursor again to make sure it isn't erased by other circles that may appear
					draw_cursor(cursor_hard);

					vg_nextFrame();

					if(count == 0)
						stop = 1;

				}
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					mouse_int_handler();

					if (getCounter() == 0) {

						updateCoord();

						int dx = getX(), dy = getY();

						update_cursor(cursor_hard, dx, dy);
					}
				}
				break;
			default:
				break;
			}
		} else {
		}
	}

	//Reset the variables
	nrIntTimer = 0;
	stop = 0;

	while (!stop) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			return 1;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_rtc) {

					rtc_int_handler();

					//do nothing
				}
				if (msg.NOTIFY_ARG & irq_set_kbd) {

					keyboard_int_handler();

					//Get last code
					unsigned long currentCode = getLastCode();

					//Check if it needs to print the code
					if (currentCode == TWOBYTE)
						continue;

					//ESC key was pressed, stop the loop
					if (currentCode == ESC) {
						setCurrentState(END_GAME);
						stop = 1;
					}

					if (!(currentCode & BREAK)) {

						if (!empty(circles)) {
							//Temporary circle
							Circle* clicked = NULL;

							//Gets the first circle in queue
							dequeue(circles, &clicked);

							//Calls Game_Core state machine
							event_t key_p = (event_t ) { KEY_PRESS, currentCode, clicked, cursor_hard };
							mosu_handler(&key_p);

							//No life left?
							if (getCurrentState() == END_GAME)
								stop = 1;

							enqueue(past_circles, clicked);
						}
					}
				}
				if (msg.NOTIFY_ARG & irq_set_timer) {

					//Create new circle every next_circle frames
					if (nrIntTimer % next_circle == 0) {

						//Create circle and add it to array
						Circle * new_circle = create_circle(1);

						//Push circle to queue
						enqueue(circles, new_circle);
					}

					//Temporary queues
					queue tmp_q1 = q_new();
					queue tmp_q2 = q_new();

					//Tick the main and outer circles from the "circles" queue
					while (!empty(circles)) {

						Circle* ticked = NULL;

						dequeue(circles, &ticked);

						if (ticked != NULL) {

							if (tick_mainCircle(ticked)) {
								printf("Missed circle \n");

								//Calls Game_Core state machine
								event_t missed = (event_t ) { MISSED, 0x00, ticked, cursor_hard };
								mosu_handler(&missed);

								//No life left?
								if (getCurrentState() == END_GAME)
									stop = 1;

								enqueue(past_circles, ticked);
							}
							else {
								tick_outerCircle(ticked);
								enqueue(tmp_q1, ticked);
							}
						}
					}

					//Recreate "circles" queue
					circles = tmp_q1;

					//Tick the "circles_past" queue
					while (!empty(past_circles)) {

						Circle* ticked = NULL;
						dequeue(past_circles, &ticked);

						if (ticked != NULL) {

							if (tick_endCircle(ticked)) {
								destroy_circle(ticked);
								continue;
							}
							else
								enqueue(tmp_q2, ticked);
						}
					}

					//Recreate "circles_past" queue
					past_circles = tmp_q2;

					//Draw rectangle with player's life indication
					vg_drawRectangle(getPlayer().life);

					//Re-apply the cursor again to make sure it isn't erased by other circles that may appear
					draw_cursor(cursor_hard);

					vg_nextFrame();

					//Speed up!
					if (nrIntTimer % 300 == 0) {
						if (next_circle > 30)
							next_circle -= 5;
						else if (next_circle > 10)
							next_circle--;
					}

					nrIntTimer++;

				}
				if (msg.NOTIFY_ARG & irq_set_mouse) {

					mouse_int_handler();

					if (getCounter() == 0) {

						updateCoord();

						int dx = getX(), dy = getY();

						update_cursor(cursor_hard, dx, dy);
					}
				}
				break;
			default:
				break;
			}
		}
		else {
		}
	}

	//Free allocated memory
	while (!empty(past_circles)) {
		Circle* ticked = NULL;
		dequeue(past_circles, &ticked);
		destroy_circle(ticked);
	}

	while (!empty(circles)) {
		Circle* ticked = NULL;
		dequeue(circles, &ticked);
		destroy_circle(ticked);
	}

	//Free allocated space for cursor
	destroy_cursor(cursor_hard);

	return 0;
}
