#include "xpm.h"
#include "game.h"
#include "video_gr.h"
#include "variables.h"
#include "print.h"
#include "pixmap.h"
#include "macros.h"
#include "utilities.h"
#include "file_handler.h"

Ship_t ship = {.map = Spaceship_xpm};
Enemy_t enemy = {.map = Invader_xpm, .destroyed = 0};
Pong_t pong = {.map = Pong_xpm};

void init() {
	ship.x = SHIP_X_INITIAL;
	ship.y = SHIP_Y_INITIAL;
	clear_buffer();
	estado = START;

	// Cria a nave
	object_creator(&ship.width, &ship.height, ship.x, ship.y, ship.map);

	// Cria a matriz de inimigos
	int i;
	int b;
	for (b = 0; b < ENEMY_ROWS; b++) {
		// Inicializa x e y com as primeiras posicoes
		int x = FST_ENEMY_COL;
		int y = FST_ENEMY_COL;
		for (i = 0; i < ENEMY_COLLUMNS; i++) {
			// Atualiza parametros para o inimigo
			enemy.y = y * (b + 1);
			enemy.x = x;
			enemy.score = 6 * ((ENEMY_ROWS)-b); // Formula de calculo da pontuacao

			// Cria um inimigo
			object_creator(&enemy.width, &enemy.height, enemy.x, enemy.y, enemy.map);

			// Coloca na matriz
			enemies[b][i] = enemy;

			// Atualiza a posicao x para o inimigo seguinte
			x += BETWEEN_ENEMIES;
		}
	}

	estado = PONG_DISABLED;

	switch (kbd_mouse_menu) {
	case KEYBOARD_KEYBOARD: {
		mouse_unsubscribe();
		start_game();
		mouse_subs = mouse_subscribe();
		mouse_command(SET_STREAM);
		mouse_command(DR_ENA);
		mouse_set = BIT(mouse_subs);
		break;
	}
	case MOUSE_MOUSE: {
		start_game();
		break;
	}
	default:
		start_game();
		break;
	}

	copy_to_video_mem();

	switch (estado) {
	case NO_ENEMIES: {
		if (time_counter)
			total_score += time_counter;
		print_message(YouWin_xpm);
		print_score();
		break;
	}
	case GAME_OVER:
		print_message(YouLose_xpm);
		print_score();
		break;
	}

	if (total_score)
		write_score();

	copy_to_video_mem();

	return;
}

int is_ship() {
	int i;
	int b;
	for (b = 0; b < ENEMY_ROWS; b++) {
		for (i = 0; i < ENEMY_COLLUMNS; i++) {
			if (!enemies[b][i].destroyed) {
				if ((enemies[b][i].y + enemies[b][i].height) > ship.y)
					return 1;
			}
		}
	}
	return 0;
}

int no_enemies() {
	int i;
	int b;
	for (b = 0; b < ENEMY_ROWS; b++) {
		for (i = 0; i < ENEMY_COLLUMNS; i++) {
			if (!enemies[b][i].destroyed) {
				return 0;
			}
		}
	}
	estado = NO_ENEMIES;
	return 1;
}

void is_enemy() {
	int i;
	int b;

	for (i = 0; i < ENEMY_COLLUMNS; i++) {
		if (!enemies[0][i].destroyed && (enemies[0][i].x - pong.width) < pong.x &&
				(enemies[0][i].x + enemies[0][i].width + pong.width) > pong.x) {
			for (b = ENEMY_ROWS - 1; b >= 0; b--) {
				if (!enemies[b][i].destroyed && enemies[b][i].y > pong.y &&
						enemies[b][i].y > (pong.y - pong.height)) {
					enemies[b][i].destroyed = 1;
					buffer_destructor(enemies[b][i].width, enemies[b][i].height,
							enemies[b][i].x, enemies[b][i].y);
					total_score += enemies[b][i].score;
					estado = PONG_DISABLED;
					buffer_destructor(pong.width, pong.height, pong.x, pong.y);

					return;
				}
			}
		}
	}
	return;
}

void fire_pong() {
	if (estado == PONG_DISABLED) {
		pong.x = ship.x + ship.width / 2;
		pong.y = ship.y - ship.height / 2;
		estado = PONG_ENABLED;
		object_creator(&pong.width, &pong.height, pong.x, pong.y, pong.map);
	}

	return;
}

void round() {
	print_info();

	if (no_enemies())
		return;

	if (estado == PONG_ENABLED) {
		buffer_destructor(pong.width, pong.height, pong.x, pong.y);
		pong.y -= PONG_SPEED;

		if (pong.y > 0)
			object_creator(&pong.width, &pong.height, pong.x, pong.y, pong.map);
		else {
			pong.y += PONG_SPEED;
			buffer_destructor(pong.width, pong.height, pong.x, pong.y);
			estado = PONG_DISABLED;
		}

		is_enemy();
	} else
		buffer_destructor(pong.width, pong.height, pong.x, pong.y);

	copy_to_video_mem();

	return;
}

int enemy_movedown() {
	int i;
	int b;
	for (b = 0; b < ENEMY_ROWS; b++) {
		for (i = 0; i < ENEMY_COLLUMNS; i++) {
			if (!enemies[b][i].destroyed) {
				buffer_destructor(enemies[b][i].width, enemies[b][i].height,
						enemies[b][i].x,
						enemies[b][i].y); // can or not comment
				enemies[b][i].y += ENEMY_DOWN_Y;
			}
		}
	}
	for (b = 0; b < ENEMY_ROWS; b++) {
		for (i = 0; i < ENEMY_COLLUMNS; i++) {
			if (!enemies[b][i].destroyed) {
				if (!is_ship())
					object_creator(&enemies[b][i].width, &enemies[b][i].height,
							enemies[b][i].x, enemies[b][i].y, enemies[b][i].map);
				else
					return 1;
			}
		}
	}

	return 0;
}

void start_game() {
	int ticks = 0;
	int code;
	estado = PONG_DISABLED;
	time_counter = ENEMY_DOWN_TIME * ENEMY_ROWS * 2.2;
	kbd_code = 0;

	title_creator(ENTER_XPM_X, GAME_TITLES_Y, Enter_xpm);
	title_creator(SCORE_XPM_X, GAME_TITLES_Y, Score_xpm);
	title_creator(TIME_XPM_X, GAME_TITLES_Y, TimeLeft_xpm);

	while (estado != ENTER_PRESSED) {
		if (driver_receive(ANY, &msg, &ipc_status) != 0)
			continue;
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & timer_set) // timer
				{
					switch (estado) {
					case GAME_OVER:
						clear_buffer();
						return;
					case NO_ENEMIES:
						clear_buffer();
						return;
					}

					if (ticks % 60 == 0)
						time_counter--;
					ticks++;
					round();
					if (ticks % (ENEMY_DOWN_TIME * 60) == 0)
						if (enemy_movedown()) {
							estado = GAME_OVER;
						}
				}
				if (msg.NOTIFY_ARG & kbd_set) // keyboard
				{
					kbd_code = kbd_scan();
					switch (kbd_code) {
					case ENTER_COD: {
						clear_buffer();
						estado = ENTER_PRESSED;
						kbd_code = RESET_KEY; // reset kbd_code
					}
					case SPC_COD:
						if (kbd_mouse_menu == KEYBOARD_KEYBOARD ||
								kbd_mouse_menu == MOUSE_KEYBOARD) {
							switch (estado) {
							case PONG_DISABLED:
								pong.x = ship.x + ship.width / 2;
								pong.y = ship.y - ship.height / 2;
								estado = PONG_ENABLED;
								object_creator(&pong.width, &pong.height, pong.x, pong.y,
										pong.map);
							}
							break;
						}
						break;

					case A_PRESS:
						if (kbd_mouse_menu == KEYBOARD_KEYBOARD ||
								kbd_mouse_menu == KEYBOARD_MOUSE) {
							kbd_code = kbd_scan();
							buffer_destructor(ship.width, ship.height, ship.x,
									ship.y); // deletes xpm
							if (ship.x - SHIP_SPEED <= 0)
								ship.x = 0;
							else
								ship.x -= SHIP_SPEED; // changes x value
							object_creator(&ship.width, &ship.height, ship.x, ship.y,
									ship.map); // creates xpm in ne
						}
						break;

					case D_PRESS:
						if (kbd_mouse_menu == KEYBOARD_KEYBOARD ||
								kbd_mouse_menu == KEYBOARD_MOUSE) {
							kbd_code = kbd_scan();
							buffer_destructor(ship.width, ship.height, ship.x, ship.y);
							if (ship.x + SHIP_SPEED >= (getHRes() - ship.width))
								ship.x = (getHRes() - ship.width);
							else
								ship.x += SHIP_SPEED; // changes x value
							object_creator(&ship.width, &ship.height, ship.x, ship.y,
									ship.map);
							break;
						}
						break;

					default:
						break;
					}
				}

				if (msg.NOTIFY_ARG & mouse_set) // mouse
				{
					char packet[3] = {0, 0, 0};
					int fst = 0, snd = 0;
					char mouse_code;
					unsigned long rsp;
					int l_button;
					char pacote;

					int i = 0;
					for (i; i < 3; i++) {
						rsp = mouse_scan();
						code = (char)rsp;
						if (fst == 0) {
							if (BIT(3) == (BIT(3) & code) && is_mouse_byte()) {
								fst = 1;
								packet[0] = code;
							} else
								continue;
						} else if (snd == 0) {
							snd = 1;
							packet[1] = code;
						} else {
							fst = 0;
							snd = 0;
							packet[2] = code;
							if (!packet[0] & BIT(3))
								return;

							if ((packet[0] & BIT(4)) == BIT(4)) {
								packet[1] = twos_complement(packet[1]);
							}

							if ((packet[0] & BIT(5)) == BIT(5)) {
								packet[2] = twos_complement(packet[2]);
							}
						}
					}

					l_button = (packet[0] & BIT(0));

					if (kbd_mouse_menu == KEYBOARD_MOUSE ||
							kbd_mouse_menu == MOUSE_MOUSE) {
						if (l_button)
							fire_pong();
					}

					if (kbd_mouse_menu == MOUSE_KEYBOARD ||
							kbd_mouse_menu == MOUSE_MOUSE) {
						if ((int)packet[1] > 0) {
							buffer_destructor(ship.width, ship.height, ship.x, ship.y);
							if (ship.x + packet[1] >= (getHRes() - ship.width))
								ship.x = (getHRes() - ship.width);
							else
								ship.x += packet[1]; // changes x value
							object_creator(&ship.width, &ship.height, ship.x, ship.y,
									ship.map);
						}

						if ((int)packet[1] < 0) {
							buffer_destructor(ship.width, ship.height, ship.x,
									ship.y); // deletes xpm
							if (ship.x + packet[1] <= 0)
								ship.x = 0;
							else
								ship.x += packet[1]; // changes x value
							object_creator(&ship.width, &ship.height, ship.x, ship.y,
									ship.map); // creates xpm in new coordinates
						}
					}
				}
			}
		}
	}
	return;
}
