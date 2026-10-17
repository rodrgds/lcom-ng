#include "cursor.h"
#include "vbe.h"

void draw_cursor(cursor r1)
{
	xpm_cre(&r1.height, &r1.width, r1.x, r1.y, r1.sprite);
}

void move_cursor(cursor *r1)
{
	unsigned short x_sign, y_sign;

	if (packet[0] & LEFT_BUTTON)
		r1->pressed = 1;
	else
		r1->pressed = 0;

	if (packet[0] & X_SIGN)
		x_sign = 1;
	else
		x_sign = 0;
//
//	if (x_sign)
//		packet[1] |= (-1 << 8);
//
//	if (packet[1]>=0)
//	{
//		if (((r1->x) + packet[1]) < 1024)
//		{
//			r1->x = (*r1).x + packet[1];
//		}
//		else r1->x = 1023;
//
//	}
//	else
//	{
//		if (((r1->x) + packet[1]) > 0)
//		{
//			r1->x = (*r1).x + packet[1];
//		}
//		else r1->x = 0;
//	}

	if (x_sign)
	{
		r1->x = r1->x -10;
		if (r1->x < 0)
			r1->x = 0;

	}
	else {
		r1->x = r1->x + 10;
		if (r1->x > 1024)
			r1->x = 1024;
	}


//	r1->x = (*r1).x + packet[1];

	if (packet[0] & Y_SIGN)
		y_sign = 1;
	else
		y_sign = 0;
//
//	if (y_sign)
//		packet[2] |= (-1 << 8);
//
//	if (packet[2]>=0)
//		{
//		if (((r1->y) - packet[2]) < 700) {
//			r1->y = (*r1).y - packet[2];
//		} else
//			r1->y = 700;
//
//	} else {
//		if (((r1->y) + packet[2]) > 0) {
//			r1->y = (*r1).y + packet[2];
//		} else
//			r1->y = 0;
//	}

	if (y_sign)
	{
		r1->y = r1->y +10;
		if (r1->y > 760)
			r1->y = 760;
	}
	else {
		r1->y = r1->y - 10;
		if (r1->y < 0)
			r1->y = 0;
	}



}

