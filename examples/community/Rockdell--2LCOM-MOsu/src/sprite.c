#include <minix/drivers.h>
#include "sprite.h"
#include "video_gr.h"
#include "read_xpm.h"

Sprite * create_sprite(char *pic[], int xi, int yi) {

	//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc (sizeof(Sprite));

	if(sp == NULL)
		return NULL;

	// read the sprite pixmap
	sp->x = xi;
	sp->y = yi;
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));

	if(sp->map == NULL) {
		free(sp);
		return NULL;
	}

	return sp;
}

void vg_drawSprite(Sprite *sp) {

	//Sprite properties
	int width = sp->width, height = sp->height;
	char *map = sp->map;
	int i, j, x = sp->x, y = sp->y;

	for (i = 0; i < height; i++) {

		for (j = 0; j < width; j++) {

			char color = *(map + i * width + j);

			if (color != 35) {
				vg_paintPixel(x + j, y + i, color);
			}
			else
				continue;
		}
	}

	destroy_sprite(sp);
}

void vg_eraseSprite(Sprite *sp) {

	//Sprite properties
	int width = sp->width, height = sp->height;
	char *map = sp->map;
	int i, j, x = sp->x, y = sp->y;

	for (i = 0; i < height; i++) {

		for (j = 0; j < width; j++) {

			char color = *(map + i * width + j);

			if (color != 35) {
				vg_paintPixel(x + j, y + i, 0);
			}
			else
				continue;
		}
	}
}

void destroy_sprite(Sprite *sp) {

	if(sp == NULL)
		return;

	if(sp->map)
		free(sp->map);

	free(sp);
	sp = NULL;

}
