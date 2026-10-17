#include <minix/syslib.h>
#include <minix/drivers.h>
#include "game.h"
#include "field.h"
#include "vbe.h"
#include "mouse.h"
#include "timer.h"
#include "keyboard.h"
#include "multiplayer.h"
#include "porta.h"
#include "font.h"
#include "rtc.h"

int irq_set_kbc;
int irq_set_mouse;
int irq_set_timer;

int main() {

	int segundos;
	int rest;
	int op;
	srand(time(NULL));

	setLado(11);
	setMines(15);
	setAddr(0x2F8);

	sef_startup();
	vg_init(0x103);
	Campo_t field[getLado()][getLado()];
	clear_fields(getLado(), field);
	printf("subscribe kbc \n");
	irq_set_kbc = subscribe_kbc();
	printf("subscribe ms \n");
	irq_set_mouse = subscribe_mouse();
	printf("subscribe tm \n");
	irq_set_timer = subscribe_timer();

	printf("done \n");

	do {

		op = menu();

		if (op == 1)

			single_play(field);

		if (op == 2)

			multiplayer(1, field);

		if (op == 3)

			multiplayer(0, field);

	} while (op != 4);

	unsubscribe_mouse();

	unsubscribe_timer();

	printf("mouse unsubscribed \n");

	vg_exit();

	printf("exiting video grafic mode\n");

	printf("flags: %d flags certas: %d \n", getFlags(), getFlagsCertas());

	return 0;

}
void clear_fields(int lado, Campo_t field[][getLado()]) {
	int i, j;

	for (i = 0; i < lado; i++) {
		for (j = 0; j < lado; j++) {
			field[i][j].revealed = 0;
			field[i][j].mine = 0;
			field[i][j].flag = 0;
			field[i][j].number = 0;
		}
	}

}
void set_fields(int lado, int mines, int row, int col,
		Campo_t field[][getLado()]) {

	int r, s, i, j;
	int count = mines;
	printf("preprando o campo, hue\n");

	printf("campo preparado , hue\n");
	field[row][col].revealed = 1;
	printf("campo clikado revelado, hue \n");
	while (count != 0) {

		r = rand() % lado;
		s = rand() % lado;
		if (field[r][s].mine == 0 && field[r][s].revealed == 0) {
			field[r][s].mine = 1;
			count--;
		}

	}
	printf("minas postas , hue\n");
	for (i = 0; i < lado; i++) {
		for (j = 0; j < lado; j++) {
			count = 0;
			if (field[i][j].mine == 1)
				field[i][j].number = -1;
			else {
				if ((i + 1 >= 0 && i + 1 < lado) && (j >= 0 && j < lado)
						&& field[i + 1][j].mine == 1)
					count++;
				if ((i >= 0 && i < lado) && (j + 1 >= 0 && j + 1 < lado)
						&& field[i][j + 1].mine == 1)
					count++;
				if ((i + 1 >= 0 && i + 1 < lado) && (j + 1 >= 0 && j + 1 < lado)
						&& field[i + 1][j + 1].mine == 1)
					count++;
				if ((i - 1 >= 0 && i - 1 < lado) && (j >= 0 && j < lado)
						&& field[i - 1][j].mine == 1)
					count++;
				if ((i - 1 >= 0 && i - 1 < lado) && (j - 1 >= 0 && j - 1 < lado)
						&& field[i - 1][j - 1].mine == 1)
					count++;
				if ((i >= 0 && i < lado) && (j - 1 >= 0 && j - 1 < lado)
						&& field[i][j - 1].mine == 1)
					count++;
				if ((i + 1 >= 0 && i + 1 < lado) && (j - 1 >= 0 && j - 1 < lado)
						&& field[i + 1][j - 1].mine == 1)
					count++;
				if ((i - 1 >= 0 && i - 1 < lado) && (j + 1 >= 0 && j + 1 < lado)
						&& field[i - 1][j + 1].mine == 1)
					count++;

				field[i][j].number = count;
			}
		}

	}
	field[row][col].revealed = 0;
	reveal(row, col, field);
}

int single_play(Campo_t field[][getLado()]) {

	//interr
	int ipc_status;
	message msg;
	int r;
	int me = 1;
	//outros

	int j = 0;

	//start screen
	vg_fill(0);
	draw_grid_easy(0, field);
	draw_mouse(getMousex(), getMousey(), 63);
	update_graf();

	//timer
	printf("getting timer ready\n");
	unsigned long timer = 0;

	printf("timer set\n");

	mouse_start();

	//starting interrupt cicle ( starting game)
	printf("interrupt loop beggining\n");
	while (1) {

		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed \n");

		}
		if (is_ipc_notify(ipc_status)) {

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE:

				if (msg.NOTIFY_ARG & irq_set_mouse) {
					//printf("notification received\n");
					j = mouse_handle();
					if (j == 3) {

						mouse_int(field, 1, 0);
						j = 0;
					}

				}
				if (msg.NOTIFY_ARG & irq_set_timer) {
					//printf("timer notification received\n");
					timer++;
					if (timer == 60) {
						timer = 0;
						IncSeg();
						printf("Segundos: %d", GetSeg());
					}
					if (timer % 1 == 0)

						update_graf();

				}
				if (msg.NOTIFY_ARG & irq_set_kbc) {

					unsigned long code = 0;
					do {
						sys_inb(0x60, &code);
					} while (code == 0);
					printf("Scan:  0x%X \n", code);
					if (code == 0x81) { //ESC
						reset(field);
						return 0;
					}
					if (code == 0x93) { //r
						reset(field);

					}

				}
				break;
			default:
				break;
			}
		}

	}

	printf("exit");

	return 0;
}

void reveal(int i, int j, Campo_t field[][getLado()]) {
	if (i < getLado() && j < getLado() && i >= 0 && j >= 0
			&& field[i][j].revealed == 0) {
		field[i][j].revealed = 1;
		printf("vai dar erro\n");
		if (field[i][j].number == 0) {
			reveal(i + 1, j, field);
			reveal(i, j - 1, field);
			reveal(i, j + 1, field);
			reveal(i - 1, j, field);
			reveal(i + 1, j + 1, field);
			reveal(i - 1, j - 1, field);
			reveal(i - 1, j + 1, field);
			reveal(i + 1, j - 1, field);

		}
	}
}
int menu() {

	int ipc_status;
	message msg;
	int r;
	int tmr = 0;
	int rd = 0;
	int option = 1;
	draw_menu(option);
	update_graf();
	set_rtc();
	while (1) {

		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed \n");

		}
		if (is_ipc_notify(ipc_status)) {

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_timer) {
					tmr++;
					if (tmr % 30) {
						vg_fill(0);
						rtc_handle();
						tmr = 0;
					}

				}
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					printf("notification received\n");
					mouse_handle();
				}
				if (msg.NOTIFY_ARG & irq_set_kbc) {
					printf("notification received");

					unsigned long code = 0;
					do {
						sys_inb(0x60, &code);
					} while (code == 0);

					printf("Scan:  0x%X \n", code);
					if (rd == 1) {
						code = 0xE000 | code;
						rd = 0;
					}
					if (code == 0xE0)
						rd = 1;

					printf("Scan:  0x%X \n", code);
					if (rd == 0) {
						switch (code) {
						case 0x9C:
							return option;
							break;
						case 0x81:
							return 4;
							break;
						case 0xE048:
							option--;
							break;
						case 0xE050:
							option++;
							break;
						default:
							break;

						}
						if (option < 1)
							option = 1;
						if (option > 4)
							option = 4;
					}

					break;
					default:
					break;
				}

			}
		}

		draw_menu(option);
		update_graf();

	}
	return 0;
}

int multiplayer(int host, Campo_t field[][getLado()]) {

	int me;
	if (host == 1)
		me = 1;
	else
		me = 2;

	int ipc_status;
	message msg;
	int r, i, j;
	int ki, kj, kcount = 0;

	vg_fill(0);
	draw_grid_easy(0, field);
	draw_mouse(getMousex(), getMousey(), 63);
	update_graf();
	unsigned long timer = 0;
	int lock = 0;
	int ori, reading = 0;
	int rdmi = 0;
	unsigned long byte;
	int rcv, rcvi, rcvj;
	int recep = 0;
	setUARTConfiguration(getAddr(), 8, 1, -1, 9600);
	set_poll(getAddr());
	mouse_start();
	printUARTConfiguration(getAddr());

	printf("interrupt loop beggining\n");
	while (1) {

		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed \n");

		}
		if (is_ipc_notify(ipc_status)) {

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE:

				if (msg.NOTIFY_ARG & irq_set_mouse) {
					//printf("notification received mouse\n");
					j = mouse_handle();
					if (j == 3) {

						if (i > 30) {

							mouse_int(field, me, 1);
						}
						j = 0;
						i++;
					}

				}
				if (msg.NOTIFY_ARG & irq_set_timer) {
					//printf("timer notification received\n");
					timer++;
					if (timer == 60) {
						timer = 0;
						IncSeg();

					}
					if (timer % 1 == 0) {

						if (me != getPlayer()) {
							char car;
							if (lock != 2 && getFirstClick() == 1)
								lock = 2;
							if (lock == 0) {
								receiveCharlight(getAddr(), &car);
								printf("recebido %d: %d\n", reading, car);
								if (car == 'm')
									lock = 1;
							} else if (lock == 1) {

								receiveCharlight(getAddr(), &car);
								printf("recebido %d: %d\n", reading, car);
								clear_fields(getLado(), field);
								while (car != 's') {
									if (kcount == 0) {

										ki = car;
										kcount++;
									} else if (kcount == 1) {
										kj = car;
										kcount = 0;

										printf(
												"setting field %d %d with a mine \n",
												ki, kj);
										field[ki][kj].mine = 1;
									}
									receiveChar(getAddr(), &car);
									printf("recebido %d: %d\n", reading, car);
								}

								lock = 2;
							} else if (lock == 2) {
								receiveCharlight(getAddr(), &car);
								printf("recebido %d: %d\n", reading, car);

								if (car == 'c') {
									changePlayer();
								}
								if (car == 'k') {

									while (reading < 3) {
										receiveChar(getAddr(), &car);
										printf("recebido %d: %d\n", reading,
												car);

										if (reading == 0)
											rcv = car;
										if (reading == 1)
											rcvi = car;
										if (reading == 2)
											rcvj = car;

										reading++;
									}
									printf("rcv = %d \n ci = %d \n cj = %d \n",
											rcv, rcvi, rcvj);
									if (rcv == 1) {
										if (rcvi < getLado() && rcvj < getLado()
												&& rcvi >= 0 && rcvj >= 0) {
											if (field[rcvi][rcvj].flag == 0) {
												if (getFirstClick() == 0) {
													set_fields(getLado(), 0,
															rcvi, rcvj, field);
													setFirstClick(1);
												}
												reveal(rcvi, rcvj, field);

												if (field[rcvi][rcvj].mine
														== 1) {

													setli(rcvi);
													setlj(rcvj);
													setLost(1);
													int m, n;
													for (m = 0; m < getLado();
															m++) {
														for (n = 0;
																n < getLado();
																n++) {
															if (field[m][n].mine
																	== 1)
																field[m][n].revealed =
																		1;
														}
													}
												}
											}
										}
									} else if (rcv == 2) {
										if (rcvi < getLado() && rcvj < getLado()
												&& rcvi >= 0 && rcvj >= 0
												&& field[rcvi][rcvj].revealed
														== 0) {
											if (field[rcvi][rcvj].flag == 0) {
												field[rcvi][rcvj].flag = 1;
												incFlags();
												if (field[rcvi][rcvj].mine
														== 1) {
													if (me == 1)
														incPlayer2();
													else
														incPlayer1();

													incFlagsCertas();
												} else {
													changePlayer();
													decFlags();
													field[rcvi][rcvj].flag = 0;
												}

											} else {

											}
											if (getFlagsCertas() == getMines())
												if (getFlagsCertas()
														== getFlags()) {
													if (getPlayer1 > getPlayer2)
														setFlagColor(1);
													else
														setFlagColor(2);
												}
										}
									}
									reading = 0;

								}

							}

							//baza
							draw_grid_easy(getFirstClick(), field);
							draw_mouse(getMousex(), getMousey(), 63);
						}

						update_graf();

					}
				}
				if (msg.NOTIFY_ARG & irq_set_kbc) {

					unsigned long code = 0;
					do {
						sys_inb(0x60, &code);
					} while (code == 0);
					printf("Scan:  0x%X \n", code);
					if (code == 0x81) { //ESC
						reset(field);
						return 0;
					}
					if (code == 0x93) { //r
						reset(field);

					}

				}
				break;
			default:
				break;
			}
		}

	}

	return 0;
}
