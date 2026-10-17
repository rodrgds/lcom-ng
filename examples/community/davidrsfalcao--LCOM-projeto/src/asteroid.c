#include "asteroid.h"
#include "vbe.h"
#include <stdlib.h>

extern unsigned long points;
extern unsigned long global_counter;

void draw_asteroids(asteroid ast1, asteroid ast2)
{
	if (ast1.y > 0)
		xpm_cre(&ast1.height, &ast1.width, ast1.x, ast1.y, ast1.sprite);

	if (ast2.y > 0)
		xpm_cre(&ast2.height, &ast2.width, ast2.x, ast2.y, ast2.sprite);




}

void move_asteroid(asteroid *ast)
{
	if ((*ast).y < 650)
	{
		(*ast).y += ((points/4)+1);
	}
	else
		{
		(*ast).y = 0;
		points++;
		}

}

void asteroid_generator(asteroid *ast1, asteroid *ast2)
{

	if (global_counter % 1000 == 0)
	{
		srand(getpid());
	}


	if (global_counter == 50)
	{
		int v = rand() % 898;
		(*ast1).x = v;
		move_asteroid(ast1);
	}

	if (global_counter > 50)
	{
		if (ast1->y > 0)
			move_asteroid(ast1);
	}


	if (global_counter == 150) {

		int v = 898-(rand() % 898);
		(*ast2).x = v;
		move_asteroid(ast2);
	}

	if (global_counter > 150) {

		if (ast2->y > 0)
			move_asteroid(ast2);
	}

	if ((global_counter > 150) && ((*ast1).y == 0) && ((*ast2).y == 0) )
	{
		global_counter = 0;
	}

}
