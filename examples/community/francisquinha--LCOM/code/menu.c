/** @file menu.c
 * @brief file containing definitions of the functions that deal with the game menu
 */
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>

#include "menu.h"
#include "menu_static.h"
#include "i8042.h"

#define DEBUG_MALLOC 0

Menu *create_menu() {
	Menu *menu = (Menu *)malloc(sizeof(Menu));
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for menu\n", sizeof(Menu));
	if (menu == NULL) {
		printf("create_menu: error allocating memory for menu!\n");
		return NULL;
	}
	reset_menu(menu);
	return menu;
}

void reset_menu(Menu *menu) {
	menu->state = MENU_INIT;
	menu->number_players = 0;
	unsigned i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 5; j++) {
			menu->player_names[i][j] = 0;
		}
	}
	menu->current_name = 0;
	menu->current_letter = 0;
	menu->names_done = 0;
}


static void manage_name_letter(Menu *menu) {
	menu->current_letter++;
	if (menu->current_letter == 5) {
		menu->current_name++;
		menu->current_letter = 0;
		if (menu->current_name == menu->number_players)
			menu->names_done = 1;
	}
}

static void get_keyboard_digit(Menu *menu, long scan_code) {
	switch (scan_code) {
	case N1_MAKE_CODE:
		menu->number_players = 1;
		break;
	case N2_MAKE_CODE:
		menu->number_players = 2;
		break;
	case N3_MAKE_CODE:
		menu->number_players = 3;
		break;
	case N4_MAKE_CODE:
		menu->number_players = 4;
		break;
	case N5_MAKE_CODE:
		menu->number_players = 5;
		break;
	case N6_MAKE_CODE:
		menu->number_players = 6;
		break;
	case N7_MAKE_CODE:
		menu->number_players = 7;
		break;
	case N8_MAKE_CODE:
		menu->number_players = 8;
		break;
	case N9_MAKE_CODE:
		menu->number_players = 9;
		break;
	}
}

static void get_keyboard_menu(Menu *menu, long scan_code) {
	switch (scan_code) {
	case ENTER_MAKE_CODE:
		menu->state = MENU_DONE;
		break;
	case SPACE_MAKE_CODE:
		manage_name_letter(menu);
		break;
	case A_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 1;
		manage_name_letter(menu);
		break;
	case B_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 2;
		manage_name_letter(menu);
		break;
	case C_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 3;
		manage_name_letter(menu);
		break;
	case D_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 4;
		manage_name_letter(menu);
		break;
	case E_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 5;
		manage_name_letter(menu);
		break;
	case F_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 6;
		manage_name_letter(menu);
		break;
	case G_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 7;
		manage_name_letter(menu);
		break;
	case H_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 8;
		manage_name_letter(menu);
		break;
	case I_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 9;
		manage_name_letter(menu);
		break;
	case J_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 10;
		manage_name_letter(menu);
		break;
	case K_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 11;
		manage_name_letter(menu);
		break;
	case L_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 12;
		manage_name_letter(menu);
		break;
	case M_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 13;
		manage_name_letter(menu);
		break;
	case N_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 14;
		manage_name_letter(menu);
		break;
	case O_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 15;
		manage_name_letter(menu);
		break;
	case P_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 16;
		manage_name_letter(menu);
		break;
	case Q_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 17;
		manage_name_letter(menu);
		break;
	case R_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 18;
		manage_name_letter(menu);
		break;
	case S_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 19;
		manage_name_letter(menu);
		break;
	case T_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 20;
		manage_name_letter(menu);
		break;
	case U_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 21;
		manage_name_letter(menu);
		break;
	case V_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 22;
		manage_name_letter(menu);
		break;
	case W_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 23;
		manage_name_letter(menu);
		break;
	case X_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 24;
		manage_name_letter(menu);
		break;
	case Y_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 25;
		manage_name_letter(menu);
		break;
	case Z_MAKE_CODE:
		menu->player_names[menu->current_name][menu->current_letter] = 26;
		manage_name_letter(menu);
		break;
	}
}

static void get_mouse_menu_init(Menu *menu, MenuButton *button, unsigned short x, unsigned short y) {
	if (456 <= x && x <= 568 && 474 <= y && y <= 496)
		*button = NEW_BUTTON;
	else if (449 <= x && x <= 575 && 568 <= y && y <= 590)
		*button = LOAD_BUTTON;
	else *button = NONE_BUTTON;
}

static void get_mouse_menu_new(Menu *menu, MenuButton *button, unsigned short x, unsigned short y) {
	if (421 <= x && x <= 603 && 190 <= y && y <= 212)
		*button = SINGLE_BUTTON;
	else if (456 <= x && x <= 568 && 312 <= y && y <= 334)
		*button = HOT_BUTTON;
	else if (428 <= x && x <= 596 && 434 <= y && y <= 456)
		*button = SUDDEN_BUTTON;
	else if (442 <= x && x <= 582 && 556 <= y && y <= 578)
		*button = DUEL_BUTTON;
	else *button = NONE_BUTTON;
}

static void get_mouse_menu_load_save(Menu *menu, MenuButton *button, unsigned short x, unsigned short y) {
	if (470 <= x && x <= 554) {
		if (190 <= y && y <= 212)
			*button = SLOT_1_BUTTON;
		else if (312 <= y && y <= 334)
			*button = SLOT_2_BUTTON;
		else if (434 <= y && y <= 456)
			*button = SLOT_3_BUTTON;
		else if (556 <= y && y <= 578)
			*button = SLOT_4_BUTTON;
		else *button = NONE_BUTTON;
	}
	else *button = NONE_BUTTON;
}

static void get_mouse_menu(Menu *menu, MenuButton *button, unsigned short x, unsigned short y) {
	switch (menu->state) {
	case MENU_INIT:
		get_mouse_menu_init(menu, button, x, y);
		break;
	case MENU_NEW:
		get_mouse_menu_new(menu, button, x, y);
		break;
	case MENU_LOAD:
		get_mouse_menu_load_save(menu, button, x, y);
		break;
	}
}

static void select_mouse_menu(Menu *menu, MenuButton button) {
	switch(button) {
	case NEW_BUTTON:
		menu->state = MENU_NEW;
		break;
	case LOAD_BUTTON:
		menu->state = MENU_LOAD;
		break;
	case SINGLE_BUTTON:
		menu->state = MENU_SINGLE;
		menu->number_players = 1;
		menu->mode = SINGLE_MODE;
		break;
	case HOT_BUTTON:
		menu->state = MENU_HOT;
		menu->mode = SINGLE_MODE;	// change this when hot mode is implemented
		break;
	case SUDDEN_BUTTON:
		menu->state = MENU_SUDDEN;
		menu->number_players = 1;
		menu->mode = SINGLE_MODE;	// change this when sudden mode is implemented
		break;
	case DUEL_BUTTON:
//		menu-state->MENU_DUEL;
//		menu->mode->DUEL_MODE;
		break;
	case SLOT_1_BUTTON:
		menu->state = MENU_DONE;
		menu->mode = SLOT_1_MODE;
		break;
	case SLOT_2_BUTTON:
		menu->state = MENU_DONE;
		menu->mode = SLOT_2_MODE;
		break;
	case SLOT_3_BUTTON:
		menu->state = MENU_DONE;
		menu->mode = SLOT_3_MODE;
		break;
	case SLOT_4_BUTTON:
		menu->state = MENU_DONE;
		menu->mode = SLOT_4_MODE;
		break;
	}
}

void draw_menu(Menu *menu, char *base, char *origin, char ***numbers, char ***letters) {
	switch (menu->state) {
	case MENU_INIT:
		vg_draw_menu_init(base, origin, letters);
		break;
	case MENU_NEW:
		vg_draw_menu_new(base, origin, letters);
		break;
	case MENU_LOAD:
		vg_draw_menu_load_save(base, origin, numbers, letters);
		break;
	case MENU_SINGLE:
		vg_draw_menu_single(base, origin, menu->player_names[0], letters);
		break;
	case MENU_HOT:
		vg_draw_menu_hot(base, origin, menu->number_players, menu->player_names,  numbers, letters);
		break;
	case MENU_SUDDEN:
		vg_draw_menu_sudden(base, origin, menu->player_names[0], letters);
		break;
	}
}

MenuState mouse_keyboard_menu_handler(int int_irq, int mse_irq, int kbd_irq, Menu *menu,
		char *menu_buffer, char *backg_buffer, char *mouse_buffer, char *video_mem, char ***numbers, char ***letters,
		unsigned h_res, unsigned v_res, Mouse *mouse, Keyboard *keyboard) {
	if (int_irq & mse_irq) {
		if (mouse->info) {
			fix_mouse_position(mouse, h_res, v_res);
			if (mouse->left && (menu->state == MENU_INIT ||
					menu->state == MENU_NEW || menu->state == MENU_LOAD)) {
				MenuButton button;
				get_mouse_menu(menu, &button, mouse->x, mouse->y);
				select_mouse_menu(menu, button);
				draw_menu(menu, menu_buffer, backg_buffer, numbers, letters);
			}
			vg_draw_mouse(mouse_buffer, menu_buffer, mouse);
			copy_buffer(video_mem, mouse_buffer);
			mouse->info = 0;
		}
	}
	if ((int_irq & kbd_irq) && (menu->state == MENU_SINGLE ||
			menu->state == MENU_HOT || menu->state == MENU_SUDDEN)) {
		if (menu->number_players == 0) {
			get_keyboard_digit(menu, keyboard->scan_code);
			draw_menu(menu, menu_buffer, backg_buffer, numbers, letters);
			vg_draw_mouse(mouse_buffer, menu_buffer, mouse);
			copy_buffer(video_mem, mouse_buffer);
		}
		else if (!menu->names_done) {
			get_keyboard_menu(menu, keyboard->scan_code);
			draw_menu(menu, menu_buffer, backg_buffer, numbers, letters);
			vg_draw_mouse(mouse_buffer, menu_buffer, mouse);
			copy_buffer(video_mem, mouse_buffer);
		}
		else if (keyboard->scan_code == ENTER_MAKE_CODE)
			menu->state = MENU_DONE;
	}
	return menu->state;
}
