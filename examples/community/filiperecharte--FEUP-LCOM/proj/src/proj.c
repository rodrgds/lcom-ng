#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "timer.h"
#include "keyboard.h"
#include "videocard.h"
#include "mouse.h"
#include "i8254.h"
#include "game.h"
#include "snake.h"
#include "rtc.h"
#include "menusnake.xpm"
#include "mouse_sprite.xpm"
#include "Snake_game_background.xpm"
#include "Game_Over.xpm"
#include "dodge_the_fruits.xpm"
#include "menu_exit_highlighted.xpm"
#include "menu_play_highlighted.xpm"
#include "menu_leaderboard_highlighted.xpm"
#include "leaderboard.xpm"
#include "apple.xpm"
#include "fruit.h"
#include "sprites.xpm"
#include "enter_username.xpm"
#include "leaderboard.h"


extern int keyboard_hook_id;
extern int timer_hook_id;
extern int mouse_hook_id;
extern uint32_t key_pressed;
extern uint8_t mouse_info;
extern int coordinate_x_mouse;
extern int coordinate_y_mouse;
extern int timer_counter;
extern unsigned int time_until_spawn;
extern unsigned int time_to_speed_up;
extern enum game_state state;
extern enum direction snake_dir;
extern char name[100];


int main(int argc, char* argv[]) {
	// sets the language of LCF messages (can be either EN-US or PT-PT)
	lcf_set_language("EN-US");

	// enables to log function invocations that are being "wrapped" by LCF
	// [comment this out if you don't want/need it]
	lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

	// enables to save the output of printf function calls on a file
	// [comment this out if you don't want/need it]
	lcf_log_output("/home/lcom/labs/proj/output.txt");

	// handles control over to LCF
	// [LCF handles command line arguments and invokes the right function]
	if (lcf_start(argc, argv))
		return 1;

	// LCF clean up tasks
	// [must be the last statement before return]
	lcf_cleanup();

	return 0;
}
/**
* @brief Runs project
*
* Loop that runs the project
*
* @param argc - arguemnts for initializing
* @param argv - arguments for initializing
* @return Returns 0 on success, 1 otherwise
*/
int(proj_main_loop)(int argc, char* argv[]) {

	// Variable declaration and image initialization

	int r;
	int ipc_status;
	message msg;
	uint8_t keyboard_irq_set, mouse_irq_set, timer_irq_set;
	int packet_counter = 0;
	struct packet pkt;
	enum xpm_image_type type = XPM_5_6_5;

	enum direction snake_dir = RIGHT;
	enum game_state state = MAIN_MENU;

	xpm_image_t img_background;
	xpm_image_t img_game_background;
	xpm_image_t img_mouse;

	xpm_image_t img_snake_head_right;
	xpm_image_t img_snake_head_left;
	xpm_image_t img_snake_head_up;
	xpm_image_t img_snake_head_down;
	xpm_image_t img_snake_body_h;
	xpm_image_t img_snake_body_v;
	xpm_image_t img_snake_tail_right;
	xpm_image_t img_snake_tail_left;
	xpm_image_t img_snake_tail_up;
	xpm_image_t img_snake_tail_down;

	xpm_image_t img_dodge;
	xpm_image_t img_game_over;
	xpm_image_t img_leaderboard_background;
	xpm_image_t img_play_highlight;
	xpm_image_t img_exit_highlight;
	xpm_image_t img_leaderboard_highlight;
	xpm_image_t img_apple;
	xpm_image_t img_username;

	void* background = NULL;
	void* background_game = NULL;
	void* leaderboard_background = NULL;
	void* play_highlight_background = NULL;
	void* exit_highlight_background = NULL;
	void* leaderboard_highlight_background=NULL;
	void* dodge = NULL;

	uint8_t* xpm_cursor_adr = xpm_load(mouse, type, &img_mouse);
	uint8_t* xpm_background_adr = xpm_load(menu_xpm, type, &img_background);
	uint8_t* xpm_game_background_adr = xpm_load(Snake_game_background_xpm, type, &img_game_background);
	uint8_t* xpm_leaderboard_background_adr = xpm_load(leaderboard_background_xpm, type, &img_leaderboard_background);
	uint8_t* snake_head_right_adr = xpm_load(snake_head_right_xpm, type, &img_snake_head_right);
	uint8_t* snake_head_left_adr = xpm_load(snake_head_left_xpm, type, &img_snake_head_left);
	uint8_t* snake_head_up_adr = xpm_load(snake_head_up_xpm, type, &img_snake_head_up);
	uint8_t* snake_head_down_adr = xpm_load(snake_head_down_xpm, type, &img_snake_head_down);

	uint8_t* snake_body_h_adr = xpm_load(snake_body_h_xpm, type, &img_snake_body_h);
	uint8_t* snake_body_v_adr = xpm_load(snake_body_v_xpm, type, &img_snake_body_v);

	uint8_t* snake_tail_right_adr = xpm_load(snake_tail_right_xpm, type, &img_snake_tail_right);
	uint8_t* snake_tail_left_adr = xpm_load(snake_tail_left_xpm, type, &img_snake_tail_left);
	uint8_t* snake_tail_up_adr = xpm_load(snake_tail_up_xpm, type, &img_snake_tail_up);
	uint8_t* snake_tail_down_adr = xpm_load(snake_tail_down_xpm, type, &img_snake_tail_down);

	uint8_t* play_highlight_adr = xpm_load(menu_exit_highlighted, type, &img_exit_highlight);
	uint8_t* exit_highlight_adr = xpm_load(menu_play_highlighted, type, &img_play_highlight);
	uint8_t* leaderboard_highlight_adr = xpm_load(menu_leaderboard_highlighted,type,&img_leaderboard_highlight);

	uint8_t* dodge_adr = xpm_load(dodge_the_fruits, type, &img_dodge);
	uint8_t* game_over_adr = xpm_load(game_over, type, &img_game_over);
	uint8_t* apple_adr = xpm_load(apple_xpm, type, &img_apple);
	uint8_t* username_adr = xpm_load(enter_username_xpm,type,&img_username);

	loadAlphabet();

	uint32_t spawn_counter = 0;
	uint32_t increase_snake = 0;
	int score = 0;

	bool writed_leaderboard=false;

	GameScore ng;
	char date[100];

	GameScore scores[10];

	read_file(scores,10);
	// Mode and I/O setup

	if (vg_init(0x11A) == NULL) {
		if (vg_exit() != 0)
			return 1;
	}

	if (keyboard_subscribe_int(&keyboard_irq_set) != 0)
		return 1;

	if (mouse_subscribe_int(&mouse_irq_set) != 0)
		return 1;

	if (timer_subscribe_int(&timer_irq_set) != 0)
		return 1;

	timer_set_frequency(0, 1);
	//print_date();

	while (state != EXIT) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.m_notify.interrupts & keyboard_irq_set) {
					kbc_ih(); // Get key
					keyboard_event_handler(&state, &key_pressed, &snake_dir);
				}
				if (msg.m_notify.interrupts & mouse_irq_set) {
					mouse_ih();
					if (packet_counter == 0) {
						if ((mouse_info & BIT(3)) != 0) {
							pkt.bytes[0] = mouse_info;
							packet_counter++;
						}
						else
							continue;
					}
					else if (packet_counter == 1) {
						pkt.bytes[1] = mouse_info;
						packet_counter++;
					}
					else {
						pkt.bytes[2] = mouse_info;
						parse_packet(&pkt);
						packet_counter = 0;
						mouse_event_handler(img_mouse,xpm_cursor_adr,&state, &pkt, background_game, apple_adr, img_apple, xpm_game_background_adr, img_game_background);
					}
				}
				if (msg.m_notify.interrupts & timer_irq_set) {
					switch (state) {
					case MAIN_MENU:
						score=0;
						spawn_counter = 0;
						timer_counter = 0;
						time_until_spawn = 100;
						writed_leaderboard=false;
						memset(name, 0, strlen(name));

						if (background == NULL) {
							draw_xpm(xpm_background_adr, img_background, 0, 0);
							background = malloc(getSize());
							memcpy(background, getBuffer(), getSize());
						}
						else {
							memcpy(getBuffer(), background, getSize());
						}
						draw_buffer();

						draw_xpm(xpm_cursor_adr, img_mouse, coordinate_x_mouse, coordinate_y_mouse);
						draw_buffer();
						break;

					case PLAY_HIGHLIGHT:
						if (play_highlight_background == NULL) {
							draw_xpm(play_highlight_adr, img_play_highlight, 0, 0);
							play_highlight_background = malloc(getSize());
							memcpy(play_highlight_background, getBuffer(), getSize());
						}
						else {
							memcpy(getBuffer(), play_highlight_background, getSize());
						}
						draw_buffer();

						draw_xpm(xpm_cursor_adr, img_mouse, coordinate_x_mouse, coordinate_y_mouse);
						draw_buffer();
						break;

					case LEADERBOARD_HIGHLIGHT:
						if (leaderboard_highlight_background == NULL) {
							draw_xpm(leaderboard_highlight_adr, img_leaderboard_highlight, 0, 0);
							leaderboard_highlight_background = malloc(getSize());
							memcpy(leaderboard_highlight_background, getBuffer(), getSize());
						}
						else {
							memcpy(getBuffer(), leaderboard_highlight_background, getSize());
						}
						draw_buffer();

						draw_xpm(xpm_cursor_adr, img_mouse, coordinate_x_mouse, coordinate_y_mouse);
						draw_buffer();
						break;

					case EXIT_HIGHLIGHT:
						if (exit_highlight_background == NULL) {
							draw_xpm(exit_highlight_adr, img_exit_highlight, 0, 0);
							exit_highlight_background = malloc(getSize());
							memcpy(exit_highlight_background, getBuffer(), getSize());
						}
						else {
							memcpy(getBuffer(), exit_highlight_background, getSize());
						}
						draw_buffer();

						draw_xpm(xpm_cursor_adr, img_mouse, coordinate_x_mouse, coordinate_y_mouse);
						draw_buffer();
						break;

					case LEADERBOARD:
						if(!writed_leaderboard){
							if (leaderboard_background == NULL) {
								draw_xpm(xpm_leaderboard_background_adr, img_leaderboard_background, 0, 0);
								leaderboard_background = malloc(getSize());
								memcpy(leaderboard_background, getBuffer(), getSize());
							}
							else {
								memcpy(getBuffer(), leaderboard_background, getSize());
							}
							draw_buffer();
							int n=0;
							for (size_t i=0; i<10;i++){
								text_to_screen(scores[i].name, 40, 50, 360+n);
								n+=65;
							}

							int n1=0;
							char str[100];
							for (size_t i=0; i<10;i++){
								sprintf(str, "%ld", scores[i].score);
								text_to_screen(str, 50, 360, 360+n1);
								n1+=65;
							}

							int n2=0;
							for (size_t i=0; i<10;i++){
								text_to_screen(scores[i].date, 100, 540, 360+n2);
								n2+=65;
							}

							writed_leaderboard=true;
						}

						break;

					case PLAY_BEFORE_KEY:
						if (background_game == NULL) {
							draw_xpm(xpm_game_background_adr, img_game_background, 0, 0);
							background_game = malloc(getSize());
							memcpy(background_game, getBuffer(), getSize());
						}
						else {
							memcpy(getBuffer(), background_game, getSize());
						}
						draw_buffer();

						snake_init();

						printSnake(snake_head_up_adr, img_snake_head_up, snake_head_down_adr, img_snake_head_down, snake_head_left_adr, img_snake_head_left, snake_head_right_adr, img_snake_head_right, snake_body_h_adr, img_snake_body_h, snake_body_v_adr, img_snake_body_v, snake_tail_right_adr, img_snake_tail_right, snake_tail_left_adr, img_snake_tail_left, snake_tail_down_adr, img_snake_tail_down, snake_tail_up_adr, img_snake_tail_up);

						state=DODGE;

						break;
					case DODGE:
						if (dodge == NULL) {
							draw_xpm(dodge_adr, img_dodge, 0, 0);
							dodge = malloc(getSize());
							memcpy(dodge, getBuffer(), getSize());
						}
						else {
							memcpy(getBuffer(), dodge, getSize());
						}
						draw_buffer();
						break;

					case PLAY:
						if (background_game == NULL) {
							draw_xpm(xpm_game_background_adr, img_game_background, 0, 0);
							background_game = malloc(getSize());
							memcpy(background_game, getBuffer(), getSize());
						}
						else {
							memcpy(getBuffer(), background_game, getSize());
						}
					
						if ((spawn_counter % time_until_spawn) == 0) {
							spawnFruit(background_game, xpm_game_background_adr, apple_adr, img_game_background, img_apple);
							spawn_counter = 0;
						}

						printSnake(snake_head_up_adr, img_snake_head_up, snake_head_down_adr, img_snake_head_down, snake_head_left_adr, img_snake_head_left, snake_head_right_adr, img_snake_head_right, snake_body_h_adr, img_snake_body_h, snake_body_v_adr, img_snake_body_v, snake_tail_right_adr, img_snake_tail_right, snake_tail_left_adr, img_snake_tail_left, snake_tail_down_adr, img_snake_tail_down, snake_tail_up_adr, img_snake_tail_up);

						if (movement(snake_dir) == 1) {
							state = GAME_OVER;
						}

						if(checkColision())
							state = GAME_OVER;

						if (increase_snake%60!=0){
							UpdateTail();
						}

						if ((spawn_counter % 18000) == 0) {
							time_until_spawn--;
						}


						if ((spawn_counter % 2) == 0) {
							score++;
						}

						increase_snake++;
						spawn_counter++;
						break;
					case GAME_OVER:
						draw_xpm(game_over_adr, img_game_over, 0, 0);
						draw_buffer();

						get_date(date);
						strcpy(ng.date,date);

						ng.score=score;

						char score_string[500];
						sprintf(score_string, "%d", score);

						text_to_screen(score_string, 100, 720, 640);

						break;
					
					case USERNAME:
						draw_xpm(username_adr, img_username, 0, 0);
						draw_buffer();
						text_to_screen(name, 100, 315, 480);
						strcpy(ng.name,name);
					
						if (key_pressed==ENTER_KEY) {
							insert_on_leaderboard(scores,10,ng);
							state=MAIN_MENU;
						}
						
						break;
					default:
						break;
					}
				}
			}
		}
	}
	write_file(scores,10);

	// Unsubscribe and exit

	if (keyboard_unsubscribe_int() != 0)
		return 1;

	if (mouse_unsubscribe_int() != 0)
		return 1;

	if (timer_unsubscribe_int() != 0)
		return 1;

	if (vg_exit() != 0)
		return 1;

	return 0;
}
