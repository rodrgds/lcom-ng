#include "state.h"
#include "../controller/menu_controller.h"
#include "../controller/name_controller.h"
#include "../controller/game_controller.h"
#include "../controller/game_over_controller.h"
#include "../view/name_viewer.h"
#include "../view/menu_viewer.h"
#include "../view/game_viewer.h"
#include "../view/game_over_viewer.h"
#include "../model/menu/menu.h"
#include "../model/name/name_input.h"
#include "../model/game/game.h"
#include "../model/game_over/game_over.h"

State state;

int (init_state)() {

	state = MENU;
	if (load_state())
		return 1;

	return 0;
}

int (leave_state)(){

	switch (state) {
		case MENU:{
			destroy_menu();
			break;
		}
		case WRITE_NAME:{
			destroy_name();
			break;
		}
		case GAME:
			destroy_game();
			break;
		case GAME_OVER:
			destroy_game_over();
			break;
		case EXIT:
			break;
		default:
			return 1;
	}

	return 0;
}

int (load_state)(){

	switch (state) {
		case MENU:{
			create_menu();
			menu_screenshot();
			break;
		}
		case WRITE_NAME:{
			create_name();
			name_screenshot();
			break;
		}
		case GAME:{
			create_game(nameBackup, "BOT");
			game_screenshot();
			break;
		}
		case GAME_OVER:{
			create_game_over();
			game_over_screenshot();
			break;
		}
		case EXIT:
			break;
		default:
			return 1;
	}
	return 0;
}

int (set_state)(State new_state) {

	if (leave_state())
		return 1;
	state = new_state;
	if (load_state())
		return 1;

	return 0;
}

int (state_kbd_handler)(uint8_t scan_code[2], int size) {

  switch (state) {
    case MENU:
			menu_kbd(scan_code, size);
			break;
    case WRITE_NAME:
			player_name_kbd(scan_code, size);
			break;
    case GAME:
			game_kbd(scan_code, size);
			break;
		case GAME_OVER:
			game_over_kbd(scan_code, size);
			break;
    case EXIT:
			break;
    default:
			return 1;
  }
	return 0;
}

int (state_mouse_handler)(CursorInput input){

	switch(state){
		case MENU:
			menu_cursor(&input);
			break;
		case WRITE_NAME:
			break;
		case GAME:
			game_cursor(&input);
			break;
		case GAME_OVER:
			game_over_cursor(&input);
			break;
		case EXIT:
			break;
		default:
			return 1;
	}

	return 0;
}

int (state_timer_handler)(){

	switch(state){
		case MENU:
			menu_refresh();
			break;
		case WRITE_NAME:
			name_refresh();
			break;
		case GAME:
			game_refresh();
			game_timer();
			break;
		case GAME_OVER:
			game_over_refresh();
			break;
		case EXIT:
			break;
		default:
			return 1;
	}

	return 0;
}
