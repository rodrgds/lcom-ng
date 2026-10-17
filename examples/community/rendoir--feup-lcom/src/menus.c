/**
	@file menus.c
	@author Daniel Marques
	@author João Almeida

	Contains only a single function which is the menuHandler.
*/


#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "keyboard.h"
#include "graphics.h"
#include "menus.h"
#include "game.h"

unsigned int menuHandler(options_t *current_option, unsigned long int *key , unsigned char *enter_pressed	){


	if ( MAKE_ENTER == *key ){
		*enter_pressed = 1;
		return 2;
	}
	else if ( BREAK_ENTER == *key && 1 == *enter_pressed){
		*enter_pressed = 0;
		return (*current_option == START_GAME) ? 1 : 0;
	}
	else if ( BREAK_UP == *key) {
		if ( START_GAME == *current_option )
			*current_option = EXIT_GAME;
		else if ( EXIT_GAME == *current_option)
			*current_option = START_GAME;

	} else if ( BREAK_DOWN == *key) {
		if ( START_GAME == *current_option)
			*current_option = EXIT_GAME;
		else if (EXIT_GAME == *current_option )
			*current_option = START_GAME;
	}
	return 2;
}
