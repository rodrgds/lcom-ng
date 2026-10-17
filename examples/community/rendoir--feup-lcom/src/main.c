/**
	@file main.c
	@author Daniel Marques
	@author João Almeida

	File that contains the main function.
	Bits of the interrupts are defined here.
*/

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "vbe.h"
#include "mouse.h"
#include "graphics.h"
#include "math.h"
#include "keyboard.h"
#include "menus.h"
#include "game.h"
#include "rtc.h"
#include "files.h"

int main(int argc, char **argv) {
	sef_startup();
	if (argc == 1) {/* Prints usage of the program if no arguments are passed */
		printf("Use the following: \n service run `pwd`/proj -args \"< path to current directory >\" \n");
		return 0;
	}
	else if (argc == 2){
		sys_enable_iop(SELF);
		srand(time(NULL));
		game_t game;

		unsigned int TIMER_BIT = 0, KBD_BIT = 1, MOUSE_BIT = 2;

		RTC_SetBinary();

		vgInit(GRAPHICS_MODE);

		game.current_dir = argv[1];
		if(setGame(&game,1)){
			vgExit();
			printf("Program Exited! \n");
			return 1;
		}
		runGame(&game, TIMER_BIT, MOUSE_BIT, KBD_BIT);

		vgExit();
		return 0;
	}
	else{
		printf("Wrong number of arguments! \n");
	}

	return 0;
}
