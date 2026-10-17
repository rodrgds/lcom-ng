#include "interface.h"
#include "core.h"
int i_drawMatrix() {
	int i, n;
	/* Draw Horizontal Lines */
	for (i = 27; i < 579; i++)
		for (n = 27; n < 579; n++)
			vg_set_pixel(i, n, 56);

	/* Draw Horizontal Lines */
	for (i = 27; i < 600; i = i + 55) {
		vg_draw_line(27, i, 579, i, 63);
		vg_draw_line(27, i + 1, 579, i + 1, 63);
		vg_draw_line(27, i + 2, 579, i + 2, 63);
	}
	/* Draw Vertical Lines */
	for (i = 27; i < 600; i = i + 55) {
		vg_draw_line(i, 27, i, 579, 63);
		vg_draw_line(i + 1, 27, i + 1, 579, 63);
		vg_draw_line(i + 2, 27, i + 2, 579, 63);
	}

	return 0;
}

int i_sideBar() {

	write_xpm(sidebar_xpm, 600, 200);

	/* Text Boxes */
	/*X*/
	vg_draw_line(727, 27, 757, 27, 63);
	vg_draw_line(727, 28, 757, 28, 63);
	vg_draw_line(727, 57, 757, 57, 63);
	vg_draw_line(727, 58, 757, 58, 63);

	vg_draw_line(727, 27, 727, 58, 63);
	vg_draw_line(728, 27, 728, 58, 63);
	vg_draw_line(756, 27, 756, 58, 63);
	vg_draw_line(757, 27, 757, 58, 63);

	/*Y*/
	vg_draw_line(727, 67, 757, 67, 63);
	vg_draw_line(727, 68, 757, 68, 63);
	vg_draw_line(727, 97, 757, 97, 63);
	vg_draw_line(727, 98, 757, 98, 63);

	vg_draw_line(727, 67, 727, 98, 63);
	vg_draw_line(728, 67, 728, 98, 63);
	vg_draw_line(756, 67, 756, 98, 63);
	vg_draw_line(757, 67, 757, 98, 63);

	/*AIRCRAFT CARRIER
	 vg_draw_line(652, 237, 682, 237, 63);
	 vg_draw_line(652, 238, 682, 238, 63);
	 vg_draw_line(652, 267, 682, 267, 63);
	 vg_draw_line(652, 268, 682, 268, 63);

	 vg_draw_line(652, 237, 652, 268, 63);
	 vg_draw_line(653, 237, 653, 268, 63);
	 vg_draw_line(681, 237, 681, 268, 63);
	 vg_draw_line(682, 237, 682, 268, 63);

	 BATTLESHIP
	 vg_draw_line(652, 317, 682, 317, 63);
	 vg_draw_line(652, 318, 682, 318, 63);
	 vg_draw_line(652, 347, 682, 347, 63);
	 vg_draw_line(652, 348, 682, 348, 63);

	 vg_draw_line(652, 317, 652, 348, 63);
	 vg_draw_line(653, 317, 653, 348, 63);
	 vg_draw_line(681, 317, 681, 348, 63);
	 vg_draw_line(682, 317, 682, 348, 63);

	 SUBMARINE
	 vg_draw_line(652, 377, 682, 377, 63);
	 vg_draw_line(652, 378, 682, 378, 63);
	 vg_draw_line(652, 407, 682, 407, 63);
	 vg_draw_line(652, 408, 682, 408, 63);

	 vg_draw_line(652, 377, 652, 408, 63);
	 vg_draw_line(653, 377, 653, 408, 63);
	 vg_draw_line(681, 377, 681, 408, 63);
	 vg_draw_line(682, 377, 682, 408, 63);

	 CRUISER
	 vg_draw_line(652, 437, 682, 437, 63);
	 vg_draw_line(652, 438, 682, 438, 63);
	 vg_draw_line(652, 467, 682, 467, 63);
	 vg_draw_line(652, 468, 682, 468, 63);

	 vg_draw_line(652, 437, 652, 468, 63);
	 vg_draw_line(653, 437, 653, 468, 63);
	 vg_draw_line(681, 437, 681, 468, 63);
	 vg_draw_line(682, 437, 682, 468, 63);

	 DESTROYER
	 vg_draw_line(652, 517, 682, 517, 63);
	 vg_draw_line(652, 518, 682, 518, 63);
	 vg_draw_line(652, 547, 682, 547, 63);
	 vg_draw_line(652, 548, 682, 548, 63);

	 vg_draw_line(652, 517, 652, 548, 63);
	 vg_draw_line(653, 517, 653, 548, 63);
	 vg_draw_line(681, 517, 681, 548, 63);
	 vg_draw_line(682, 517, 682, 548, 63);
	 */
	return 0;
}

int i_drawHit(int x, int y, int hit) {
	int i, color;
	if (hit)
		color = 36;
	else
		color = 11;
	for (i = 0; i < 52; i++) {
		vg_draw_line(30 + x * 55, i + 30 + y * 55, 82 + x * 55, i + 30 + y * 55,
				color);
	}
}

int i_printNumber(int n, int x, int y) {
	int i;
	switch (n) {
	case -1:
		for (i = 0; i < 28; i++) {
			vg_draw_line(x, y + i, x + 26, y + i, 0);
		}
		break;
	case 0:
		write_xpm(zero_xpm, x, y);
		break;
	case 1:
		write_xpm(one_xpm, x, y);
		break;
	case 2:
		write_xpm(two_xpm, x, y);
		break;
	case 3:
		write_xpm(three_xpm, x, y);
		break;
	case 4:
		write_xpm(four_xpm, x, y);
		break;
	case 5:
		write_xpm(five_xpm, x, y);
		break;
	case 6:
		write_xpm(six_xpm, x, y);
		break;
	case 7:
		write_xpm(seven_xpm, x, y);
		break;
	case 8:
		write_xpm(eight_xpm, x, y);
		break;
	case 9:
		write_xpm(nine_xpm, x, y);
		break;
	}
	return 0;
}

int i_readXY(int *x, int *y) {
	int key = 0, exit = 0;
	i_printNumber(-1, 729, 29);
	i_printNumber(-1, 729, 69);
	while (key != 28 || !exit) {
		key = readkey();
		switch (key) {
		case 1:
			return 1;
		case 2:
			i_printNumber(1, 729, 29);
			exit = 1;
			*x = 1;
			break;
		case 3:
			i_printNumber(2, 729, 29);
			exit = 1;
			*x = 2;
			break;
		case 4:
			i_printNumber(3, 729, 29);
			exit = 1;
			*x = 3;
			break;
		case 5:
			i_printNumber(4, 729, 29);
			exit = 1;
			*x = 4;
			break;
		case 6:
			i_printNumber(5, 729, 29);
			exit = 1;
			*x = 5;
			break;
		case 7:
			i_printNumber(6, 729, 29);
			exit = 1;
			*x = 6;
			break;
		case 8:
			i_printNumber(7, 729, 29);
			exit = 1;
			*x = 7;
			break;
		case 9:
			i_printNumber(8, 729, 29);
			exit = 1;
			*x = 8;
			break;
		case 10:
			i_printNumber(9, 729, 29);
			exit = 1;
			*x = 9;
			break;
		case 11:
			i_printNumber(0, 729, 29);
			exit = 1;
			*x = 0;
			break;
		case 14:
			i_printNumber(-1, 729, 29);
			exit = 0;
			break;
		}
	}
	key = 0;
	exit = 0;
	while (key != 28 || !exit) {
		key = readkey();
		switch (key) {
		case 1:
			return 1;
		case 2:
			i_printNumber(1, 729, 69);
			exit = 1;
			*y = 1;
			break;
		case 3:
			i_printNumber(2, 729, 69);
			exit = 1;
			*y = 2;
			break;
		case 4:
			i_printNumber(3, 729, 69);
			exit = 1;
			*y = 3;
			break;
		case 5:
			i_printNumber(4, 729, 69);
			exit = 1;
			*y = 4;
			break;
		case 6:
			i_printNumber(5, 729, 69);
			exit = 1;
			*y = 5;
			break;
		case 7:
			i_printNumber(6, 729, 69);
			exit = 1;
			*y = 6;
			break;
		case 8:
			i_printNumber(7, 729, 69);
			exit = 1;
			*y = 7;
			break;
		case 9:
			i_printNumber(8, 729, 69);
			exit = 1;
			*y = 8;
			break;
		case 10:
			i_printNumber(9, 729, 69);
			exit = 1;
			*y = 9;
			break;
		case 11:
			i_printNumber(0, 729, 69);
			exit = 1;
			*y = 0;
			break;
		case 14:
			i_printNumber(-1, 729, 69);
			exit = 0;
			break;
		}
	}
	return 0;
}

int menu() {
	int key = 0, selected = 1;
	write_xpm(battleship1_xpm, 0, 0);
	while (key != 28) {
		key = readkey();
		if (key == 72) {
			if (selected == 1) {
				selected = 0;
				write_xpm(battleship2_xpm, 0, 0);
			} else {
				selected = 1;
				write_xpm(battleship1_xpm, 0, 0);
			}
		} else if (key == 80) {
			if (selected == 1) {
				selected = 0;
				write_xpm(battleship2_xpm, 0, 0);
			} else {
				selected = 1;
				write_xpm(battleship1_xpm, 0, 0);
			}
		}
	}
	return selected;
}
