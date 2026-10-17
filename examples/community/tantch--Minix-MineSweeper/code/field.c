#include "field.h"
#include "font.h"

unsigned first_x; //cordenadas do centro quadrado mais a esquerda e a cima (primeiro) [0][0]
unsigned first_y;
unsigned long graf[800 * 600];
int lado;
int flags = 0;
int flags_certas = 0;
int mines;
int flag_color = 1;
int player1=0;
int player2=0;

int vg_fill(unsigned long color) {
	int i;

	for (i = 0; i < (800 * 600); i++) {

		graf[i] = color;
	}
	return 0;
}
void set_pixel(int x, int y, unsigned long color) {

	if (x < 800 && y < 600)
		graf[x + y * 800] = color;
}

void draw_mouse(int topx, int topy, unsigned long color) {

	int xi = topx, yi = topy, xf = topx, yf = topy;
	int i = 0;

	for (i = 0; i < 13; i++) {
		vg_draw_line(xi, yi + i, xf + i, yf + i, color);

	}
	vg_draw_line(xi, yi + 12, xf, yf + 17, color);
	for (i = 1; i < 5; i++) {
		vg_draw_line(xi, yi + 17, xf + i, yf + 12, color);
	}
	for (i = 0; i < 2; i++) {
		vg_draw_line(xi + 5 + i, yi + 12, xf + 8 + i, yf + 21, color);
	}
	vg_draw_line(xi, yi, xf, yf + 17, 0);
	vg_draw_line(xi, yi + 17, xf + 4, yf + 12, 0);
	vg_draw_line(xi, yi, xf + 12, yf + 12, 0);
	vg_draw_line(xi + 7, yi + 12, xf + 12, yf + 12, 0);
	vg_draw_line(xi + 4, yi + 13, xf + 7, yf + 21, 0);
	vg_draw_line(xi + 7, yi + 13, xf + 10, yf + 21, 0);
	vg_draw_line(xi + 8, yi + 21, xi + 9, yf + 21, 0);
}

void draw_grid_easy(int first_click, Campo_t field[][getLado()]) {
	int quad = getLado();
	unsigned long xi = 235, yi = 120, xf = 235, yf = 120;
	int i, j, largura, altura, m, n;
	largura = quad * 29 + (quad - 1) * 2 + 10;
	altura = quad * 29 + (quad - 1) * 2 + 10;

	first_x = xi + 5 + 15;
	first_y = yi + 5 + 15;

	for (i = 0; i <= altura; i++) {
		vg_draw_line(xi, yi + i, xf + largura, yf + i, 7);
	}
	for (n = 0; n < quad; n++) {
		for (m = 0; m < quad; m++) {
			int c_x = getx(n);
			int c_y = gety(m);
			for (i = 0; i <= 28; i++) {

				vg_draw_line(c_x - 14, c_y - 14 + i, c_x + 14, c_y - 14 + i,
						56);
			}

		}
	}

	if (first_click) {
		for (i = 0; i < getLado(); i++) {
			for (j = 0; j < getLado(); j++) {

				if (field[i][j].revealed == 1) {
					if (field[i][j].mine == 1)
						if (field[i][j].flag == 1)
							draw_mine(i, j, 1);
						else
							draw_mine(i, j, 0);
					else
						draw_number(i, j, field[i][j].number);
				} else if (field[i][j].flag == 1)
					draw_flag(i, j, flag_color);
			}
		}
	}

}

void draw_mine(int row, int col, int color) {
	unsigned long c_x = getx(col);
	unsigned long c_y = gety(row);
	int i;
	square_clicked(c_x, c_y);
	vg_draw_line(c_x - 13, c_y, c_x - 9, c_y, color);
	vg_draw_line(c_x, c_y - 13, c_x, c_y - 9, color); //Linhas da cruz
	vg_draw_line(c_x + 9, c_y, c_x + 13, c_y, color);
	vg_draw_line(c_x, c_y + 9, c_x, c_y + 13, color);

	vg_draw_line(c_x - 10, c_y - 10, c_x - 7, c_y - 7, color);
	vg_draw_line(c_x + 7, c_y - 7, c_x + 10, c_y - 10, color); //linhas diagonais da cruz
	vg_draw_line(c_x - 10, c_y + 10, c_x - 7, c_y + 7, color);
	vg_draw_line(c_x + 7, c_y + 7, c_x + 10, c_y + 10, color);

	vg_draw_line(c_x - 8, c_y - 2, c_x - 8, c_y + 2, color);
	vg_draw_line(c_x + 8, c_y - 2, c_x + 8, c_y + 2, color);
	vg_draw_line(c_x - 7, c_y + 2, c_x - 7, c_y + 4, color);
	vg_draw_line(c_x - 7, c_y - 2, c_x - 7, c_y - 4, color);
	vg_draw_line(c_x + 7, c_y + 2, c_x + 7, c_y + 4, color); //linhas verticais do contorno
	vg_draw_line(c_x + 7, c_y - 2, c_x + 7, c_y - 4, color);
	vg_draw_line(c_x - 6, c_y + 4, c_x - 6, c_y + 6, color);
	vg_draw_line(c_x - 6, c_y - 4, c_x - 6, c_y - 6, color);
	vg_draw_line(c_x + 6, c_y + 4, c_x + 6, c_y + 6, color);
	vg_draw_line(c_x + 6, c_y - 4, c_x + 6, c_y - 6, color);

	vg_draw_line(c_x - 2, c_y - 8, c_x + 2, c_y - 8, color);
	vg_draw_line(c_x - 2, c_y + 8, c_x + 2, c_y + 8, color); //linhas horizontais do contorno
	vg_draw_line(c_x + 2, c_y + 7, c_x + 4, c_y + 7, color);
	vg_draw_line(c_x + 2, c_y - 7, c_x + 4, c_y - 7, color);
	vg_draw_line(c_x - 4, c_y + 7, c_x - 2, c_y + 7, color);
	vg_draw_line(c_x - 4, c_y - 7, c_x - 2, c_y - 7, color);
	vg_draw_line(c_x + 4, c_y + 6, c_x + 6, c_y + 6, color);
	vg_draw_line(c_x + 4, c_y - 6, c_x + 6, c_y - 6, color);
	vg_draw_line(c_x - 6, c_y + 6, c_x - 4, c_y + 6, color);
	vg_draw_line(c_x - 6, c_y - 6, c_x - 4, c_y - 6, color);

	for (i = 0; i < 11; i++) {
		vg_draw_line(c_x - 5, c_y - 5 + i, c_x + 5, c_y - 5 + i, color);
	}
	vg_draw_line(c_x - 4, c_y - 6, c_x + 4, c_y - 6, color);
	vg_draw_line(c_x - 4, c_y + 6, c_x + 4, c_y + 6, color);
	vg_draw_line(c_x - 3, c_y - 7, c_x + 3, c_y - 7, color);
	vg_draw_line(c_x - 3, c_y + 7, c_x + 3, c_y + 7, color);
	vg_draw_line(c_x - 6, c_y - 4, c_x - 6, c_y + 4, color);
	vg_draw_line(c_x + 6, c_y - 4, c_x + 6, c_y + 4, color);
	vg_draw_line(c_x - 7, c_y - 3, c_x - 7, c_y + 3, color);
	vg_draw_line(c_x + 7, c_y - 3, c_x + 7, c_y + 3, color);

	set_pixel(c_x, c_y, 4); //pinta no meio
}

void draw_flag(int row, int col, unsigned long color) {

	unsigned long c_x = getx(col);
	unsigned long c_y = gety(row);
	int i;
	vg_draw_line(c_x - 7, c_y + 13, c_x + 8, c_y + 13, 0);
	vg_draw_line(c_x - 7, c_y + 12, c_x + 8, c_y + 12, 0);
	vg_draw_line(c_x - 7, c_y + 11, c_x + 8, c_y + 11, 0);
	vg_draw_line(c_x - 4, c_y + 9, c_x + 5, c_y + 9, 0);
	vg_draw_line(c_x - 4, c_y + 10, c_x + 5, c_y + 10, 0);

	vg_draw_line(c_x, c_y - 10, c_x, c_y + 8, 63);
	vg_draw_line(c_x + 1, c_y - 10, c_x + 1, c_y + 8, 63);

	for (i = 0; i < 6; i++) {
		vg_draw_line(c_x + 2 + i, c_y - 10 + i, c_x + 2 + i, c_y + 2 - i,
				color);
	}
	set_pixel(c_x + 8, c_y - 4, color);

}
void draw_number(int row, int col, int number) {
	unsigned long c_x = getx(col);
	unsigned long c_y = gety(row);
	int color;
	int i;
//color = rand() % 63;
	square_clicked(c_x, c_y);
	switch (number) {
	case 0:

		break;
	case 1:
		color = 51;

		vg_draw_line(c_x - 6, c_y + 9, c_x + 8, c_y + 9, color);
		vg_draw_line(c_x - 6, c_y + 10, c_x + 8, c_y + 10, color);
		vg_draw_line(c_x - 6, c_y + 11, c_x + 8, c_y + 11, color);
		vg_draw_line(c_x, c_y - 11, c_x, c_y + 9, color);
		vg_draw_line(c_x + 1, c_y - 11, c_x + 1, c_y + 9, color);
		vg_draw_line(c_x + 2, c_y - 11, c_x + 2, c_y + 9, color);

		vg_draw_line(c_x - 7, c_y - 5, c_x - 1, c_y - 11, color);
		vg_draw_line(c_x - 7, c_y - 4, c_x - 1, c_y - 10, color);
		vg_draw_line(c_x - 7, c_y - 3, c_x - 1, c_y - 9, color);
		vg_draw_line(c_x - 6, c_y - 3, c_x - 1, c_y - 8, color);
		vg_draw_line(c_x - 5, c_y - 3, c_x - 1, c_y - 7, color);
		break;
	case 2: //-10 6
		color = 22;

		c_y--;
		for (i = 0; i < 4; i++) {
			vg_draw_line(c_x - 10 + i, c_y + 13, c_x + 6 + i, c_y - 3, color);
		}
		vg_draw_line(c_x - 9, c_y + 13, c_x + 9, c_y + 13, color);
		vg_draw_line(c_x - 9, c_y + 12, c_x + 9, c_y + 12, color);
		vg_draw_line(c_x + 6, c_y - 4, c_x + 9, c_y - 4, color);
		vg_draw_line(c_x + 6, c_y - 5, c_x + 9, c_y - 5, color);
		vg_draw_line(c_x + 5, c_y - 6, c_x + 8, c_y - 6, color);
		vg_draw_line(c_x + 5, c_y - 7, c_x + 7, c_y - 7, color);
		vg_draw_line(c_x + 4, c_y - 8, c_x + 6, c_y - 8, color);
		vg_draw_line(c_x + 3, c_y - 9, c_x + 5, c_y - 9, color);
		vg_draw_line(c_x - 3, c_y - 10, c_x + 4, c_y - 10, color);
		vg_draw_line(c_x - 2, c_y - 11, c_x + 3, c_y - 11, color);
		vg_draw_line(c_x - 5, c_y - 9, c_x - 2, c_y - 9, color);
		vg_draw_line(c_x - 6, c_y - 8, c_x - 4, c_y - 8, color);
		vg_draw_line(c_x - 7, c_y - 7, c_x - 5, c_y - 7, color);
		vg_draw_line(c_x - 8, c_y - 6, c_x - 6, c_y - 6, color);
		vg_draw_line(c_x - 9, c_y - 5, c_x - 7, c_y - 5, color);
		break;

	case 3:
		color = 27;

		vg_draw_line(c_x, c_y, c_x + 5, c_y, color);
		vg_draw_line(c_x, c_y + 1, c_x + 5, c_y + 1, color);
		vg_draw_line(c_x + 1, c_y - 1, c_x + 6, c_y - 1, color);
		vg_draw_line(c_x + 1, c_y + 2, c_x + 6, c_y + 2, color);
		vg_draw_line(c_x + 2, c_y - 2, c_x + 7, c_y - 2, color);
		vg_draw_line(c_x + 2, c_y + 3, c_x + 7, c_y + 3, color);
		vg_draw_line(c_x + 3, c_y - 3, c_x + 8, c_y - 3, color);
		vg_draw_line(c_x + 3, c_y + 4, c_x + 8, c_y + 4, color);
		vg_draw_line(c_x + 4, c_y - 4, c_x + 9, c_y - 4, color);
		vg_draw_line(c_x + 4, c_y + 5, c_x + 9, c_y + 5, color);
		vg_draw_line(c_x + 5, c_y - 5, c_x + 10, c_y - 5, color);
		vg_draw_line(c_x + 5, c_y + 6, c_x + 10, c_y + 6, color);
		vg_draw_line(c_x + 5, c_y - 6, c_x + 10, c_y - 6, color);
		vg_draw_line(c_x + 5, c_y + 7, c_x + 10, c_y + 7, color);
		vg_draw_line(c_x + 4, c_y - 7, c_x + 9, c_y - 7, color);
		vg_draw_line(c_x + 4, c_y + 8, c_x + 9, c_y + 8, color);
		vg_draw_line(c_x + 3, c_y - 8, c_x + 8, c_y - 8, color);
		vg_draw_line(c_x + 3, c_y + 9, c_x + 8, c_y + 9, color);
		vg_draw_line(c_x - 9, c_y - 7, c_x - 4, c_y - 7, color);
		vg_draw_line(c_x - 9, c_y + 8, c_x - 4, c_y + 8, color);
		vg_draw_line(c_x - 8, c_y - 8, c_x - 3, c_y - 8, color);
		vg_draw_line(c_x - 8, c_y + 9, c_x - 3, c_y + 9, color);
		vg_draw_line(c_x - 7, c_y - 9, c_x + 7, c_y - 9, color);
		vg_draw_line(c_x - 7, c_y + 10, c_x + 7, c_y + 10, color);
		vg_draw_line(c_x - 5, c_y - 10, c_x + 5, c_y - 10, color);
		vg_draw_line(c_x - 5, c_y + 11, c_x + 5, c_y + 11, color);
		vg_draw_line(c_x - 3, c_y - 11, c_x + 3, c_y - 11, color);
		vg_draw_line(c_x - 3, c_y + 12, c_x + 3, c_y + 12, color);
		break;
	case 4:
		color = 13;

		vg_draw_line(c_x - 9, c_y + 3, c_x + 8, c_y + 3, color);
		vg_draw_line(c_x - 9, c_y + 4, c_x + 8, c_y + 4, color);
		vg_draw_line(c_x - 9, c_y + 5, c_x + 8, c_y + 5, color);

		vg_draw_line(c_x + 3, c_y + 11, c_x + 3, c_y - 9, color);
		vg_draw_line(c_x + 4, c_y + 11, c_x + 4, c_y - 9, color);
		vg_draw_line(c_x + 5, c_y + 11, c_x + 5, c_y - 9, color);

		vg_draw_line(c_x - 5, c_y + 2, c_x + 2, c_y - 5, color);
		vg_draw_line(c_x - 6, c_y + 2, c_x + 2, c_y - 6, color);
		vg_draw_line(c_x - 7, c_y + 2, c_x + 2, c_y - 7, color);
		vg_draw_line(c_x - 8, c_y + 2, c_x + 2, c_y - 8, color);
		vg_draw_line(c_x - 9, c_y + 2, c_x + 2, c_y - 9, color);
		break;

	case 5:
		color = 19;

		vg_draw_line(c_x - 8, c_y - 11, c_x + 8, c_y - 11, color);
		vg_draw_line(c_x - 8, c_y - 10, c_x + 8, c_y - 10, color);
		vg_draw_line(c_x - 8, c_y - 9, c_x + 8, c_y - 9, color);

		vg_draw_line(c_x - 8, c_y - 8, c_x - 8, c_y, color);
		vg_draw_line(c_x - 7, c_y - 8, c_x - 7, c_y, color);
		vg_draw_line(c_x - 6, c_y - 8, c_x - 6, c_y, color);

		vg_draw_line(c_x - 5, c_y - 2, c_x + 5, c_y - 2, color);
		vg_draw_line(c_x - 5, c_y - 1, c_x + 5, c_y - 1, color);
		vg_draw_line(c_x - 5, c_y, c_x + 5, c_y, color);

		vg_draw_line(c_x - 6, c_y + 8, c_x + 5, c_y + 8, color);
		vg_draw_line(c_x - 6, c_y + 9, c_x + 5, c_y + 9, color);
		vg_draw_line(c_x - 6, c_y + 10, c_x + 5, c_y + 10, color);

		vg_draw_line(c_x + 6, c_y - 2, c_x + 6, c_y + 10, color);
		vg_draw_line(c_x + 7, c_y - 1, c_x + 7, c_y + 9, color);
		vg_draw_line(c_x + 8, c_y, c_x + 8, c_y + 8, color);
		vg_draw_line(c_x + 9, c_y + 2, c_x + 9, c_y + 6, color);
		break;
	default:
		break;

	}
}

void square_clicked(unsigned long c_x, unsigned long c_y) {

	int i;

	for (i = 0; i < 27; i++) {
		vg_draw_line(c_x - 13, c_y - 13 + i, c_x + 13, c_y - 13 + i, 7);
	}

}

void draw_lose(int i, int j) {

	int k, l;
	int x = getx(j);
	int y = gety(i);
//draw_mine(x, y);
	vg_draw_line(x - 13, y - 13, x + 13, y + 13, 4);
	vg_draw_line(x - 13, y + 13, x + 13, y - 13, 4);

}

int getrow(int y) {

	int ini = gety(0) - 13;
	int lmt = ini + 29 + 2;
	int i;

	for (i = 0; i < getLado(); i++) {
		if (y >= (ini + 31 * i) && y <= (lmt + 31 * i))
			return i;

	}

}
int getcol(int x) {

	int ini = getx(0) - 13;
	int lmt = ini + 29 + 2;
	int i;

	for (i = 0; i < getLado(); i++) {
		if (x >= (ini + 31 * i) && x <= (lmt + 31 * i))
			return i;

	}

}

int getLado() {
	return lado;
}
void setLado(int ld) {
	lado = ld;
}
int getMines() {
	return mines;
}
void setMines(int mn) {
	mines = mn;
}

unsigned getx(int col) {
	return first_x + 31 * col;
}
unsigned gety(int row) {
	return first_y + 31 * row;
}
void update_graf() {
	unsigned long i;
	for (i = 0; i < 480000; i++) {

		vg_set_pixel(i, graf[i]);

	}
}
int getFlags() {
	return flags;
}

void setFlags(int n) {

	flags = n;

}
void incFlags() {
	flags++;
}

int getFlagsCertas() {
	return flags_certas;
}

void setFlagsCertas(int n) {

	flags_certas = n;

}
void incFlagsCertas() {
	flags_certas++;
}
void decFlagsCertas() {
	flags_certas--;
}
void decFlags() {
	flags--;
}
int getFlagColor() {
	return flag_color;
}

void setFlagColor(int n) {

	flag_color = n;

}

void reset(Campo_t field[][getLado()]) {

	int i, j;
	for (i = 0; i < lado; i++) {
		for (j = 0; j < lado; j++) {
			field[i][j].revealed = 0;
			field[i][j].mine = 0;
			field[i][j].flag = 0;
			field[i][j].number = 0;
		}
	}

	flags = 0;
	flags_certas = 0;

	if (getPlayer() == 2) {
		changePlayer();
	}

	clean_mouse();

	printf("minas: %d\n", mines);

}

void draw_menu(int option) {


	int xc = 250;
	int yc = 100;
	int i;
	int clr = 7;
	if (option == 1)
		clr = 34;
	for (i = 0; i < 80; i++)
		vg_draw_line(xc, yc + i, xc + 300, yc + i, clr);
	draw_string("single player", xc + 50, yc + 40, 63);
	clr = 7;

	yc += 100;
	if (option == 2)
		clr = 34;
	for (i = 0; i < 80; i++)
		vg_draw_line(xc, yc + i, xc + 300, yc + i, clr);
	draw_string("multiplayer: host", xc + 50, yc + 40, 63);
	clr = 7;

	yc += 100;
	if (option == 3)
		clr = 34;
	for (i = 0; i < 80; i++)
		vg_draw_line(xc, yc + i, xc + 300, yc + i, clr);
	draw_string("multiplayer: join", xc + 50, yc + 40, 63);
	clr = 7;

	yc += 100;
	if (option == 4)
		clr = 34;
	for (i = 0; i < 80; i++)
		vg_draw_line(xc, yc + i, xc + 300, yc + i, clr);
	draw_string("exit", xc + 50, yc + 40, 63);

}
int getPlayer1() {
	return player1;
}


void incPlayer1() {
	player1++;
}
int getPlayer2() {
	return player2;
}


void incPlayer2() {
	player2++;
}
