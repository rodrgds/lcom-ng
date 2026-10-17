#include "sprite.h"
#include "read_xpm.h"
#include <stdio.h>

Sprite * create_sprite(int xi, int yi, char *pic[])
{
	Sprite * s = (Sprite *)malloc(sizeof(Sprite));

	s->map = (char *)read_xpm(pic,&s->width,&s->height);
	if (s->map == NULL)
	{
		printf("Error on mapping sprite - create_sprite()\n");
		free(s);
		return NULL;
	}

	s->x = xi;
	s->y = yi;
	s->xspeed = 0;
	s->yspeed = 0;
	return s;
}

void destroy_sprite(Sprite *fig, char *base)
{
	free(fig->map);
	free(fig);
}

void clear_sprite_off_screen(Sprite *fig)
{
	/*
	int height, width;

	for (height = 0; height < fig->height; height++)
	{
		for (width = 0; width < fig->width; width++)
		{
			colorPixel((unsigned short) (width + fig->x), (unsigned short) (height + fig->y), video_mem, 0);
		}
	}
	*/
}
