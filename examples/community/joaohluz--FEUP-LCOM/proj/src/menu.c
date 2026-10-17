#include <lcom/lcf.h>
#include <machine/int86.h> // /usr/src/include/arch/i386
#include "graphics.h"
#include "menu.h"

extern int h_res, v_res;
extern game_st game_state;

Mouse* mouseInit(){
	Mouse* mouse = (Mouse*) malloc(sizeof(Mouse));

	mouse->x = h_res/2;
	mouse->y = v_res/2;

	mouse->click = false;
	mouse->play = false;
	mouse->score = false;
	mouse->exit = false;

	return mouse;
}

void reset_mouse(Mouse* mouse){
	mouse->x = h_res/2;
	mouse->y = v_res/2;

	mouse->click = false;
	mouse->play = false;
	mouse->score = false;
	mouse->exit = false;
}

void updatePosition(Mouse* mouse, int delta_x, int delta_y){
	mouse->x += delta_x;
	mouse->y -= delta_y;

	mouse->play = false;
	mouse->score = false;
	mouse->exit = false;

	if (mouse->x + 50 > h_res)
	{
		mouse->x = h_res - 50;
	}
	if (mouse->x < 0)
	{
		mouse->x = 0;
	}
	if (mouse->y + 50 > v_res)
	{
		mouse->y = v_res - 50;
	}
	if (mouse->y < 0)
	{
		mouse->y = 0;
	}
	if (mouse->x >= 371 && mouse->x <= 612 && mouse->y >= 223 && mouse->y <= 303)
	{
		mouse->play = true;
		if (mouse->click)
		{
			game_state = GAME;
		}
	}
	else if (mouse->x >= 224 && mouse->x <= 737 && mouse->y >= 361 && mouse->y <= 435)
	{
		mouse->score = true;
		if (mouse->click)
		{
			game_state = SCORE;
		}
	}
	else if (mouse->x >= 412 && mouse->x <= 581 && mouse->y >= 510 && mouse->y <= 585)
	{
		mouse->exit = true;
		if (mouse->click)
		{
			game_state = COMP;
		}
	}
}

void deleteMouse(Mouse* mouse){
	free(mouse);
}

