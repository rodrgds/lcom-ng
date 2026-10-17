#include "sprite.h"
#include "read_xpm.h"

#define NULL 0

Sprite * create_sprite(char *pic[], int xi, int yi){
	Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
	if( sp == NULL )
		return NULL;
	// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if( sp->map == NULL ) {
		free(sp);
		return NULL;
	}
	sp->x = xi;
	sp->y = yi;
	return sp;
}

int animate_sprite(Sprite *fig, float delta , int hor){
	Sprite *sp2 = (Sprite *) malloc ( sizeof(Sprite));
	sp2 = fig;
	destroy_sprite(fig);
	if(hor) sp2->x += delta;
	else sp2->y +=delta;
	draw_sprite(sp2);
}

void destroy_sprite(Sprite *sp) {
	if( sp == NULL )
		return;
	free(sp->map);
	free(sp);
	sp = NULL; // hopeless: pointer is passed by value
	draw_sprite(sp);
}

void erase_sprite(Sprite *sp){
	int i,j;
	for (j = sp->y; j < sp->y + sp->height; j++) {
		for (i = sp->x; i < sp->x + sp->width; i++) {
			vg_set_pixel(i, j, 0);
		}
	}
}


int draw_sprite(Sprite *sp) {
	int i, j;
	for (j = sp->y; j < sp->y + sp->height; j++) {
		for (i = sp->x; i < sp->x + sp->width; i++) {
			vg_set_pixel(i, j, sp->map[(j-sp->y)*sp->width + i-sp->x]);
		}
	}
}
