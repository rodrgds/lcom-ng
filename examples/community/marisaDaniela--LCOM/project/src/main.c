#include <stdio.h>
#include "game.h"
#include "snake.h"
#include "utils.h"

int main(int argc, char **argv)
{

	printf("Entering Game\n");
	srand(time(NULL));
	return game();
}
