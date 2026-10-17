/** @file game.c
 * @brief file containing the definitions of the main game functions
 */

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>

#include "game.h"
#include "dispatcher.h"
#include "game_static.h"
#include "logic.h"
#include "i8042.h"
#include "i8254.h"
#include "read_xpm.h"
#include "images.h"

#define DEBUG_MALLOC 0

static void reset_gamestate(GameState *gamestate) {
	gamestate->dealt = 0;
	unsigned short i;
	for (i = 0; i < 21; i++)
		gamestate->display_cards[i] = 81;
	gamestate->display = 0;
	for (i = 0; i < 3; i++)
		gamestate->remove_cards[i] = 21;
	gamestate->extra_card = 21;
	gamestate->remove_card = 0;
	gamestate->remove = 0;
	gamestate->frame = 0;
	for (i = 0; i < 3; i++)
		gamestate->add_cards[i] = 21;
	gamestate->add = 0;
	gamestate->hint = 0;
	gamestate->state = GAME_MENU;
}

static GameState *create_gamestate() {
	GameState *gamestate = (GameState *)malloc(sizeof(GameState));
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for gamestate\n", sizeof(GameState));
	if (gamestate == NULL) {
		printf("create_state: error allocating memory for state!\n");
		return NULL;
	}
	reset_gamestate(gamestate);
	return gamestate;
}

static Game *create_game() {
	Game *game = (Game *)malloc(sizeof(Game));
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for game\n", sizeof(Game));
	if (game == NULL) {
		printf("create_game: error allocating memory for game!\n");
		return NULL;
	}
	if ((game->menu = create_menu()) == NULL) {
		free(game);
		if (DEBUG_MALLOC) printf("Freed game\n");
		printf("create_game: error creating menu!\n");
		return NULL;
	}
	if ((game->gamestate = create_gamestate()) == NULL) {
		free(game->menu);
		if (DEBUG_MALLOC) printf("Freed menu\n");
		free(game);
		if (DEBUG_MALLOC) printf("Freed game\n");
		printf("create_game: error creating state!\n");
		return NULL;
	}
	if ((game->graphics = create_graphics()) == NULL) {
	    free(game->gamestate);
		if (DEBUG_MALLOC) printf("Freed gamestate\n");
		free(game->menu);
		if (DEBUG_MALLOC) printf("Freed menu\n");
	    free(game);
		if (DEBUG_MALLOC) printf("Freed game\n");
		printf("create_game: error creating graphics!\n");
		return NULL;
	}
	if ((game->keyboard = create_keyboard()) == NULL) {
	    destroy_graphics(game->graphics);
	    free(game->gamestate);
		if (DEBUG_MALLOC) printf("Freed gamestate\n");
		free(game->menu);
		if (DEBUG_MALLOC) printf("Freed menu\n");
	    free(game);
		if (DEBUG_MALLOC) printf("Freed game\n");
		printf("create_game: error creating keyboard!\n");
		return NULL;
	}
	if ((game->mouse = create_mouse()) == NULL) {
		free(game->keyboard);
		if (DEBUG_MALLOC) printf("Freed keyboard\n");
	    destroy_graphics(game->graphics);
	    free(game->gamestate);
		if (DEBUG_MALLOC) printf("Freed gamestate\n");
		free(game->menu);
		if (DEBUG_MALLOC) printf("Freed menu\n");
	    free(game);
		if (DEBUG_MALLOC) printf("Freed game\n");
		printf("create_game: error creating mouse!\n");
		return NULL;
	}
	if ((game->timer = create_timer()) == NULL) {
		free(game->mouse);
		if (DEBUG_MALLOC) printf("Freed mouse\n");
		free(game->keyboard);
		if (DEBUG_MALLOC) printf("Freed keyboard\n");
	    destroy_graphics(game->graphics);
	    free(game->gamestate);
		if (DEBUG_MALLOC) printf("Freed gamestate\n");
		free(game->menu);
		if (DEBUG_MALLOC) printf("Freed menu\n");
	    free(game);
		if (DEBUG_MALLOC) printf("Freed game\n");
		printf("create_game: error creating timer!\n");
		return NULL;
	}
	if ((game->rtc = create_rtc()) == NULL) {
		free(game->timer);
		if (DEBUG_MALLOC) printf("Freed timer\n");
		free(game->mouse);
		if (DEBUG_MALLOC) printf("Freed mouse\n");
		free(game->keyboard);
		if (DEBUG_MALLOC) printf("Freed keyboard\n");
	    destroy_graphics(game->graphics);
	    free(game->gamestate);
		if (DEBUG_MALLOC) printf("Freed gamestate\n");
		free(game->menu);
		if (DEBUG_MALLOC) printf("Freed menu\n");
	    free(game);
		if (DEBUG_MALLOC) printf("Freed game\n");
		printf("create_game: error creating timer!\n");
		return NULL;
	}
	unsigned short i;
	for (i = 0; i < 10; i++) {
		if ((game->highscores[i] = create_highscore()) == NULL) {
			destroy_rtc(game->rtc);
			free(game->timer);
			if (DEBUG_MALLOC) printf("Freed timer\n");
			free(game->mouse);
			if (DEBUG_MALLOC) printf("Freed mouse\n");
			free(game->keyboard);
			if (DEBUG_MALLOC) printf("Freed keyboard\n");
		    destroy_graphics(game->graphics);
		    free(game->gamestate);
			if (DEBUG_MALLOC) printf("Freed gamestate\n");
			free(game->menu);
			if (DEBUG_MALLOC) printf("Freed menu\n");
		    free(game);
			if (DEBUG_MALLOC) printf("Freed game\n");
			printf("create_game: error creating highscore!\n");
			return NULL;
		}
	}
	return game;
}

static void destroy_game(Game *game) {
    short i;
    for (i = 9; i >= 0; i--)
    	destroy_highscore(game->highscores[i]);
	destroy_rtc(game->rtc);
	free(game->timer);
	if (DEBUG_MALLOC) printf("Freed timer\n");
	free(game->mouse);
	if (DEBUG_MALLOC) printf("Freed mouse\n");
	free(game->keyboard);
	if (DEBUG_MALLOC) printf("Freed keyboard\n");
    destroy_graphics(game->graphics);
    free(game->gamestate);
	if (DEBUG_MALLOC) printf("Freed gamestate\n");
	free(game->menu);
	if (DEBUG_MALLOC) printf("Freed menu\n");
    free(game);
	if (DEBUG_MALLOC) printf("Freed game\n");
}

static void get_position(Game *game, unsigned short index, unsigned short *x, unsigned short *y) {
	*x = game->graphics->gap + (index / 3) * (game->graphics->width + game->graphics->gap);
	*y = game->graphics->gap + (index % 3) * (game->graphics->height + game->graphics->gap);
}

static void get_keyboard_button(Button *button, long scan_code) {
	if (scan_code == SPACE_MAKE_CODE || scan_code == S_MAKE_CODE)
		*button = SET_BUTTON;
	else if (scan_code == ENTER_MAKE_CODE || scan_code == C_MAKE_CODE)
		*button = NOSETS_BUTTON;
	else if (scan_code == RSHFT_MAKE_CODE || scan_code == LSHFT_MAKE_CODE
			|| scan_code == H_MAKE_CODE)
		*button = HINT_BUTTON;
	else if (scan_code == CTRL_MAKE_CODE
			|| scan_code == P_MAKE_CODE)
		*button = PAUSE_BUTTON;
	else if (scan_code == ALT_MAKE_CODE
			|| scan_code == W_MAKE_CODE)
		*button = SAVE_BUTTON;
	else if (scan_code == ESC_BREAK_CODE
			|| scan_code == E_MAKE_CODE)
		*button = EXIT_BUTTON;
	else *button = NO_BUTTON;
}

static void get_mouse_button(Game *game, Button *button, unsigned short x, unsigned short y) {
	if (3 * game->graphics->height + 4 * game->graphics->gap + game->graphics->button_gap <= y
			&& y <= 3 * game->graphics->height + 4 * game->graphics->gap
			+ game->graphics->button_gap + game->graphics->button_height) {
		if (game->graphics->gap + game->graphics->button_gap <= x
				&& x <= game->graphics->button_width + game->graphics->gap
				+ game->graphics->button_gap)
			*button = SET_BUTTON;
		else if (game->graphics->button_width + 2 * game->graphics->gap
				+ 3 * game->graphics->button_gap <= x
				&& x <= 2 * game->graphics->button_width + 2 * game->graphics->gap
				+ 3 * game->graphics->button_gap)
			*button = NOSETS_BUTTON;
		else if (2 * game->graphics->button_width + 3 * game->graphics->gap
				+ 5 * game->graphics->button_gap <= x
				&& x <= 3 * game->graphics->button_width + 3 * game->graphics->gap
				+ 5 * game->graphics->button_gap)
			*button = HINT_BUTTON;
		else if (4 * game->graphics->button_width + 5 * game->graphics->gap
				+ 9 * game->graphics->button_gap <= x
				&& x <= 5 * game->graphics->button_width + 5 * game->graphics->gap
				+ 9 * game->graphics->button_gap)
			*button = PAUSE_BUTTON;
		else if (5 * game->graphics->button_width + 6 * game->graphics->gap
				+ 11 * game->graphics->button_gap <= x
				&& x <= 6 * game->graphics->button_width + 6 * game->graphics->gap
				+ 11 * game->graphics->button_gap)
			*button = SAVE_BUTTON;
		else if (6 * game->graphics->button_width + 7 * game->graphics->gap
				+ 13 * game->graphics->button_gap <= x
				&& x <= 7 * game->graphics->button_width + 7 * game->graphics->gap
				+ 13 * game->graphics->button_gap)
			*button = EXIT_BUTTON;
	}
	else *button = NO_BUTTON;
}

static void get_mouse_card_index(Game *game, unsigned short *index, unsigned short x, unsigned short y) {
	if (game->graphics->gap <= y
			&& y <= game->graphics->height + game->graphics->gap)
		*index = 0;
	else if (game->graphics->height + 2 * game->graphics->gap <= y
			&& y <= 2 * game->graphics->height + 2 * game->graphics->gap)
		*index = 1;
	else if (2 * game->graphics->height + 3 * game->graphics->gap <= y
			&& y <= 3 * game->graphics->height + 3 * game->graphics->gap)
		*index = 2;
	else {
		*index = 81;
		return;
	}
	if (game->graphics->gap <= x && x <= game->graphics->width + game->graphics->gap)
		*index += 0;
	else if (game->graphics->width + 2 * game->graphics->gap <= x
			&& x <= 2 * game->graphics->width + 2 * game->graphics->gap)
		*index += 3;
	else if (2 * game->graphics->width + 3 * game->graphics->gap <= x
			&& x <= 3 * game->graphics->width + 3 * game->graphics->gap)
		*index += 6;
	else if (3 * game->graphics->width + 4 * game->graphics->gap <= x
			&& x <= 4 * game->graphics->width + 4 * game->graphics->gap)
		*index += 9;
	else if (4 * game->graphics->width + 5 * game->graphics->gap <= x
			&& x <= 5 * game->graphics->width + 5 * game->graphics->gap)
		*index += 12;
	else if (5 * game->graphics->width + 6 * game->graphics->gap <= x
			&& x <= 6 * game->graphics->width + 6 * game->graphics->gap)
		*index += 15;
	else if (6 * game->graphics->width + 7 * game->graphics->gap <= x
			&& x <= 7 * game->graphics->width + 7 * game->graphics->gap)
		*index += 18;
	else *index = 81;
}

static void remove_card(Game *game, char *base, char *origin, unsigned short index) {
	unsigned short x, y;
	get_position(game, index, &x, &y);
	vg_clear_pixmap(base, origin, x - 6, y - 6, game->graphics->width + 12, game->graphics->height + 12);
	game->gamestate->display_cards[index] = 81;
	game->gamestate->display--;
}

static void remove_cards(Game *game) {
	remove_card(game, game->graphics->cards_buffer, game->graphics->backg_buffer,
			game->gamestate->remove_cards[0]);
	remove_card(game, game->graphics->cards_buffer, game->graphics->backg_buffer,
			game->gamestate->remove_cards[1]);
	remove_card(game, game->graphics->cards_buffer, game->graphics->backg_buffer,
			game->gamestate->remove_cards[2]);
}

static void add_card(Game *game, char *base, unsigned short index) {
	unsigned short x, y;
	get_position(game, index, &x, &y);
	int width, height;
	unsigned short card_index = game->array[game->gamestate->dealt];
	char **xpm = game->graphics->cards[card_index];
	char *pixmap = read_xpm(xpm, &width, &height);
	vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
	free(pixmap);
	game->gamestate->dealt++;
	game->gamestate->display_cards[index] = card_index;
	game->gamestate->display++;
}

static void add_cards(Game *game) {
	add_card(game, game->graphics->cards_buffer, game->gamestate->add_cards[0]);
	add_card(game, game->graphics->cards_buffer, game->gamestate->add_cards[1]);
	add_card(game, game->graphics->cards_buffer, game->gamestate->add_cards[2]);
}

static void add_cards_remove(Game * game) {
	add_card(game, game->graphics->cards_buffer, game->gamestate->remove_cards[0]);
	add_card(game, game->graphics->cards_buffer, game->gamestate->remove_cards[1]);
	add_card(game, game->graphics->cards_buffer, game->gamestate->remove_cards[2]);
}

static void add_frame(Game *game, char *base, unsigned short index) {
	unsigned short x, y;
	get_position(game, index, &x, &y);
	vg_draw_frame(base, x - 6, y - 6, game->graphics->width + 12, game->graphics->height + 12, 54);
}

static void remove_frame(Game *game, char *base, char *origin, unsigned short index) {
	unsigned short x, y;
	get_position(game, index, &x, &y);
	vg_remove_frame(base, origin, x - 6, y - 6, game->graphics->width + 12, game->graphics->height + 12);
}

static void remove_frames(Game *game) {
	remove_frame(game, game->graphics->cards_buffer, game->graphics->backg_buffer,
			game->gamestate->remove_cards[0]);
	remove_frame(game, game->graphics->cards_buffer, game->graphics->backg_buffer,
			game->gamestate->remove_cards[1]);
	remove_frame(game, game->graphics->cards_buffer, game->graphics->backg_buffer,
			game->gamestate->remove_cards[2]);
}

static void displace_card(Game *game, char *base, char *origin,
	unsigned short destination_index, unsigned short origin_index) {
	unsigned short card_index = game->gamestate->display_cards[origin_index];
	game->gamestate->display_cards[destination_index] = card_index;
	game->gamestate->display_cards[origin_index] = 81;
	char **xpm = game->graphics->cards[card_index];
	int width, height;
	char *pixmap = read_xpm(xpm, &width, &height);
	unsigned short dest_x, dest_y;
	get_position(game, destination_index, &dest_x, &dest_y);
	vg_draw_pixmap(base, dest_x, dest_y, pixmap, (unsigned short)width, (unsigned short)height);
	free(pixmap);
	unsigned short orig_x, orig_y;
	get_position(game, origin_index, &orig_x, &orig_y);
	vg_clear_pixmap(base, origin, orig_x - 6, orig_y - 6,
			game->graphics->width + 12, game->graphics->height + 12);
}

static void set_add_cards(Game *game) {
	if (game->gamestate->display < 21) {
		if (game->gamestate->dealt < 81) {
			game->gamestate->add_cards[0] = game->gamestate->display;
			game->gamestate->add_cards[1] = game->gamestate->display + 1;
			game->gamestate->add_cards[2] = game->gamestate->display + 2;
		}
		game->gamestate->add = 1;
	}
}

static void pause_game(Game *game) {
	game->gamestate->state = GAME_PAUSE;
	vg_draw_time(game->graphics->time_buffer, game->graphics->backg_buffer,
			game->timer, game->graphics->numbers, game->graphics->time_x, game->graphics->time_y);
	vg_draw_buttons(game->graphics->time_buffer, game->graphics->gap, game->graphics->width, game->graphics->height, (char)game->gamestate->state);
	vg_draw_mouse(game->graphics->mouse_buffer, game->graphics->time_buffer, game->mouse);
	copy_buffer(game->graphics->video_mem, game->graphics->mouse_buffer);
}

static void play_game(Game *game) {
	game->gamestate->state = GAME_PLAY;
	vg_draw_time(game->graphics->time_buffer, game->graphics->cards_buffer,
			game->timer, game->graphics->numbers, game->graphics->time_x, game->graphics->time_y);
	vg_draw_buttons(game->graphics->time_buffer, game->graphics->gap, game->graphics->width, game->graphics->height, (char)game->gamestate->state);
	vg_draw_mouse(game->graphics->mouse_buffer, game->graphics->time_buffer, game->mouse);
	copy_buffer(game->graphics->video_mem, game->graphics->mouse_buffer);
}

static void save_game(Game *game) {
	return;
}

static void load_game(Game *game) {
	return;
}

static void menu_game(Game *game) {
	game->gamestate->state = GAME_MENU;
	reset_menu(game->menu);
	vg_fill_screen(game->graphics->backg_buffer, 0);
	draw_menu(game->menu, game->graphics->menu_buffer, game->graphics->backg_buffer, game->graphics->numbers, game->graphics->letters);
	vg_draw_mouse(game->graphics->mouse_buffer, game->graphics->menu_buffer, game->mouse);
	copy_buffer(game->graphics->video_mem, game->graphics->mouse_buffer);
}

static void select_card(Game *game, unsigned short index) {
	if (index < game->gamestate->display && game->gamestate->state == GAME_PLAY)
		if (game->gamestate->remove_card < 3) {
			game->gamestate->remove_cards[game->gamestate->remove_card] = index;
			game->gamestate->remove_card++;
			game->gamestate->frame = 1;
		}
		else game->gamestate->extra_card = index;
}

static void select_button(Game *game, Button button) {
	if (game->gamestate->state == GAME_PLAY) {
		switch(button) {
		case SET_BUTTON:
			if (game->gamestate->remove_card == 3)
				game->gamestate->remove = 1;
			break;
		case NOSETS_BUTTON:
			set_add_cards(game);
			break;
		case HINT_BUTTON:
			game->gamestate->hint = 1;
			break;
		case PAUSE_BUTTON:
			pause_game(game);
			break;
		case SAVE_BUTTON:
			save_game(game);
			break;
		case EXIT_BUTTON:
			menu_game(game);
			break;
		}
	}
	else if (game->gamestate->state == GAME_PAUSE) {
		switch(button) {
		case PAUSE_BUTTON:
			play_game(game);
			break;
		case SAVE_BUTTON:
			save_game(game);
			break;
		case EXIT_BUTTON:
			menu_game(game);
			break;
		}
	}
}

static void state_check(Game *game) {
	if (game->gamestate->state != GAME_PLAY) return;
	if (game->gamestate->hint) {
		if (!get_set(game)) {
			game->gamestate->frame = 1;
			penalize(game->timer, 20);
		}
		else if (game->gamestate->remove_card == 0) {
			set_add_cards(game);
			penalize(game->timer, 10);
		}
		game->gamestate->hint = 0;
	}
	if (game->gamestate->add) {
		if (check_no_sets(game)) {
			game->gamestate->add = 0;
			penalize(game->timer, 20);
		}
		else if (game->gamestate->dealt == 81) game->gamestate->state = GAME_OVER;
	}
	else if (game->gamestate->remove) {
		if (check_set(game)) {
			add_frame(game, game->graphics->cards_buffer,
					game->gamestate->remove_cards[game->gamestate->remove_card - 1]);
			vg_draw_time(game->graphics->time_buffer, game->graphics->cards_buffer,
					game->timer, game->graphics->numbers, game->graphics->time_x, game->graphics->time_y);
			vg_draw_mouse(game->graphics->mouse_buffer, game->graphics->time_buffer, game->mouse);
			copy_buffer(game->graphics->video_mem, game->graphics->mouse_buffer);
			remove_frames(game);
			game->gamestate->remove_card = 0;
			game->gamestate->frame = 0;
			game->gamestate->remove = 0;
			sleep(1);
			vg_draw_time(game->graphics->time_buffer, game->graphics->cards_buffer,
					game->timer, game->graphics->numbers, game->graphics->time_x, game->graphics->time_y);
			vg_draw_mouse(game->graphics->mouse_buffer, game->graphics->time_buffer, game->mouse);
			copy_buffer(game->graphics->video_mem, game->graphics->mouse_buffer);
			penalize(game->timer, 30);
		}
	}
}

static void state_undo(Game *game) {
	if (game->gamestate->state != GAME_PLAY) return;
	if (game->gamestate->remove_card == 2 && game->gamestate->remove_cards[0] == game->gamestate->remove_cards[1]) {
		remove_frame(game, game->graphics->cards_buffer, game->graphics->backg_buffer,
				game->gamestate->remove_cards[0]);
		game->gamestate->remove_card = 0;
		game->gamestate->frame = 0;
	}
	else if (game->gamestate->remove_card == 3) {
		if (game->gamestate->remove_cards[0] == game->gamestate->remove_cards[2]) {
			remove_frame(game, game->graphics->cards_buffer, game->graphics->backg_buffer,
					game->gamestate->remove_cards[0]);
			game->gamestate->remove_cards[0] = game->gamestate->remove_cards[1];
			game->gamestate->remove_card = 1;
			game->gamestate->remove = 0;
		}
		else if (game->gamestate->remove_cards[1] == game->gamestate->remove_cards[2]) {
			remove_frame(game, game->graphics->cards_buffer, game->graphics->backg_buffer,
					game->gamestate->remove_cards[1]);
			game->gamestate->remove_card = 1;
			game->gamestate->remove = 0;
		}
		else if (game->gamestate->remove_cards[0] == game->gamestate->extra_card) {
			remove_frame(game, game->graphics->cards_buffer, game->graphics->backg_buffer,
					game->gamestate->remove_cards[0]);
			game->gamestate->remove_cards[0] = game->gamestate->remove_cards[2];
			game->gamestate->remove_card = 2;
			game->gamestate->remove = 0;
			game->gamestate->extra_card = 21;
		}
		else if (game->gamestate->remove_cards[1] == game->gamestate->extra_card) {
			remove_frame(game, game->graphics->cards_buffer, game->graphics->backg_buffer,
					game->gamestate->remove_cards[1]);
			game->gamestate->remove_cards[1] = game->gamestate->remove_cards[2];
			game->gamestate->remove_card = 2;
			game->gamestate->remove = 0;
			game->gamestate->extra_card = 21;
		}
		else if (game->gamestate->remove_cards[2] == game->gamestate->extra_card) {
			remove_frame(game, game->graphics->cards_buffer, game->graphics->backg_buffer,
					game->gamestate->remove_cards[2]);
			game->gamestate->remove_card = 2;
			game->gamestate->remove = 0;
			game->gamestate->extra_card = 21;
		}
	}
	vg_draw_time(game->graphics->time_buffer, game->graphics->cards_buffer,
			game->timer, game->graphics->numbers, game->graphics->time_x, game->graphics->time_y);
	vg_draw_mouse(game->graphics->mouse_buffer, game->graphics->time_buffer, game->mouse);
	copy_buffer(game->graphics->video_mem, game->graphics->mouse_buffer);
}

static void state_action(Game *game) {
	if (game->gamestate->state != GAME_PLAY) return;
	if (game->gamestate->add) {
		add_cards(game);
		game->gamestate->add = 0;
	}
	if (game->gamestate->frame) {
		add_frame(game, game->graphics->cards_buffer,
				game->gamestate->remove_cards[game->gamestate->remove_card - 1]);
	}
	if (game->gamestate->remove) {
		remove_cards(game);
		game->gamestate->remove = 0;
		game->gamestate->remove_card = 0;
		game->gamestate->frame = 0;
		if (game->gamestate->display == 9 && game->gamestate->dealt < 81)
			add_cards_remove(game);
		else {
			short empty[3] = {-1, -1, -1};
			unsigned short j = 0;
			unsigned short k = 0;
			for (k = 0; k < 3; k++) {
				if (game->gamestate->remove_cards[k] != game->gamestate->display
						&& game->gamestate->remove_cards[k] != (game->gamestate->display + 1)
						&& game->gamestate->remove_cards[k] != (game->gamestate->display + 2)) {
					empty[j] = game->gamestate->remove_cards[k];
					j++;
				}
			}
			short full[3] = {-1, -1, -1};
			j = 0;
			for (k = 0; k < 3; k++) {
				if (game->gamestate->display + k != game->gamestate->remove_cards[0]
						&& game->gamestate->display + k != game->gamestate->remove_cards[1]
						&& game->gamestate->display + k != game->gamestate->remove_cards[2]) {
					full[j] = game->gamestate->display + k;
					j++;
				}
			}
			for (k = 0; k < 3; k++) {
				if (empty[k] == -1) break;
				else displace_card(game, game->graphics->cards_buffer,
						game->graphics->backg_buffer, empty[k], full[k]);
			}
		}
		sleep(1);
	}
	vg_draw_time(game->graphics->time_buffer, game->graphics->cards_buffer,
			game->timer, game->graphics->numbers, game->graphics->time_x, game->graphics->time_y);
	vg_draw_mouse(game->graphics->mouse_buffer, game->graphics->time_buffer, game->mouse);
	copy_buffer(game->graphics->video_mem, game->graphics->mouse_buffer);
}

/*
 * arg[0] is game
 * arg[1] is mse_irq_set
 * arg[2] is kbd_irq_set
 * arg[3] is t0_irq_set
 */
static unsigned short mouse_keyboard_timer_int_handler(int int_irq, long *arg) {
	Game *game = (Game *)arg[0];
	if (int_irq & arg[1]) {
		mouse_int_handler(game->mouse);
	}
	if (int_irq & arg[2]) {
		keyboard_int_handler(game->keyboard);
	}
	if ((int_irq & arg[3]) && game->gamestate->state == GAME_PLAY) {
		timer_int_handler(game->timer);
	}
	return 0;
}

static void compute_scores(Game *game) {
	unsigned long game_time = game->timer->hours * 60 * 60 + game->timer->minutes * 60 + game->timer->seconds;
	switch(game->mode) {
	case SINGLE_MODE:
		if (game_time > 24 * 60 * 60)							/* if player takes more than 1 day, score is zero */
			game->scores[0] = 0;
		else game->scores[0] = 24 * 60 * 60 - game_time;
		break;
	}
}

static void show_highscores(Game *game) {
	unsigned short i = 0;
	unsigned short xi = 261;
	unsigned short yi = 204;
	for (i = 0; i < 10; i++) {
		vg_draw_highscore(game->graphics->cards_buffer, game->graphics->cards_buffer,
				game->highscores[i], game->graphics->numbers, game->graphics->letters, xi, yi);
		yi += 36;
	}
}

static void score_game(Game *game) {
	game->gamestate->state = GAME_SCORE;
	get_data_rtc(game->rtc);
	compute_scores(game);
	update_highscores(game->highscores, game->scores[0], game->players[0], game->rtc->datetime);
	save_highscores(game->highscores);
	vg_fill_screen(game->graphics->cards_buffer, 0);
	show_highscores(game);
	vg_draw_mouse(game->graphics->mouse_buffer, game->graphics->cards_buffer, game->mouse);
	copy_buffer(game->graphics->video_mem, game->graphics->mouse_buffer);
}

static void shuffle(unsigned short *array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          unsigned short t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

static void start_game(Game *game) {
	reset_gamestate(game->gamestate);
	reset_timer(game->timer);
	game->mode = game->menu->mode;
	if (game->mode == SINGLE_MODE || game->mode == HOT_MODE
			|| game->mode == DUEL_MODE || game->mode == SUDDEN_MODE) {
		game->number_players = game->menu->number_players;
		unsigned short i, j;
		for (i = 0; i < 9; i++) {
			for (j = 0; j < 5; j++) {
				game->players[i][j] = game->menu->player_names[i][j];
			}
		}
		for (i = 0; i < 81; i++) {
			game->array[i] = i;
		}
		shuffle(game->array, 81);
//		vg_draw_random_background(game->graphics->backg_buffer);
		unsigned short background_height = game->graphics->gap + 3 * (game->graphics->height + game->graphics->gap);
		vg_draw_random_square_background(game->graphics->backg_buffer, 100, 500, 10, 50, background_height);
		copy_buffer(game->graphics->cards_buffer, game->graphics->backg_buffer);
		for (i = 0; i < 12 ; i++) {
			add_card(game, game->graphics->cards_buffer, game->gamestate->display);
		}
		vg_draw_buttons(game->graphics->cards_buffer, game->graphics->gap, game->graphics->width, game->graphics->height, (char)game->gamestate->state);
		copy_buffer(game->graphics->video_mem, game->graphics->cards_buffer);
	}
	else load_game(game);
}

static unsigned short mouse_keyboard_timer_app_handler(int int_irq, long *arg) {
	Game *game = (Game *)arg[0];
	MenuState menustate;
	if (game->gamestate->state == GAME_MENU) {
		menustate = mouse_keyboard_menu_handler(int_irq, (int)arg[1], (int)arg[2], game->menu,
				game->graphics->menu_buffer, game->graphics->backg_buffer, game->graphics->mouse_buffer,
				game->graphics->video_mem, game->graphics->numbers, game->graphics->letters,
				game->graphics->h_res, game->graphics->v_res, game->mouse, game->keyboard);
		if (menustate == MENU_DONE) {
			start_game(game);
			game->gamestate->state = GAME_PLAY;
		}
		else if ((int_irq & arg[2]) &&
				(game->keyboard->scan_code == ESC_BREAK_CODE))
			if (game->menu->state == MENU_INIT) return 1;
			else menu_game(game);
		return 0;
	}
	if ((int_irq & arg[1]) &&
			(game->gamestate->state == GAME_PLAY
					|| game->gamestate->state == GAME_PAUSE)) {
		if (game->mouse->info) {
			fix_mouse_position(game->mouse, game->graphics->h_res, game->graphics->v_res);
			vg_draw_mouse(game->graphics->mouse_buffer, game->graphics->time_buffer, game->mouse);
			copy_buffer(game->graphics->video_mem, game->graphics->mouse_buffer);
			if (game->mouse->left) {
				unsigned short index;
				get_mouse_card_index(game, &index, game->mouse->x, game->mouse->y);
				select_card(game, index);
				Button button;
				get_mouse_button(game, &button, game->mouse->x, game->mouse->y);
				select_button(game, button);
			}
			game->mouse->info = 0;
		}
	}
	if ((int_irq & arg[1]) && game->gamestate->state == GAME_SCORE) {
		if (game->mouse->info) {
			fix_mouse_position(game->mouse, game->graphics->h_res, game->graphics->v_res);
			vg_draw_mouse(game->graphics->mouse_buffer, game->graphics->cards_buffer, game->mouse);
			copy_buffer(game->graphics->video_mem, game->graphics->mouse_buffer);
			game->mouse->info = 0;
		}
	}
	if ((int_irq & arg[2]) && game->gamestate->state == GAME_PLAY) {
		Button button;
		get_keyboard_button(&button, game->keyboard->scan_code);
		select_button(game, button);
	}
	else if ((int_irq & arg[2]) && game->gamestate->state == GAME_PAUSE &&
			(game->keyboard->scan_code == CTRL_MAKE_CODE
					|| game->keyboard->scan_code == P_MAKE_CODE))
		play_game(game);
	else if ((int_irq & arg[2]) &&
			(game->gamestate->state == GAME_SCORE)
			&& (game->keyboard->scan_code == ESC_BREAK_CODE
					|| game->keyboard->scan_code == E_MAKE_CODE)) {
			menu_game(game);
	}
	if ((int_irq & arg[3]) && game->gamestate->state == GAME_PLAY) {
		vg_draw_time(game->graphics->time_buffer, game->graphics->cards_buffer,
				game->timer, game->graphics->numbers, game->graphics->time_x, game->graphics->time_y);
		vg_draw_mouse(game->graphics->mouse_buffer, game->graphics->time_buffer, game->mouse);
		copy_buffer(game->graphics->video_mem, game->graphics->mouse_buffer);
	}
	state_undo(game);
	state_check(game);
	state_action(game);
	if (game->gamestate->state == GAME_OVER) {
		score_game(game);
		return 0;
	}
	return 0;
}

#define DEBUG_RUN_GAME	0
int run_game() {
	Game *game;
	if ((game = create_game()) == NULL) {
		printf("test_cards: error creating game!\n");
		return 1;
	}
	if (DEBUG_RUN_GAME) printf("game created\n");
	load_highscores(game->highscores);
	if (DEBUG_RUN_GAME) printf("highscores loaded\n");
	draw_menu(game->menu, game->graphics->menu_buffer, game->graphics->backg_buffer, game->graphics->numbers, game->graphics->letters);
	copy_buffer(game->graphics->video_mem, game->graphics->menu_buffer);
	if (DEBUG_RUN_GAME) printf("menu showed\n");
	int kbd_hook_id = KBD_HOOK_ID;
	int mse_hook_id = MSE_HOOK_ID;
	int t0_hook_id = T0_HOOK_ID;
	int mse_irq_set;
	int kbd_irq_set;
	int t0_irq_set;
	if ((mse_irq_set = subscribe_int(MSE_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &mse_hook_id, 1)) < 0) {
		destroy_game(game);
		vg_exit();
		printf("test_cards: mouse subscribe_int failed!\n");
		return 1;
	}
	if (mse_write_read(KBC_MSE_DSBSM) != 0) {
		destroy_game(game);
		vg_exit();
		printf("test_cards: mouse disable stream mode failed!\n");
		return 1;
	}
	if (mse_write_read(KBC_MSE_SETSM) != 0) {
		destroy_game(game);
		vg_exit();
		printf("test_cards: mouse set stream mode failed!\n");
		return 1;
	}
	if (mse_write_read(KBC_MSE_ENBDP) != 0) {
		destroy_game(game);
		vg_exit();
		printf("test_cards: mouse enable data packets failed!\n");
		return 1;
	}
	if ((kbd_irq_set = subscribe_int(KBD_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &kbd_hook_id, 1)) < 0) {
		destroy_game(game);
		vg_exit();
		printf("test_cards: keyboard subscribe_int failed!\n");
		return 1;
	}
	if ((t0_irq_set = subscribe_int(T0_IRQ, IRQ_REENABLE, &t0_hook_id, 1)) < 0) {
		destroy_game(game);
		vg_exit();
		printf("test_cards: timer 0 subscribe_int failed!\n");
		return 1;
	}
	if (DEBUG_RUN_GAME) printf("interruptions subscribed\n");
	long arg[4];
	arg[0] = (long) game;
	arg[1] = mse_irq_set;
	arg[2] = kbd_irq_set;
	arg[3] = t0_irq_set;
	interrupt_loop(mse_irq_set | kbd_irq_set | t0_irq_set,
			mouse_keyboard_timer_int_handler, mouse_keyboard_timer_app_handler, arg);
	if (DEBUG_RUN_GAME) printf("interrupt loop done\n");
	if (mse_write_read(KBC_MSE_DSBSM) != 0) {
		destroy_game(game);
		vg_exit();
		printf("test_cards: mouse disable stream mode failed!\n");
		return 1;
	}
	if (mse_write_read(KBC_MSE_SETSM) != 0) {
		destroy_game(game);
		vg_exit();
		printf("test_cards: mouse set stream mode failed!\n");
		return 1;
	}
	if (unsubscribe_int(&t0_hook_id) != 0) {
		destroy_game(game);
		vg_exit();
		printf("test_cards: timer 0 unsubscribe_int failed!\n");
		return 1;
	}
	if (unsubscribe_int(&kbd_hook_id) != 0) {
		destroy_game(game);
		vg_exit();
		printf("test_cards: keyboard unsubscribe_int failed!\n");
		return 1;
	}
	if (unsubscribe_int(&mse_hook_id) != 0) {
		destroy_game(game);
		vg_exit();
		printf("test_cards: mouse unsubscribe_int failed!\n");
		return 1;
	}
	if (DEBUG_RUN_GAME) printf("interruptions unsubscribed\n");
	destroy_game(game);
	if (DEBUG_RUN_GAME) printf("game destroyed\n");
	vg_exit();
	return 0;
}
