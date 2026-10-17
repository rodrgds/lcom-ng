#include "keyboard.h"
#include "xpm.h"
#include "pixmap.h"
#include "print.h"
#include "variables.h"
#include "game.h"
#include "video_gr.h"
#include "macros.h"
#include "numbers_pixmap.h"

void print_info() {
	print_number(total_score, TOTAL_SCORE_X, TOTAL_SCORE_Y);
	print_number(time_counter, TIME_COUNT_X, TIME_COUNT_Y);
	return;
}

void print_score() {

	title_creator(FINAL_SCORE_X, FINAL_SCORE_Y, Score_xpm);
	print_number(total_score, 570, 300);

	copy_to_video_mem();

	sleep(3);

	return;
}

void print_message(char *pixmap[]) {
	int counter = 0;

	clear_buffer();
	char **title = pixmap;

	while (counter != BLINKS) {
		title_creator(WIN_LOSE_X, WIN_LOSE_Y, title);
		copy_to_video_mem();
		sleep(1);
		clear_buffer();
		copy_to_video_mem();
		counter++;
		sleep(1);
	}

	return;
}

void print_number(unsigned number, int x, int y) {

	buffer_destructor(150, 25, x - 100, y); // reserves space to the number
	while (number != 0) {
		int digit = number % 10;

		switch (digit) {
		case 0:
			title_creator(x, y, Zero_xpm);
			break;
		case 1:
			title_creator(x, y, One_xpm);
			break;
		case 2:
			title_creator(x, y, Two_xpm);
			break;
		case 3:
			title_creator(x, y, Three_xpm);
			break;
		case 4:
			title_creator(x, y, Four_xpm);
			break;
		case 5:
			title_creator(x, y, Five_xpm);
			break;
		case 6:
			title_creator(x, y, Six_xpm);
			break;
		case 7:
			title_creator(x, y, Seven_xpm);
			break;
		case 8:
			title_creator(x, y, Eight_xpm);
			break;
		case 9:
			title_creator(x, y, Nine_xpm);
			break;
		}

		number -= digit;
		number /= 10;
		x -= 40;
	}

	copy_to_video_mem();

	return;
}

void print_instructions() {
	memset(buffer, 0, getHRes() * getVRes() * getBitsPerPixel() / 8);
	while (kbd_code != SPC_COD) {
		kbd_code = kbd_scan();
		title_creator(CONTROLS_X, CONTROLS_Y, Controls_xpm);
		title_creator(SPACEBAR_X, SPACEBAR_Y, Spacebar_xpm);
		copy_to_video_mem();
	}

	return;
}

void print_menu() {
	static int arrow_y = 220;

	clear_buffer();

	title_creator(230, arrow_y, Arrow_xpm);

	title_creator(NEW_GAME_X, NEW_GAME_Y, NewGame_xpm);

	title_creator(INSTRUCTIONS_X, INSTRUCTIONS_Y, Instructions_xpm);

	title_creator(EXIT_X, EXIT_Y, Exit_xpm);

	copy_to_video_mem();

	while (kbd_code != ESC_COD) {
		kbd_code = kbd_scan();
		switch (menu) {
		case NEW_GAME:

			if (kbd_code == S_PRESS) {
				menu = INSTRUCTIONS;
				arrow_y += 168;
				print_menu();
			} else if (kbd_code == ENTER_COD) {
				total_score = 0;
				kbd_mouse_menu = MOUSE_MOUSE;
				print_menu_kbd_or_mouse();
				print_menu();
			}
			break;
		case INSTRUCTIONS:

			if (kbd_code == W_PRESS) {
				menu = NEW_GAME;
				arrow_y -= 168;
				print_menu();
			} else if (kbd_code == ENTER_COD) {
				print_instructions();
				print_menu();
				menu = NEW_GAME;
			}
		}
	}
}

void print_menu_kbd_or_mouse() {
	static int arrow_y = 250;

	clear_buffer();

	title_creator(50, arrow_y, Arrow_xpm);

	title_creator(GAME_MODES_X, GAME_MODES_Y, game_modes_xpm);

	title_creator(EXIT_X, EXIT_Y, Exit_xpm);

	copy_to_video_mem();

	while (kbd_code != ESC_COD && kbd_mouse_menu != PLAYED) {
		kbd_code = kbd_scan();
		switch (kbd_mouse_menu) {
		case MOUSE_MOUSE:
			if (kbd_code == S_PRESS) {
				kbd_mouse_menu = KEYBOARD_KEYBOARD;
				arrow_y += 85;
				print_menu_kbd_or_mouse();
			} else if (kbd_code == ENTER_COD) {
				init();
				kbd_mouse_menu = PLAYED;
				arrow_y = 250;
			}
			break;

		case KEYBOARD_KEYBOARD:
			if (kbd_code == S_PRESS) {
				kbd_mouse_menu = MOUSE_KEYBOARD;
				arrow_y += 85;
				print_menu_kbd_or_mouse();
			}
			if (kbd_code == W_PRESS) {
				kbd_mouse_menu = MOUSE_MOUSE;
				arrow_y -= 85;
				print_menu_kbd_or_mouse();
			} else if (kbd_code == ENTER_COD) {
				init();
				kbd_mouse_menu = PLAYED;
				arrow_y = 250;
			}
			break;

		case MOUSE_KEYBOARD:
			if (kbd_code == S_PRESS) {
				kbd_mouse_menu = KEYBOARD_MOUSE;
				arrow_y += 85;
				print_menu_kbd_or_mouse();
			}
			if (kbd_code == W_PRESS) {
				kbd_mouse_menu = KEYBOARD_KEYBOARD;
				arrow_y -= 85;
				print_menu_kbd_or_mouse();
			} else if (kbd_code == ENTER_COD) {
				init();
				kbd_mouse_menu = PLAYED;
				arrow_y = 250;
			}
			break;

		case KEYBOARD_MOUSE:
			if (kbd_code == W_PRESS) {
				kbd_mouse_menu = MOUSE_KEYBOARD;
				arrow_y -= 85;
				print_menu_kbd_or_mouse();
			} else if (kbd_code == ENTER_COD) {
				init();
				kbd_mouse_menu = PLAYED;
				arrow_y = 250;
			}
			break;
		}
	}
}
