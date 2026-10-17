#include <minix/drivers.h>
#include "game.h"

int main(int argc, char **argv)
{
	/**
	 *  @mainpage MOsu Index Page
	 *
	 *  @section intro_sec Introduction
	 *
	 *  This is the documentation for the project "MOsu", developed in the classes of LCOM.
	 *
	 *  @section usage_sec Usage
	 *
	 *  Choose one of the options on the screen using the mouse and left click.
	 *  When in-game, use the mouse to point and, if it's easy mode, click any key from the keyboard, or, if it's
	 *  hard mode, click the corresponding key from the keyboard.
	 *
	 *  @section done_Sec Work done by
	 *
	 *  Pedro Pinho and Miguel Teixeira.
	 */

	sef_startup();

	sys_enable_iop(SELF);

	char txtfile[200];
	strcpy(txtfile, argv[0]);
	char *filename = strrchr(txtfile, '/');
	strcpy(filename ? filename + 1 : txtfile, "highscores.txt");

	init_game(txtfile);

	return 0;
}
