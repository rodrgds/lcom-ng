#include "space_ship.h"
#include "vbe.h"


int move_spaceship(space_ship *nv1, short delta) {

	if (!( ((nv1->x == 0) && (delta < 0)) || ((nv1->x == 940) && (delta > 0))))
	{
			if ((nv1->x + delta) < 0)
				delta = -nv1->x;

			if ((nv1->x + delta) > 940)
				delta = 940 - nv1->x;


			nv1->x = nv1->x + delta;

	}

	return 0;
}

void draw_spaceship(space_ship nv1)
{
	xpm_cre(&nv1.height, &nv1.width, nv1.x, nv1.y, nv1.sprite);

}
