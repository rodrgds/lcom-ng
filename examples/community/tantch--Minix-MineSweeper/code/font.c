#include "font.h"

//vg_draw_line(x, y, x, y, color);

void draw_0(unsigned long x, unsigned long y, int color) {
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x + 5, y - 10, x + 5, y + 10, color);
	vg_draw_line(x - 4, y + 9, x + 4, y - 9, color);
}
void draw_1(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x + 5, y - 10, x + 5, y + 10, color);

}
void draw_2(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x + 5, y - 10, x + 5, y, color);
	vg_draw_line(x - 5, y, x - 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y, x + 5, y, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);
}
void draw_3(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x + 5, y - 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y, x + 5, y, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);

}
void draw_4(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x + 3, y - 10, x + 3, y + 10, color);
	vg_draw_line(x - 5, y + 4, x + 5, y + 4, color);
	vg_draw_line(x - 5, y + 4, x + 2, y - 9, color);

}
void draw_5(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x + 5, y, x + 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x - 5, y, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y, x + 5, y, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);

}
void draw_6(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x + 5, y, x + 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y, x + 5, y, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);

}
void draw_7(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 3, y + 10, x + 5, y - 11, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);

}
void draw_8(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x + 5, y - 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y, x + 5, y, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);

}

void draw_9(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x + 5, y - 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x - 5, y, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y, x + 5, y, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);
}
void draw_A(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 3, y + 2, x + 3, y + 2, color);
	vg_draw_line(x - 5, y + 10, x - 1, y - 10, color);
	vg_draw_line(x, y - 10, x + 5, y + 10, color);
}
void draw_B(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x + 2, y - 10, x + 2, y, color);
	vg_draw_line(x + 5, y, x + 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 2, y - 10, color);
	vg_draw_line(x - 5, y, x + 5, y, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);

}
void draw_C(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);

}
void draw_D(unsigned long x, unsigned long y, int color) {
	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	set_pixel(x - 4, y - 10, color);
	set_pixel(x - 3, y - 10, color);
	set_pixel(x - 2, y - 9, color);
	set_pixel(x - 1, y - 9, color);
	set_pixel(x, y - 8, color);
	set_pixel(x + 1, y - 8, color);
	set_pixel(x + 2, y - 7, color);
	set_pixel(x + 3, y - 6, color);
	set_pixel(x + 4, y - 5, color);
	set_pixel(x - 4, y + 10, color);
	set_pixel(x - 3, y + 10, color);
	set_pixel(x - 2, y + 9, color);
	set_pixel(x - 1, y + 9, color);
	set_pixel(x, y - 8, color);
	set_pixel(x + 1, y + 8, color);
	set_pixel(x + 2, y + 7, color);
	set_pixel(x + 3, y + 6, color);
	set_pixel(x + 4, y + 5, color);
	vg_draw_line(x + 5, y - 4, x + 5, y + 4, color);

}
void draw_E(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y, x + 5, y, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);
}
void draw_F(unsigned long x, unsigned long y, int color) {
	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y, x + 5, y, color);

}
void draw_G(unsigned long x, unsigned long y, int color) {
	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);
	vg_draw_line(x + 5, y, x + 5, y + 10, color);
	vg_draw_line(x, y, x + 5, y, color);

}
void draw_H(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x + 5, y - 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y, x + 5, y, color);

}
void draw_I(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x, y - 10, x, y + 10, color);

	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);

}
void draw_J(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x + 5, y - 10, x + 5, y + 10, color);

	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y + 3, x - 5, y + 10, color);

}
void draw_K(unsigned long x, unsigned long y, int color) {
	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x - 5, y, x + 5, y - 10, color);
	vg_draw_line(x - 5, y, x + 5, y + 10, color);

}
void draw_L(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);

}
void draw_M(unsigned long x, unsigned long y, int color) {
	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x + 5, y - 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x, y, color);
	vg_draw_line(x, y, x + 5, y - 10, color);

}
void draw_N(unsigned long x, unsigned long y, int color) {
	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x + 5, y - 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y + 10, color);

}
void draw_O(unsigned long x, unsigned long y, int color) {
	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x + 5, y - 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);

}
void draw_P(unsigned long x, unsigned long y, int color) {
	vg_draw_line(x - 5, y + 10, x - 5, y - 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x + 5, y - 10, x + 5, y, color);
	vg_draw_line(x - 5, y, x + 5, y, color);
}
void draw_Q(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x + 5, y - 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);
	vg_draw_line(x + 1, y + 7, x + 5, y + 10, color);
}
void draw_R(unsigned long x, unsigned long y, int color) {
	vg_draw_line(x - 5, y - 10, x - 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x + 5, y - 10, x + 5, y, color);
	vg_draw_line(x - 5, y, x + 5, y, color);
	vg_draw_line(x - 5, y, x + 5, y + 10, color);
}
void draw_S(unsigned long x, unsigned long y, int color) {
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y, x + 5, y, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y - 10, x - 5, y, color);
	vg_draw_line(x + 5, y, x + 5, y + 10, color);
}
void draw_T(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x, y + 10, x, y - 10, color);
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);

}
void draw_U(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y + 10, x - 5, y - 10, color);
	vg_draw_line(x + 5, y + 10, x + 5, y - 10, color);
}
void draw_V(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 5, y - 10, x, y + 10, color);
	vg_draw_line(x + 1, y + 10, x + 5, y - 11, color);

}
void draw_W(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 5, y - 10, x - 2, y + 10, color);
	vg_draw_line(x - 2, y + 10, x, y - 10, color);
	vg_draw_line(x, y - 10, x + 2, y + 10, color);
	vg_draw_line(x + 2, y + 10, x + 5, y - 10, color);

}
void draw_X(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 5, y - 9, x + 5, y + 11, color);
	vg_draw_line(x - 5, y + 9, x + 5, y - 11, color);

}
void draw_Y(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 5, y - 10, x, y, color);
	vg_draw_line(x, y, x + 4, y - 12, color);
	vg_draw_line(x, y, x, y + 10, color);

}
void draw_Z(unsigned long x, unsigned long y, int color) {
	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x - 5, y + 10, x + 5, y + 10, color);
	vg_draw_line(x - 5, y + 10, x + 4, y - 11, color);
}

void draw_2dots(unsigned long x, unsigned long y, int color) {
	set_pixel(x, y - 5, color);
	set_pixel(x, y - 6, color);
	set_pixel(x + 1, y - 5, color);
	set_pixel(x + 1, y - 6, color);
	set_pixel(x, y + 5, color);
	set_pixel(x, y + 6, color);
	set_pixel(x + 1, y + 5, color);
	set_pixel(x + 1, y + 6, color);
}
void draw_space(unsigned long x, unsigned long y, int color) {

}
void draw_unrecognized(unsigned long x, unsigned long y, int color) {

	vg_draw_line(x - 5, y - 10, x + 5, y - 10, color);
	vg_draw_line(x + 5, y - 10, x + 5, y, color);
	vg_draw_line(x, y, x + 5, y, color);
	vg_draw_line(x, y, x, y + 5, color);

}
void draw_char(char c, unsigned long x, unsigned long y, unsigned long color) {

	switch (c) {

	//numeros
	case '0':
		draw_0(x, y, color);
		break;
	case '1':
		draw_1(x, y, color);
		break;
	case '2':
		draw_2(x, y, color);
		break;
	case '3':
		draw_3(x, y, color);
		break;
	case '4':
		draw_4(x, y, color);
		break;
	case '5':
		draw_5(x, y, color);
		break;
	case '6':
		draw_6(x, y, color);
		break;
	case '7':
		draw_7(x, y, color);
		break;
	case '8':
		draw_8(x, y, color);
		break;
	case '9':
		draw_9(x, y, color);
		break;

		//letras

	case 'a':
		draw_A(x, y, color);
		break;
	case 'b':
		draw_B(x, y, color);
		break;
	case 'c':
		draw_C(x, y, color);
		break;
	case 'd':
		draw_D(x, y, color);
		break;
	case 'e':
		draw_E(x, y, color);
		break;
	case 'f':
		draw_F(x, y, color);
		break;
	case 'g':
		draw_G(x, y, color);
		break;
	case 'h':
		draw_H(x, y, color);
		break;
	case 'i':
		draw_I(x, y, color);
		break;
	case 'j':
		draw_J(x, y, color);
		break;
	case 'k':
		draw_K(x, y, color);
		break;
	case 'l':
		draw_L(x, y, color);
		break;
	case 'm':
		draw_M(x, y, color);
		break;
	case 'n':
		draw_N(x, y, color);
		break;
	case 'o':
		draw_O(x, y, color);
		break;
	case 'p':
		draw_P(x, y, color);
		break;
	case 'q':
		draw_Q(x, y, color);
		break;
	case 'r':
		draw_R(x, y, color);
		break;
	case 's':
		draw_S(x, y, color);
		break;
	case 't':
		draw_T(x, y, color);
		break;
	case 'u':
		draw_U(x, y, color);
		break;
	case 'v':
		draw_V(x, y, color);
		break;
	case 'w':
		draw_W(x, y, color);
		break;
	case 'x':
		draw_X(x, y, color);
		break;
	case 'y':
		draw_Y(x, y, color);
		break;
	case 'z':
		draw_Z(x, y, color);
		break;

		//simbolos

	case ':':
		draw_2dots(x, y, color);
		break;
	case ' ':
		draw_space(x, y, color);
		break;
	default:
		draw_unrecognized(x, y, color);
		break;

	}
}

void draw_string(char* c, unsigned long x, unsigned long y, unsigned long color) {

	int i = 0;
	for (i = 0; c[i] != 0; i++) {
		draw_char(c[i], x, y, color);
		x += 15;

	}
}
void draw_cp(int c,unsigned long x,unsigned long y,int color) {

	switch (c) {

	case 0:
		draw_string("0", x, y, color);
		break;
	case 1:
		draw_string("1", x, y, color);
		break;
	case 2:
		draw_string("2", x, y, color);
		break;
	case 3:
		draw_string("3", x, y, color);
		break;
	case 4:
		draw_string("4", x, y, color);
		break;
	case 5:
		draw_string("5", x, y, color);
		break;
	case 6:
		draw_string("6", x, y, color);
		break;
	case 7:
		draw_string("7", x, y, color);
		break;
	case 8:
		draw_string("8", x, y, color);
		break;
	case 9:
		draw_string("9", x, y, color);
		break;
	case 10:
		draw_string("10", x, y, color);
		break;
	case 11:
		draw_string("11", x, y, color);
		break;
	case 12:
		draw_string("12", x, y, color);
		break;
	case 13:
		draw_string("13", x, y, color);
		break;
	case 14:
		draw_string("14", x, y, color);
		break;
	case 15:
		draw_string("15", x, y, color);
		break;
	case 16:
		draw_string("16", x, y, color);
		break;
	case 17:
		draw_string("17", x, y, color);
		break;
	case 18:
		draw_string("18", x, y, color);
		break;
	case 19:
		draw_string("19", x, y, color);
		break;
	case 30:
		draw_string("30", x, y, color);
		break;
	case 31:
		draw_string("31", x, y, color);
		break;
	case 32:
		draw_string("32", x, y, color);
		break;
	case 33:
		draw_string("33", x, y, color);
		break;
	case 34:
		draw_string("34", x, y, color);
		break;
	case 35:
		draw_string("35", x, y, color);
		break;
	case 36:
		draw_string("36", x, y, color);
		break;
	case 37:
		draw_string("37", x, y, color);
		break;
	case 38:
		draw_string("38", x, y, color);
		break;
	case 39:
		draw_string("39", x, y, color);
		break;
	case 40:
		draw_string("40", x, y, color);
		break;
	case 41:
		draw_string("41", x, y, color);
		break;
	case 42:
		draw_string("42", x, y, color);
		break;
	case 43:
		draw_string("43", x, y, color);
		break;
	case 44:
		draw_string("44", x, y, color);
		break;
	case 45:
		draw_string("45", x, y, color);
		break;
	case 46:
		draw_string("46", x, y, color);
		break;
	case 47:
		draw_string("47", x, y, color);
		break;
	case 48:
		draw_string("48", x, y, color);
		break;
	case 49:
		draw_string("49", x, y, color);
	case 50:
		draw_string("50", x, y, color);
		break;
	case 51:
		draw_string("51", x, y, color);
		break;
	case 52:
		draw_string("52", x, y, color);
		break;
	case 53:
		draw_string("53", x, y, color);
		break;
	case 54:
		draw_string("54", x, y, color);
		break;
	case 55:
		draw_string("55", x, y, color);
		break;
	case 56:
		draw_string("56", x, y, color);
		break;
	case 57:
		draw_string("57", x, y, color);
		break;
	case 58:
		draw_string("58", x, y, color);
		break;
	case 60:
		draw_string("60", x, y, color);
	case 20:
		draw_string("20", x, y, color);
		break;
	case 21:
		draw_string("21", x, y, color);
		break;
	case 22:
		draw_string("22", x, y, color);
		break;
	case 23:
		draw_string("23", x, y, color);
		break;
	case 24:
		draw_string("24", x, y, color);
		break;
	case 25:
		draw_string("25", x, y, color);
		break;
	case 26:
		draw_string("26", x, y, color);
		break;
	case 27:
		draw_string("27", x, y, color);
		break;
	case 28:
		draw_string("28", x, y, color);
		break;
	case 29:
		draw_string("29", x, y, color);
	case 59:
		draw_string("59", x, y, color);
		break;
	}

}
