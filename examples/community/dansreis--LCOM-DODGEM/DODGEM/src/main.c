#include <stdio.h>
#include "DODGEM.h"
#include "graphics.h"
#include "utilities.h"
#include "vbe.h"
#include "bitmap.h"

#include <stdbool.h>





int main(int argc, char **argv) {
	srand(time(NULL));
	sef_startup();

	int MainOption;

	start_DODGEM();
	do
	{
		//test123(); // falta fazer uma função para checkar a opção escolhida!

		MainOption = mainMenu();

		switch(MainOption)
		{
		case 1:// Menu em que aparece o jogo!
			MainOption = gameMenu();
			break;
		case 2:// Menu em que aparecem os highscores
			MainOption = highscoreMenu();
			break;
		case 3:
		{
			MainOption = exitMenu();
			break;
		}

		}

	}while(MainOption!= 3);
	exit_DODGEM();


	return 0;
	}
