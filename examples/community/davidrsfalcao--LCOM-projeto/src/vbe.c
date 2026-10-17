#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include "lmlib.h"
#include "vbe.h"
#include "video_gr.h"

#define LINEAR_MODEL_BIT (1 << 14)


#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)


int vbe_set_mode(unsigned short function, unsigned short mode) {


	struct reg86u r;

	r.u.w.ax = function;
	r.u.w.bx =  mode;
	r.u.b.intno = INTERRUPT_VBE;


	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return 1;
	}
	if (r.u.b.ah == 0x01 )
	{
		printf("Function call failed!\n");
		return 1;
	}
	else if (r.u.b.ah == 0x02)
	{
		printf("Function is not supported in current HW configuration!\n");
		return 1;
	}
	else if (r.u.b.ah == 0x03)
	{
		printf("Function is invalid in current video mode!\n");
		return 1;
	}

	return 0;
}

char *read_xpm(char *map[], int *wd, int *ht)
{
	int width, height, colors;
	char sym[256];
	int  col;
	int i, j;
	char *pix, *pixtmp, *tmp, *line;
	char symbol;

	/* read width, height, colors */
	if (sscanf(map[0],"%d %d %d", &width, &height, &colors) != 3) {
		printf("read_xpm: incorrect width, height, colors\n");
		return NULL;
	}
#ifdef DEBUG
	printf("%d %d %d\n", width, height, colors);
#endif
	if (width > 1024 || height > 768 || colors > 256) {
		printf("read_xpm: incorrect width, height, colors\n");
		return NULL;
	}

	*wd = width;
	*ht = height;

	for (i=0; i<256; i++)
		sym[i] = 0;

	/* read symbols <-> colors */
	for (i=0; i<colors; i++) {
		if (sscanf(map[i+1], "%c %d", &symbol, &col) != 2) {
			printf("read_xpm: incorrect symbol, color at line %d\n", i+1);
			return NULL;
		}
#ifdef DEBUG
		printf("%c %d\n", symbol, col);
#endif
		if (col > 256) {
			printf("read_xpm: incorrect color at line %d\n", i+1);
			return NULL;
		}
		sym[col] = symbol;
	}

	/* allocate pixmap memory */
	pix = pixtmp = malloc(width*height);

	/* parse each pixmap symbol line */
	for (i=0; i<height; i++) {
		line = map[colors+1+i];
#ifdef DEBUG
		printf("\nparsing %s\n", line);
#endif
		for (j=0; j<width; j++) {
			tmp = memchr(sym, line[j], 256);  /* find color of each symbol */
			if (tmp == NULL) {
				printf("read_xpm: incorrect symbol at line %d, col %d\n", colors+i+1, j);
				return NULL;
			}
			*pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-) */
#ifdef DEBUG
			printf("%c:%d ", line[j], tmp-sym);
#endif
		}
	}

	return pix;
}

int xpm_cre(int *altura, int *largura, unsigned short x, unsigned short y, char* xpm[]) {

	int i = 0;
	int j = 0;

	char *video_copy = getSBuffer();
	unsigned h_res = getHRes();

	video_copy = video_copy + h_res * y + x;

	char* pixel;
	if (( pixel= read_xpm(xpm, largura, altura)) == NULL)
	{
		return 1;
	}

	else
	{
		char *allocation = pixel;
		for (i; i < *altura; i++)
		{
			for (j; j < *largura; j++)
			{
				if (*pixel != 0)
					*video_copy = *pixel;
				video_copy++;
				pixel++;
			}
			video_copy = video_copy + h_res - *largura;
			j = 0;
		}
		free(allocation);
	}
	return 0;
}

int xpm_del(int *altura, int *largura, unsigned short x, unsigned short y) {

	int i = 0;
	int j = 0;

	char *video_copy = getSBuffer();
	unsigned h_res = getHRes();

	video_copy = video_copy + h_res * y + x;

	for (i; i < *altura; i++)
	{
		for (j; j < *largura; j++)
		{
			*video_copy = 0;
			video_copy++;
		}
		video_copy = video_copy + h_res - *largura;
		j = 0;
	}
	return 0;
}

