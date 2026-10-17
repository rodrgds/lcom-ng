#include <lcom/lcf.h>
#include "mouse.h"
#include "video_card.h"
#include "kbd.h"
#include "rtc.h"
#include "sprites.h"
#include "menu.h"
#include "game_loop.h"
#include "proj_main_loop.h"
#include "assets/pixmaps.h"

int main(int argc, char **argv) {
    //lcf_trace_calls("/home/lcom/labs/g6/proj/src/trace.txt");

    //lcf_log_output("/home/lcom/labs/g6/proj/src/output.txt");

    if (lcf_start(argc, argv))
        return 1;

    lcf_cleanup();

    return 0;
}

/// Variáveis do main loop
static Game_State gs = MENU;
static mouse_sprite* mouse_cursor;

extern uint8_t race_id;

/**
 * -1 if failed subscribing timer
 * -2 if failed subscribing keyboard
 * -3 if failed subscribing mouse
 * -4 if failed subscribing rtc
 * -5 if failed to set the vbe mode
*/
int (initialize) (uint16_t vbe_mode, uint32_t frequency, int* timer_irq, int* keyboard_irq, int* mouse_irq, int* rtc_irq){
	/// Set all used variable to the default
	uint8_t bit_no;
	if (timer_subscribe_int(&bit_no) != 0){
		printf("Error subscribing the timer.\n");
		return -1;
	}
	*timer_irq = BIT(bit_no);
	while (timer_set_frequency(0, frequency) != 0);

	if (keyboard_subscribe_int(&bit_no) != 0){
		printf("Error subscribing the keyboard.\n");
		return -2;
	}
	*keyboard_irq = BIT(bit_no);

	write_command_mouse(MOUSE_ENABLE_DATA_REPORTING);
	if (mouse_subscribe_int(&bit_no) != 0){
		printf("Error subscribing the mouse.\n");
		return -3;
	}
	*mouse_irq = BIT(bit_no);

	if (rtc_subscribe_int(&bit_no) != 0){
		printf("Error subscribing the rtc.\n");
		return -4;
	}
	*rtc_irq = BIT(bit_no);
	set_alarm(120);
	active_alarm_int();

	if (vg_start(vbe_mode) != 0){
		printf("Error setting the vbe mode.\n");
		return -5;
	}

	mouse_sprite *cursor = malloc(sizeof(mouse_sprite));
	cursor->x = 200;
	cursor->y = 22;
	cursor->speed = 0;
	cursor->index = 0;
	if (xpm_load(cursor3, XPM_8_8_8_8, &cursor->info[0]) == NULL) return -4;
	mouse_cursor = cursor;

	if (xpm_load(cursor_pointer, XPM_8_8_8_8, &cursor->info[1]) == NULL) return -4;
	mouse_cursor = cursor;

	return 0;
}

/**
 * Reset every thing that was setup
 * If ret = 0, then everything must be reseted
 * If ret = -1 only timer
 * 	...
*/
void (clean) (int ret){
	if (ret < 0 || ret == 0){
		while (timer_unsubscribe_int() != 0);
	}
	if (ret < -1 || ret == 0){
		while (keyboard_unsubscribe_int() != 0);
	}
	if (ret < -2 || ret == 0){
		write_command_mouse(MOUSE_DISABLE_DATA_REPORTING);
		while (mouse_unsubscribe_int() != 0);
	}
	if (ret < -3 || ret == 0){
		while (rtc_unsubscribe_int() != 0);
		disable_alarm_int();
	}
	if (ret < -4 || ret == 0){
		while (vg_exit() != 0);
	}

	if (mouse_cursor != NULL){
		free(mouse_cursor);
	}
}

int (proj_main_loop) (int argc, char** argv){
	/// Set up
	uint16_t mode = 0x14C;
	uint32_t frequency = FPS;
	int timer_irq, keyboard_irq, mouse_irq,rtc_irq;
	int ret = initialize(mode, frequency, &timer_irq, &keyboard_irq, &mouse_irq, &rtc_irq);
	if (ret != 0) {
		clean(ret);
		return -1;
	}

	/// Main loop controlling the state of the game
	bool close = false;
	while (true){
		switch (gs){
			case MENU:
				gs = menu_controller(timer_irq, mouse_irq, keyboard_irq, rtc_irq, mouse_cursor);
				break;

			case SINGLE_PLAYER:
				gs = game_controller(timer_irq, mouse_irq, keyboard_irq, rtc_irq, mouse_cursor, race_id);
				break;

			case RECORDS:
				gs = MENU;
				break;

			case CLOSE:
				close = true;
				break;

			default:
				break;
		}
		if (close) break;
	}
	/// Clean up
	clean(0);
	return 0;
}
