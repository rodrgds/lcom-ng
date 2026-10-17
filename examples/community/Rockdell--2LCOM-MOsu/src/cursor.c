#include <minix/drivers.h>
#include "cursor.h"
#include "video_gr.h"

#include "pixmap.h"

Cursor * create_cursor() {

	Cursor* c = (Cursor *) malloc (sizeof(Cursor));

	if(c == NULL)
		return NULL;

	c->x = 500;
	c->y = 500;

	draw_cursor(c);

	return c;
}

void draw_cursor(Cursor * c) {

	vg_sprite(cursor, c->x, c->y, 1);
}

void erase_cursor(Cursor * c) {

	vg_sprite(cursor, c->x, c->y, 0);
}

void update_cursor(Cursor * c, int dx, int dy) {

	erase_cursor(c);

	int x = c->x, y = c->y;

	x += dx;
	y -= dy;

	if (x < 3)
		x = 3;
	else if (x > 1021)
		x = 1021;

	if (y < 3)
		y = 3;
	else if (y > 765)
		y = 765;

	c->x = x;
	c->y = y;

	draw_cursor(c);
}

void destroy_cursor(Cursor * c) {

	if(c == NULL)
		return;

	free(c);

}
