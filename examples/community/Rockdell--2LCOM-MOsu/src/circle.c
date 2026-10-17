#include <minix/drivers.h>
#include <math.h>
#include "circle.h"
#include "video_gr.h"

#include "pixmap.h"
#include "i8042.h"

//Last circle's oordinates
static unsigned int last_x = 0, last_y = 0;

Circle * create_circle(int mode) {

	Circle * c = (Circle *) malloc (sizeof(Circle));

	if(c == NULL)
		return NULL;

	//Possible x and y
	unsigned int rand_x;
	unsigned int rand_y;

	new_xy:
	rand_x = rand() % 800 + 100;
	rand_y = rand() % 450 + 100;

	//Hipotenusa
	unsigned int dx = abs(rand_x - last_x);
	unsigned int dy = abs(rand_y - last_y);

	dx *= dx;
	dy *= dy;

	//Test colision
	if(dx + dy > 60*60) {
		//Circle's x and y
		c->x = rand_x;
		c->y = rand_y;
	}
	else
		goto new_xy;

	//Update static variables
	last_x = c->x;
	last_y = c->y;

	//Time the circle is gonna be up (60 frames at 60Hz = 1 second)
	c->t = 60;

	//Radius of the circle and outer circle
	c->r = 60;
	c->out_r = 2 * c->t + 4;

	//Circle's color
	c->c = 63;

	//Hard mode (easy mode has no letters)
	if (mode) {

		//Create random number representing a letter
		unsigned int rand_l = rand() % 4;

		//Assign circle's letter code
		switch (rand_l) {
		case 0:
			c->l = Q;
			break;
		case 1:
			c->l = W;
			break;
		case 2:
			c->l = E;
			break;
		case 3:
			c->l = R;
			break;
		}
	}

	draw_circle(c);

	return c;
}

Circle * menu_create_circle() {

	Circle * c = (Circle *) malloc (sizeof(Circle));

	if(c == NULL)
		return NULL;

	//Possible x and y
	unsigned int rand_x;
	unsigned int rand_y;

	new_xy:
	rand_x = rand() % 800 + 100;
	rand_y = rand() % 450 + 100;

	//Hipotenusa
	unsigned int dx = abs(rand_x - last_x);
	unsigned int dy = abs(rand_y - last_y);

	dx *= dx;
	dy *= dy;

	//Test colision
	if(dx + dy > 60*60) {
		//Circle's x and y
		c->x = rand_x;
		c->y = rand_y;
	}
	else
		goto new_xy;

	//Update static variables
	last_x = c->x;
	last_y = c->y;

	//No need for the circle's time
	c->t = 0;

	//Radius of the circle
	unsigned int rand_r = rand() % 130 + 30;
	c->r = rand_r;

	//For the menu circles we don't need this variable
	c->out_r = 0;

	//Circle's color
	unsigned int rand_c;

	other:
	rand_c = rand() % 65 + 1;

	if(rand_c == 35)
		goto other;

	c->c = rand_c;

	draw_menu_circle(c);

	return c;
}

void draw_circle(Circle * c) {

	//Draw main circle
	vg_circle(c->x, c->y, c->r, c->c);
	vg_circle(c->x, c->y, c->r + 1, c->c);
	vg_circle(c->x, c->y, c->r + 2, c->c);

	//Draw outer circle
	vg_circle(c->x, c->y, c->out_r, c->c);

	//Draws letter -> needs -24/-22/-19 to adjust to the center of the circle
	switch (c->l) {
	case Q:
		vg_sprite(letterQ, c->x - 24, c->y - 22, 1);
		break;
	case W:
		vg_sprite(letterW, c->x - 24, c->y - 19, 1);
		break;
	case E:
		vg_sprite(letterE, c->x - 24, c->y - 19, 1);
		break;
	case R:
		vg_sprite(letterR, c->x - 24, c->y - 19, 1);
		break;
	case Z:
		vg_sprite(correct, c->x - 24, c->y - 19, 1);
		break;
	case X:
		vg_sprite(wrong, c->x - 24, c->y - 20, 1);
		break;
	}
}

void draw_menu_circle(Circle * c) {

	vg_circle(c->x, c->y, c->r, c->c);
	vg_circle(c->x, c->y, c->r + 1, c->c);
	vg_circle(c->x, c->y, c->r + 2, c->c);

}

void erase_circle(Circle * c) {

	//Erase main circle
	vg_circle(c->x, c->y, c->r, 0);
	vg_circle(c->x, c->y, c->r + 1, 0);
	vg_circle(c->x, c->y, c->r + 2, 0);

	vg_circle(c->x, c->y, c->out_r, 0);

	//Erases letter -> needs -24/-22/-19 to adjust to the center of the circle
	switch (c->l) {
	case Q:
		vg_sprite(letterQ, c->x - 24, c->y - 22, 0);
		break;
	case W:
		vg_sprite(letterW, c->x - 24, c->y - 19, 0);
		break;
	case E:
		vg_sprite(letterE, c->x - 24, c->y - 19, 0);
		break;
	case R:
		vg_sprite(letterR, c->x - 24, c->y - 19, 0);
		break;
	case Z:
		vg_sprite(correct, c->x - 24, c->y - 19, 0);
		break;
	case X:
		vg_sprite(wrong, c->x - 24, c->y - 20, 0);
		break;
	}
}

int tick_mainCircle(Circle * c) {

	c->t--;

	if(c->t == 0) {
		erase_circle(c);
		return 1;
	}

	return 0;
}

int tick_menu_circle(Circle * c) {

	vg_circle(c->x, c->y, c->r, 0);
	vg_circle(c->x, c->y, c->r + 1, 0);
	vg_circle(c->x, c->y, c->r + 2, 0);

	c->r--;

	if(c->r == 0) {
		destroy_circle(c);
		return 1;
	}

	vg_circle(c->x, c->y, c->r, c->c);
	vg_circle(c->x, c->y, c->r + 1, c->c);
	vg_circle(c->x, c->y, c->r + 2, c->c);

	return 0;
}

void tick_outerCircle(Circle * c) {

	if(c->t == 0)
		return;

	if(c->out_r > c->r + 3) {
		//Delete last outer circle and draw new
		vg_circle(c->x, c->y, c->out_r, 0);
		c->out_r--;
		vg_circle(c->x, c->y, c->out_r, c->c);

		//Re-apply the circles (in case we delete some pixels)
		vg_circle(c->x, c->y, c->r, c->c);
		vg_circle(c->x, c->y, c->r + 1, c->c);
		vg_circle(c->x, c->y, c->r + 2, c->c);

		//Re-apply the letter again to make sure it isn't erased by other circles that may appear
		switch (c->l) {
		case Q:
			vg_sprite(letterQ, c->x - 24, c->y - 22, 1);
			break;
		case W:
			vg_sprite(letterW, c->x - 24, c->y - 19, 1);
			break;
		case E:
			vg_sprite(letterE, c->x - 24, c->y - 19, 1);
			break;
		case R:
			vg_sprite(letterR, c->x - 24, c->y - 19, 1);
			break;
		case Z:
			vg_sprite(correct, c->x - 24, c->y - 19, 1);
			break;
		case X:
			vg_sprite(wrong, c->x - 24, c->y - 20, 1);
			break;
		}
	}

}

int tick_endCircle(Circle * c) {

	c->t--;

	if(c->t == 0) {
		erase_circle(c);
		return 1;
	}

	//Re-apply the circles (in case we delete some pixels)
	vg_circle(c->x, c->y, c->r, c->c);
	vg_circle(c->x, c->y, c->r + 1, c->c);
	vg_circle(c->x, c->y, c->r + 2, c->c);

	//Re-apply the letter again to make sure it isn't erased by other circles that may appear
	switch (c->l) {
	case Z:
		vg_sprite(correct, c->x - 24, c->y - 19, 1);
		break;
	case X:
		vg_sprite(wrong, c->x - 24, c->y - 20, 1);
		break;
	}

	return 0;
}

void destroy_circle(Circle * c) {

	if(c == NULL)
		return;

	free(c);
}
