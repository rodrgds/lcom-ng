#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xpm.h"
#include "video_gr.h"

char *read_xpm(char *map[], int *wd, int *ht) {
	int width, height, colors;
	char sym[256];
	int col;
	int i, j;
	char *pix, *pixtmp, *tmp, *line;
	char symbol;

	/* read width, height, colors */
	if (sscanf(map[0], "%d %d %d", &width, &height, &colors) != 3) {
		return NULL;
	}
	if (width > getHRes() || height > getVRes() || colors > 256) {
		return NULL;
	}

	*wd = width;
	*ht = height;

	for (i = 0; i < 256; i++)
		sym[i] = 0;

	/* read symbols <-> colors */
	for (i = 0; i < colors; i++) {
		if (sscanf(map[i + 1], "%c %d", &symbol, &col) != 2) {
			return NULL;
		}
		if (col > 256) {
			return NULL;
		}
		sym[col] = symbol;
	}

	/* allocate pixmap memory */
	pix = pixtmp = malloc(width * height);

	/* parse each pixmap symbol line */
	for (i = 0; i < height; i++) {
		line = map[colors + 1 + i];
		for (j = 0; j < width; j++) {
			tmp = memchr(sym, line[j], 256); /* find color of each symbol */
			if (tmp == NULL) {
				printf("read_xpm: incorrect symbol at line %d, col %d\n",
						colors + i + 1, j);
				return NULL;
			}
			*pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-) */
		}
	}

	return pix;
}

// Cria um XPM no buffer
void object_creator(int *width, int *height, unsigned short xi,
		unsigned short yi, char *xpm[]) {
	char *video = (char *)getBuffer();
	video = video + xi + yi * getHRes();

	char *pixel = read_xpm(xpm, width, height);

	char *decoded = pixel;
	int w, h;

	for (h = 0; h < *height; h++) {
		for (w = 0; w < *width; w++) {
			*video = *pixel;
			video++;
			pixel++;
		}
		video = video - *width + getHRes();
		w = 0;
	}

	free(decoded);
	return;
}

// Cria um XPM no buffer
void title_creator(unsigned short xi, unsigned short yi, char *xpm[]) {
	int width;
	int height;
	char *video = (char *)getBuffer();
	video = video + xi + yi * getHRes();

	char *pixel = read_xpm(xpm, &width, &height);

	char *decoded = pixel;
	int w, h;

	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) {
			*video = *pixel;
			video++;
			pixel++;
		}
		video = video - width + getHRes();
		w = 0;
	}
	free(decoded);
	return;
}

// Apaga no buffer
void buffer_destructor(int width, int height, unsigned short xi,
		unsigned short yi) {
	char *video = getBuffer();
	video = video + xi + yi * getHRes();

	int w, h;

	for (h = 0; h < height; h++) {
		for (w = 0; w < width; w++) {
			*video = 0; // the pixel there won't have any value, therefore it deletes
			video++;
		}
		video = video - width + getHRes();
		w = 0;
	}
	return;
}
