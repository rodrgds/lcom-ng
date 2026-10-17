#include "vbe.h"
#include "mouse.h"
#include "multiplayer.h"
#include "field.h"
#include "porta.h"
#include "general.h"
#include <minix/sysutil.h>
int hook_id_mouse = 12;
unsigned long packet[3];
int mouse_x = 100;
int mouse_y = 100;
int lost = 0, won = 0;
int left_click = 0;
int first_click = 0;
int right_click = 0;
int li, lj;
int sensiblidade = 1.175;
int j = 0;

int subscribe_mouse() {

	int irqf = 12;

	int irq_set = BIT(hook_id_mouse);
	if (sys_irqsetpolicy(irqf, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse)
			!= OK || sys_irqenable(&hook_id_mouse) != OK)
		return -1;
	else
		return irq_set;
}

int unsubscribe_mouse() {
	if (sys_irqdisable(&hook_id_mouse) != OK) {
	}
	if (sys_irqrmpolicy(&hook_id_mouse) != OK) {
		printf("IRQRMPOLICY falhou /n");
		return -1;
	}
	return 0;
}

void clear_buffer() {
	unsigned long trash;
	sys_outb(0x60, trash);
}

void mouse_start() {
	unsigned long byte;
	do {
		printf("starting mouse \n");
		sys_outb(0x64, 0xD4);
		printf("1\n");
		sys_outb(0x60, 0xf4);
		printf("2\n");
		sys_inb(0x60, &byte);
		printf("4\n");

	} while (byte != 0xFA);
	/*
	 sys_outb(0x64, 0x20);
	 sys_inb(0x60, &byte);
	 byte = byte | BIT(1) | BIT(0);

	 sys_outb(0x64, 0x60);
	 sys_outb(0x64,byte);
	 tickdelay(micros_to_ticks(20000));
	 sys_inb(0x60, &byte);
	 */

	printf("mouse done/n");

}

int mouse_handle() {

	unsigned long data;
	int k;

	sys_inb(OUT_BUF, &data);

	if (j == 0 && data & BIT(3)) {
		packet[0] = data;
		j++;
	} else if (j != 0) {
		packet[j] = data;
		j++;
	}
	k = j;
	if (j == 3)
		j = 0;
	return k;
}

unsigned char getpacket(int i) {

	return packet[i];
}
void mouse_int(Campo_t field[][getLado()], int me, int multi) {

	int x, y;
	int ci, cj;
	int m, n;
	char tx;
	int enviar;
	if (packet[0] & BIT(XSIGN))
		x = packet[1] | 0xFFFFFF00;
	else
		x = packet[1];

	x = x * sensiblidade;

	mouse_x += x;

	if (packet[0] & BIT(YSIGN))
		y = packet[2] | 0xFFFFFF00;
	else
		y = packet[2];

	y = y * sensiblidade;

	mouse_y -= y;

	if (mouse_x > 798)
		mouse_x = 798;
	if (mouse_y > 596)
		mouse_y = 596;
	if (mouse_x < 0)
		mouse_x = 0;
	if (mouse_y < 0)
		mouse_y = 0;

	if (lost == 0 && won == 0 && getPlayer() == me) {

		if (packet[0] & BIT(0)) {
			if (left_click == 0) {
				left_click = 1;
			}
		}

		else if (left_click == 1) {

			cj = getcol(mouse_x);
			ci = getrow(mouse_y);
			enviar = 0;
			char ch;

			if (ci < getLado() && cj < getLado() && ci >= 0 && cj >= 0) {
				if (field[ci][cj].flag == 0) {
					if (first_click == 0) {
						clear_fields(getLado(), field);
						set_fields(getLado(), getMines(), ci, cj, field);
						if (multi == 1) {
							tx = 'm';
							sendChar(getAddr(), tx);
							for (m = 0; m < getLado(); m++) {
								for (n = 0; n < getLado(); n++) {
									if (field[m][n].mine == 1) {
										tx = m;
										sendChar(getAddr(), tx);

										tx = n;
										sendChar(getAddr(), tx);
									}
								}
							}
							tx = 's';

							sendChar(getAddr(), tx);
							while (enviar < 4) {

								printf("enviar: %d \n", enviar);
								if (enviar == 0)
									ch = 'k';
								if (enviar == 1)
									ch = 1;
								if (enviar == 2)
									ch = ci;
								if (enviar == 3)
									ch = cj;
								sendChar(getAddr(), ch);

								enviar++;
							}
						}
						first_click = 1;
					}
					reveal(ci, cj, field);

					if (field[ci][cj].mine == 1) {

						if (multi == 1) {
							changePlayer();
							ch = 'c';
							sendChar(getAddr(), ch);
							field[ci][cj].revealed = 0;
						} else {

							li = ci;
							lj = cj;
							lost = 1;

							for (m = 0; m < getLado(); m++) {
								for (n = 0; n < getLado(); n++) {
									if (field[m][n].mine == 1)
										field[m][n].revealed = 1;
								}
							}
						}
					} else if (multi == 1) {

						if (first_click == 1) {
							while (enviar < 4) {

								printf("enviar: %d \n", enviar);
								if (enviar == 0)
									ch = 'k';
								if (enviar == 1)
									ch = 1;
								if (enviar == 2)
									ch = ci;
								if (enviar == 3)
									ch = cj;
								sendChar(getAddr(), ch);
								tickdelay(micros_to_ticks(500));
								enviar++;
							}
						} else
							printf("skipped first click \n");

					}
				}
			}
			left_click = 0;

		}
		if (packet[0] & BIT(1)) {
			if (right_click == 0) {
				right_click = 1;
			}
		}

		else if (right_click == 1) {

			cj = getcol(mouse_x);
			ci = getrow(mouse_y);
			if (multi == 1) {
				enviar = 0;
				char ch;
				while (enviar < 4) {

					printf("enviar: %d \n", enviar);
					if (enviar == 0)
						ch = 'k';
					if (enviar == 1)
						ch = 2;
					if (enviar == 2)
						ch = ci;
					if (enviar == 3)
						ch = cj;
					sendChar(getAddr(), ch);
					tickdelay(micros_to_ticks(500));
					enviar++;
				}

			}
			if (ci < getLado() && cj < getLado() && ci >= 0 && cj >= 0
					&& field[ci][cj].revealed == 0) {
				if (field[ci][cj].flag == 0) {
					field[ci][cj].flag = 1;
					incFlags();
					if (field[ci][cj].mine == 1) {
						incFlagsCertas();

						if (multi == 1) {
							if (me == 1)
								incPlayer1();
							else
								incPlayer2();
						}
					} else{
						if(multi==1){
							changePlayer();
						decFlags();
						field[ci][cj].flag = 0;
						}
					}

				} else {
					if (multi == 0) {
						field[ci][cj].flag = 0;
						decFlags();
						if (field[ci][cj].mine == 1)
							decFlagsCertas();
					}

				}
				if (getFlagsCertas() == getMines())
					if (getFlagsCertas() == getFlags()) {
						won = 1;
						if (multi == 0)
							setFlagColor(45);
						if (multi == 1) {
							if (getPlayer1 > getPlayer2)
								setFlagColor(1);
							else
								setFlagColor(2);
						}
					}
			}
			right_click = 0;
		}
	}
	vg_fill(0);

	draw_grid_easy(first_click, field);

	if (lost == 1)
		draw_lose(li, lj);

	draw_mouse(mouse_x, mouse_y, 63);

}
int getMousex() {
	return mouse_x;
}
int getMousey() {
	return mouse_y;
}
int getLost() {
	return lost;
}
void setLost(int n) {
	lost = n;
}
int getli() {
	return li;
}
void setli(int n) {
	li = n;
}
int getlj() {
	return lj;
}
void setlj(int n) {
	lj = n;
}
int getFirstClick() {
	return first_click;

}
void setFirstClick(int n) {
	first_click = n;

}
void clean_mouse() {

	lost = 0, won = 0;
	left_click = 0;
	first_click = 0;
	right_click = 0;
	li = 0, lj = 0;

}
