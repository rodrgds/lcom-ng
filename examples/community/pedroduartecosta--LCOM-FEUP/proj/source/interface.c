#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "vbe.h"
#include "timer.h"
#include "i8254.h"

static int hook_id = NOTIFICATION_TIMER;
static unsigned long counter = 0;
#define DELAY_US 20000
#define SET_VBE_MODE        0x4F02
#define VIDEO_CARD            0x10
#define GFXMODE "0x114"

static char *video_mem; /* Process address to which VRAM is mapped */
static char *double_video; /* Temporary process address to which VRAM is mapped */
static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned bytes_per_pixel; /* Number of VRAM bytes per pixel */
int exitGame() {
	struct reg86u reg86;
	reg86.u.b.intno = 0x10; /* BIOS video services */
	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/
	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void *initGame(unsigned short mode) {
	int r;
	struct mem_range mr;
	unsigned long vram_size;
	struct reg86u reg;
	vbe_mode_info_t vm;
	reg.u.w.ax = SET_VBE_MODE;
	reg.u.w.bx = 1 << 14 | mode;
	reg.u.b.intno = VIDEO_CARD;
	if (sys_int86(&reg) != OK) {
		printf("Failed sys_int86().\n");
	}
	if (vbe_get_mode_info(mode, &vm) == 1) {
		printf("Failed vbe_get_mode_info().\n");
		return NULL;
	}
	h_res = vm.XResolution;
	v_res = vm.YResolution;
	bits_per_pixel = vm.BitsPerPixel;
	bytes_per_pixel = bits_per_pixel / 8;



	mr.mr_base = (phys_bytes)(vm.PhysBasePtr);
	mr.mr_limit = mr.mr_base + (h_res * v_res * bits_per_pixel) / 8;
	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_gr: sys_privctl (ADD_MEM) failed: %d\n", r);
	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *) mr.mr_base,
			h_res * v_res * bits_per_pixel / 8);
	if (video_mem == MAP_FAILED)
		panic("Couldn't map video memory");
	double_video = (char*) malloc(h_res * v_res * bits_per_pixel / 8);
	return video_mem;
}

void fill_pixel(unsigned short x, unsigned short y, unsigned short color) {
	*(double_video+((x+h_res*y)*bits_per_pixel/8))=color;
}

unsigned getVerResolution()
{
	return v_res;
}

unsigned getHorResolution()
{
	return h_res;
}

char* getGraphicsBuffer()
{
	return video_mem;
}

char* getGraphicsBufferTmp() {
	return double_video;
}

unsigned getVRAMSize() {
	return h_res * v_res * bits_per_pixel/8;
}

unsigned getBytesPerPixel(){
	return bytes_per_pixel;
}

char *read_xpm(char *map[], int *wd, int *ht) {
	{
		int width, height, colors;
		char sym[256];
		int col;
		int i, j;
		char *pix, *pixtmp, *tmp, *line;
		char symbol;
		/* read width, height, colors */
		if (sscanf(map[0], "%d %d %d", &width, &height, &colors) != 3) {
			printf("read_xpm: incorrect width, height, colors\n");
			return NULL;
		}
#ifdef DEBUG
		printf("%d %d %d\n", width, height, colors);
#endif
		if (width > h_res || height > v_res || colors > 256) {
			printf("read_xpm: incorrect width, height, colors\n");
			return NULL;
		}
		*wd = width;
		*ht = height;
		for (i = 0; i < 256; i++)
			sym[i] = 0;
		/* read symbols <-> colors */
		for (i = 0; i < colors; i++) {
			if (sscanf(map[i + 1], "%c %d", &symbol, &col) != 2) {
				printf("read_xpm: incorrect symbol, color at line %d\n", i + 1);
				return NULL;
			}
#ifdef DEBUG
			printf("%c %d\n", symbol, col);
#endif
			if (col > 256) {
				printf("read_xpm: incorrect color at line %d\n", i + 1);
				return NULL;
			}
			sym[col] = symbol;
		}
		/* allocate pixmap memory */
		pix = pixtmp = malloc(width * height);
		/* parse each pixmap symbol line */
		for (i = 0; i < height; i++) {
			line = map[colors + 1 + i];
#ifdef DEBUG
			printf("\nparsing %s\n", line);
#endif
			for (j = 0; j < width; j++) {
				tmp = memchr(sym, line[j], 256); /* find color of each symbol */
				if (tmp == NULL) {
					printf("read_xpm: incorrect symbol at line %d, col %d\n",
							colors + i + 1, j);
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
}

int print_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	unsigned int x_var = xi, y_var = yi;
	char *pix;
	int widht, height, tmp = 0;
	pix = read_xpm(xpm, &widht, &height);
	if (pix == NULL) {
		printf("Failed read_xpm.\n");
		return 1;
	}
	for (y_var; y_var < (yi + height); y_var++) {
		x_var = xi;
		for (x_var; x_var < (xi + widht); x_var++) {
			fill_pixel(x_var, y_var, pix[tmp]);
			tmp++;
		}
	}
	return 0;
}

void refresh() {
	memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
	memset(double_video,0,getVRAMSize());
}

