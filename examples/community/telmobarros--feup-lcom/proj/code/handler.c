#include <stdio.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdbool.h>
#include "i8042.h"
#include "i8254.h"
#include "handler.h"
#include "timer.h"
#include "kbd.h"
#include "mouse.h"
#include "video_gr.h"
#include "piece.h"
#include "sprite.h"
#include "bitmap.h"
#include "vbe.h"
#include "rtc.h"
#include "serial.h"


Mouse_t mouse;

static long int packet[3];
static unsigned int packet_counter = 0 ;

unsigned long code;
bool is_two_byte = false;

int timer_irq_set;
int kbd_irq_set;
int mouse_irq_set;
int rtc_irq_set;
int serial_irq_set;

unsigned int selecao = 0;

unsigned int highscores[3];

int mainhandler(){
	vg_init (GRAPHICS_MODE_1024_768_256);				//Initialization of graphics mode in 1024x768 resolution
	timer_irq_set = timer_subscribe_int();
	kbd_irq_set = kbd_subscribe_int();
	mouse_irq_set = mouse_subscribe_int();
	rtc_irq_set = rtc_subscribe_int();
	serial_irq_set = serial_subscribe_int();
	write_to_mouse();
	enable_packets();
	mouse.map=map_Bitmap("/home/lcom/proj/code/img/mouse.bmp", &mouse.width, &mouse.height);
	rtc_get_highscores(highscores);


	//display presentation image
	Bitmap * loading1 = loadBitmap("/home/lcom/proj/code/img/loading1.bmp");
	Bitmap * loading2 = loadBitmap("/home/lcom/proj/code/img/loading2.bmp");

	int ipc_status;
	message msg;
	int r;
	int counter = 0;
	while( code != BREAKCODE && counter < 8*60) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) {
					mouse_packet_handler();

				}
				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
					kbd_int_handler();
				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					counter++;
					if (counter < 1 * 60){
						vg_fadein((uint16_t *)loading1->bitmapData);
					}else if (counter < 3* 60){
						drawBitmap(loading1, 0, 0, ALIGN_LEFT);
					} else if (counter < 4* 60){
						vg_darker();
					}else if (counter < 5 * 60){
						vg_fadein((uint16_t *)loading2->bitmapData);
					}else if (counter < 7* 60){
						drawBitmap(loading2, 0, 0, ALIGN_LEFT);
					} else if (counter < 8* 60){
						vg_darker();
					}
					vg_buffer();
				}
				if (msg.NOTIFY_ARG & serial_irq_set) {
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	deleteBitmap(loading1);
	deleteBitmap(loading2);
	code = 0;

	while (selecao != 6){
		menu_handler();

		switch (selecao){
		case 1:
			game_handler();
			selecao = 0;
			break;
		case 2:
			multi_game_handler();
			selecao = 0;
			break;
		case 3:
			battle_game_handler();
			selecao = 0;
			break;
		case 4:
			highscores_handler();
			selecao = 0;
			break;
		case 5:
			instructions_handler();
			selecao = 0;
			break;
		default:
			break;

		}
		code = 0;
	}
	vg_exit();
	timer_unsubscribe_int();
	kbd_unsubscribe_int();
	mouse_unsubscribe_int();
	rtc_unsubscribe_int();
	serial_unsubscribe_int();


	return 0;
}

int mouse_packet_handler(){
	packet[packet_counter] = mouse_output();
	if (packet_counter == 0)
		//Verifica se é o primeiro packet através da verificação do bit 3 (grande probabilidade)
		if ((packet[packet_counter] & ISFIRSTPACKET) == 0)
			return 0;

	//incrementa as variáveis referentes ao contador de packets e ao geral
	packet_counter ++;

	if(packet_counter == 3)
	{
		//Reset da variável contador dos packets
		packet_counter = 0;

		mouse.deltax = packet[1] - 256 *((packet[0] & XSIGN) >> 4);
		mouse.deltay = packet[2] - 256 * ((packet[0] & YSIGN) >> 5);

		if (mouse.x + mouse.deltax > 2 && mouse.x + mouse.deltax + mouse.width < getH_res() &&
				mouse.y - mouse.deltay > 2 && mouse.y -mouse.deltay + mouse.height< getV_res()){
			mouse.x += mouse.deltax;
			mouse.y -= mouse.deltay;
		}
		mouse.left_btn = packet[0] & LB;
		mouse.middle_btn = (packet[0] & MB) >> 2;
		mouse.right_btn = (packet[0] & RB) >> 1;


	}
	return 0;
}

main_menu_event main_menu_event_handler(Menu* menu){

	if(mouse.left_btn == 1)
		return LEFT_CLICK;
	else {
		if(mouse.x < menu->buttons[0].x || mouse.x > (menu->buttons[0].x + menu->buttons[0].width))
			return NOACTION;
		else{
			unsigned int i, index = 0;
			for(i = 0; i < 6; i++){
				if(mouse.y >= menu->buttons[i].y && mouse.y <= (menu->buttons[i].y+ menu->buttons[i].height))
					break;
				index++;
			}
			if(index == 0)
				return BUTTON0_ISABOVE;
			else if(index == 1)
				return BUTTON1_ISABOVE;
			else if(index == 2)
				return BUTTON2_ISABOVE;
			else if(index == 3)
				return BUTTON3_ISABOVE;
			else if(index == 4)
				return BUTTON4_ISABOVE;
			else if(index == 5)
				return BUTTON5_ISABOVE;
			else return NOACTION;
		}
	}
}

void selecao_handler(Menu* menu){
	if (menu->state == SINGLEPLAYER)
		selecao = 1;
	if (menu->state == MULTIPLAYER)
		selecao = 2;
	if (menu->state == BATTLE)
		selecao = 3;
	if (menu->state == HIGHSCORES)
		selecao = 4;
	if (menu->state == INSTRUCTIONS)
		selecao = 5;
	if (menu->state == SHUTDOWN)
		selecao = 6;
}


int menu_handler(){

	Menu * main_menu = new_main_menu();

	int ipc_status;
	message msg;
	int r;
	int counter = 0; //Inicialização do contador
	unsigned long serial_info;

	serial_int_handler(&serial_info);

	while(selecao == 0) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */
					mouse_packet_handler(mouse);
				}
				if (msg.NOTIFY_ARG & kbd_irq_set) {
					kbd_int_handler();
				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					counter++;
					main_menu->event = main_menu_event_handler(main_menu);
					update_main_menu_state(main_menu);
					update_main_menu(main_menu);
					draw_main_menu(main_menu);
					rtc_draw_current_time(RTC_TIME_X,RTC_TIME_Y);
					vg_map_transparent(mouse.map, mouse.x, mouse.y, mouse.width, mouse.height, 0);
					vg_buffer();
					selecao_handler(main_menu);
				}
				if (msg.NOTIFY_ARG & serial_irq_set) {
					if (serial_int_handler(&serial_info) == 2){
						if (serial_info == 'M'){
							selecao = 2;
						}
					}
				}
			default:
				break;
			}
		} else {
		}
	}
	if (selecao == 2 && main_menu->state == MULTIPLAYER){
		serial_write_char('M');
		serial_int_handler(&serial_info);
	}

	delete_main_menu(main_menu);
	return 0;
}


int kbd_int_handler() {
	code = kbd_output();
	if (code == 0xE0)
	{
		is_two_byte= true;
		return 0;
	}
	else if (is_two_byte)
	{
		code |= (0xE0 << 8);
		is_two_byte = false;
	}
}


kbd_game_event kbd_event_handler(){
	if(code == UPKEY_MC)
	{
		return UPKEY_DOWN;
	}
	else if(code == DOWNKEY_MC)
	{
		return DOWNKEY_DOWN;
	}
	else if(code == RIGHTKEY_MC)
	{
		return RIGHTKEY_DOWN;
	}
	else if(code == LEFTKEY_MC)
	{
		return LEFTKEY_DOWN;
	}
	else if(code == SPACEBAR_MC)
	{
		return SPACEBAR_DOWN;
	}
	else return NOKEY;
}

mouse_game_event mouse_event_handler(){
	if (mouse.left_btn == 1)
		return MOUSE_LEFT_BTN;
	if (mouse.right_btn == 1)
		return MOUSE_RIGHT_BTN;
	if(mouse.deltax > 50)
	{
		return MOUSE_RIGHT;
	}
	if(mouse.deltax < -50)
	{
		return MOUSE_LEFT;
	}
	if (mouse.middle_btn == 1)
		return MOUSE_MIDDLE_BTN;
	return MOUSE_STOPPED;
}

timer_game_event timer_event_handler(unsigned long counter, unsigned int delay){
	if( counter%delay == 0)
		return FALL_TICK;
	else return NO_TICK;
}


int game_handler(){
	Game * game;
	game = new_game(0);


	int ipc_status;
	message msg;
	int r;
	unsigned long counter = 0; //Inicialização do contador

	char buffer[10];

	while( code != BREAKCODE && game->state != GAME_OVER) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) {
					mouse_packet_handler();
					if (packet_counter == 0){
						game->mouse_event = mouse_event_handler();
						update_gamestate(game,1);
						update_game(game);
					}
				}
				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
					kbd_int_handler();
					game->last_kbd_event = game->kbd_event;
					game->kbd_event = kbd_event_handler();
					if (game->kbd_event != NOKEY){
						update_gamestate(game,0);
						update_game(game);
					}
				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					counter++;
					game->timer_event = timer_event_handler(counter, game->fall_delay);
					if(game->timer_event != NO_TICK){
						update_gamestate(game,0);
						update_game(game);
					}
					draw_game(game);
					vg_counter(game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_COUNTER_Y, counter);
					//sprintf(buffer, "%d", counter);
					//vg_string(buffer,0,0,2,WHITE);
					vg_buffer();
				}
				if (msg.NOTIFY_ARG & serial_irq_set) {
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	if (game->points >= 2560){
		game->points = 2560;
	}
	if (game->points >= highscores[0]){
		highscores[2]=highscores[1];
		highscores[1]=highscores[0];
		highscores[0] = game->points;
	} else if (game->points >= highscores[1]){
		highscores[2]=highscores[1];
		highscores[1] = game->points;
	} else if (game->points >= highscores[2]){
		highscores[2] = game->points;
	}

	rtc_set_highscores(highscores);
	delete_game(game);

	return 0;
}

int serial_int_handler(unsigned long * temp){
	int int_type;
	int_type = serial_interrupt_identification();
	switch (int_type){
	case -1:
		//for debugging purposes
		//vg_string("UART DID NOT GENERATE THAT INTERRUPT",0,0,2,WHITE);
		break;
	case 0:
		//for debugging purposes
		//vg_string("MODEM STATUS",0,50,2,WHITE);
		break;
	case 1:
		//for debugging purposes
		//vg_string("TRANSMITTER EMPTY",0,100,2,WHITE);
		sys_inb(COM1_ADDRESS + INTERRUPT_IDENTIFICATION_IIR, temp);		//RESET
		break;
	case 2:

		//for debugging purposes
		//vg_string("RECEIVED DATA AVAILABLE",0,150,2,WHITE);
		*temp = serial_read_char();											//RESET
		break;
	case 3:
		//for debugging purposes
		//vg_string("LINE STATUS",0,200,2,WHITE);
		sys_inb(COM1_ADDRESS + LINE_STATUS_LSR, temp);						//RESET
		break;
	case 4:
		//for debugging purposes
		//vg_string("CHARACTER TIMEOUT INDICATION",0,250,2,WHITE);
		*temp = serial_read_char();											//RESET
		break;
	default:
		break;
	}

	return int_type;
}

int multi_game_handler(){
	Game * game;
	game = new_game(1);

	int ipc_status;
	message msg;
	int r;
	unsigned long counter = 0; //Inicialização do contador

	char buffer[10];
	unsigned long serial_info;

	serial_int_handler(&serial_info);
	serial_write_char('R');
	while(serial_info != 'R') {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) {
					mouse_packet_handler();
				}
				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
					kbd_int_handler();
				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					vg_string("Waiting your oponent",300,200,2, WHITE);
					vg_buffer();
				}
				if (msg.NOTIFY_ARG & serial_irq_set) {
					serial_int_handler(&serial_info);
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
		if (serial_info == 'R')
			break;
	}

	while(game->state != GAME_OVER) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) {
					mouse_packet_handler();
					if (packet_counter == 0){
						game->mouse_event = mouse_event_handler();
						update_gamestate(game,1);
						update_game(game);
					}
				}
				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
					kbd_int_handler();
					char temp;
					temp = serial_read_char();
					unsigned long tmp;
					sys_inb(COM1_ADDRESS + LINE_STATUS_LSR, &tmp);
					game->last_kbd_event = game->kbd_event;
					game->kbd_event = kbd_event_handler();
					if (game->kbd_event != NOKEY){
						update_gamestate(game,0);
						update_game(game);
					}
				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					counter++;
					game->timer_event = timer_event_handler(counter, game->fall_delay);
					if(game->timer_event != NO_TICK){
						update_gamestate(game,0);
						update_game(game);
					}
					draw_game(game);
					vg_counter(game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_COUNTER_Y, counter);
					vg_buffer();
				}
				if (msg.NOTIFY_ARG & serial_irq_set) {
					if (serial_int_handler(&serial_info) == 2){
						if (serial_info != 'G'){
							game->lines_received += serial_info;
						}
					}
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
		if (serial_info == 'G')
			break;
	}

	while( code != BREAKCODE) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) {
					mouse_packet_handler();

				}
				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
					kbd_int_handler();
				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					vg_rectangle(0,0,1024,768,BLACK);
					if (game->state == GAME_OVER){
						vg_string("YOU LOSE",300,200,2, WHITE);
					} else {
						vg_string("YOU WON",340,200,2, WHITE);
					}
					vg_string("Press ESC to go back",350,400,2, WHITE);
					vg_buffer();
				}
				if (msg.NOTIFY_ARG & serial_irq_set) {
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	delete_game(game);

	return 0;
}

kbd_character_event kbd_char_event_handler(){
	switch (code){
	case A_MC: return A_DOWN;
	case A_BC: return A_UP;
	case W_MC: return W_DOWN;
	case W_BC: return W_UP;
	case S_MC: return S_DOWN;
	case S_BC: return S_UP;
	case D_MC: return D_DOWN;
	case D_BC: return D_UP;
	default: return NO_KEY;
	}
}

timer_character_event timer_char_event_handler(unsigned long * counter){
	if ((*counter)%CHAR_MOVE_FRAME_DELAY == 0){
		return MOVE_FRAME_TICK;
	} else {
		return NOTICK;
	}
}

int battle_game_handler(){
	Game * game;
	game = new_game(0);
	Character * character;
	character = new_character();


	int ipc_status;
	message msg;
	int r;
	unsigned long counter = 0; //Inicialização do contador

	char buffer[10];

	while( code != BREAKCODE && game->state != GAME_OVER && character->state != GAMEOVER) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) {
					mouse_packet_handler();
					if (packet_counter == 0){
						game->mouse_event = mouse_event_handler();
						update_gamestate(game,1);
						update_game(game);
					}
				}
				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
					kbd_int_handler();
					game->last_kbd_event = game->kbd_event;
					game->kbd_event = kbd_event_handler();
					if (game->kbd_event != NOKEY){
						update_gamestate(game,0);
						update_game(game);
					}

					character->kbd_event = kbd_char_event_handler();
					update_character_state(character);
					//update_character(character);

				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					if (char_piece_collision(character,game->actual_piece) == 0){
						character->state = GAMEOVER;
						break;
					}

					counter++;
					game->timer_event = timer_event_handler(counter, game->fall_delay);
					if(game->timer_event != NO_TICK){
						update_gamestate(game,0);
						update_game(game);
					}
					character->timer_event = timer_char_event_handler(&counter);
					update_character_state(character);
					update_character(character);

					draw_game(game);
					draw_character(character);
					vg_counter(game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_COUNTER_Y, counter);
					vg_buffer();
				}
				if (msg.NOTIFY_ARG & serial_irq_set) {
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	while( code != BREAKCODE) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) {
					mouse_packet_handler();

				}
				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
					kbd_int_handler();
				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					vg_rectangle(0,0,1024,768,BLACK);
					if (game->state == GAME_OVER){
						vg_string("CHARACTER WON",300,200,2, WHITE);
					} else {
						vg_string("PIECES WON",350,200,2, WHITE);
					}
					vg_string("Press ESC to go back",350,400,2, WHITE);
					vg_buffer();
				}
				if (msg.NOTIFY_ARG & serial_irq_set) {
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	delete_game(game);
	delete_character(character);
	return 0;
}

int highscores_handler(){

	Bitmap * highscores_background = loadBitmap("/home/lcom/proj/code/img/highscores_background.bmp");

	int ipc_status;
	message msg;
	int r;

	char temp[8];

	while( code != BREAKCODE) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) {
					mouse_packet_handler();

				}
				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
					kbd_int_handler();
				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					drawBitmap(highscores_background, 0, 0, ALIGN_LEFT);

					sprintf(temp, "%d", highscores[0]);
					vg_string(temp,450,240,2,BLACK);
					sprintf(temp, "%d", highscores[1]);
					vg_string(temp,450,360,2,BLACK);
					sprintf(temp, "%d", highscores[2]);
					vg_string(temp,450,484,2,BLACK);

					rtc_draw_current_time(RTC_TIME_X,RTC_TIME_Y);

					vg_buffer();
				}
				if (msg.NOTIFY_ARG & serial_irq_set) {
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	deleteBitmap(highscores_background);
	return 0;
}

int instructions_handler(){

	Bitmap * instructions[4];
	instructions[0] = loadBitmap("/home/lcom/proj/code/img/instructions1.bmp");
	instructions[1] = loadBitmap("/home/lcom/proj/code/img/instructions2.bmp");
	instructions[2] = loadBitmap("/home/lcom/proj/code/img/instructions3.bmp");
	instructions[3] = loadBitmap("/home/lcom/proj/code/img/instructions4.bmp");
	int ipc_status;
	message msg;
	int r;

	char temp[8];

	unsigned int current = 0;

	while( code != BREAKCODE) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) {
					mouse_packet_handler();

				}
				if (msg.NOTIFY_ARG & kbd_irq_set) { /* subscribed interrupt */
					kbd_int_handler();
					if (code == RIGHTKEY_MC){
						if (current < 3){
							current++;
						}
					}else if (code == LEFTKEY_MC){
						if (current > 0){
							current--;
						}
					}
				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					drawBitmap(instructions[current], 0, 0, ALIGN_LEFT);

					rtc_draw_current_time(RTC_TIME_X,RTC_TIME_Y);

					vg_buffer();
				}
				if (msg.NOTIFY_ARG & serial_irq_set) {
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	unsigned int i;
	for(i = 0; i < 4;i++){
		deleteBitmap(instructions[i]);
	}

	return 0;
}
