#include "core.h"
#include "battleship.h"
#include <stdio.h>

int main(void) {
	struct game a;
	int key, x, y, quit;
	sef_startup();
	aloc_xpm();
	vg_init(0x103);
	while (menu()) {
		key = 0;
		attempts = 0;
		quit = 0;
		game_build(&a);
		write_xpm(map_xpm, 0, 0);
		i_drawMatrix();
		i_sideBar();
		write_xpm(x_xpm, 687, 27);
		write_xpm(y_xpm, 687, 67);
		i_printNumber(5, 654, 239);
		i_printNumber(4, 654, 319);
		i_printNumber(3, 654, 379);
		i_printNumber(3, 654, 439);
		i_printNumber(2, 654, 519);
		while (!game_isOver(&a)) {
			if (i_readXY(&x, &y)) {
				quit = 1;
				break;
			}
			game_hit(&a, x, y);
		}
		if (!quit) {
			write_xpm(won_xpm, 0, 0);
			if (attempts == 100) {
				i_printNumber(1, 342, 366);
				i_printNumber(0, 372, 366);
				i_printNumber(0, 402, 366);
			} else {
				i_printNumber(attempts / 10, 342, 366);
				i_printNumber(attempts % 10, 372, 366);
			}
			while (key != 28) {
				key = readkey();
			}
		}
	}
	vg_exit();

	return 0;
}
