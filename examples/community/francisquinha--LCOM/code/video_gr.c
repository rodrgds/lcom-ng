/** @file video_gr.c
 * @brief file containing the function definitions to handle video graphics
 */
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "video_gr.h"
#include "vbe.h"
#include "read_xpm.h"
#include "images.h"
#include "game.h"

#define NUMBER_COLORS 64
#define VIDEO_MODE	0x105

#define DEBUG_MALLOC 0

/* Private global variables */

static char *video_mem;							/* Process address to which VRAM is mapped */
static char *vram_phys_addr;					/* Physical address of VRAM */
static unsigned h_res;							/* Horizontal screen resolution in pixels */
static unsigned v_res;							/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; 				/* Number of VRAM bits per pixel */

Graphics *create_graphics() {
	Graphics *graphics = (Graphics *)malloc(sizeof(Graphics));
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for graphics\n", sizeof(Graphics));
	if (graphics == NULL) {
		printf("create_graphics: error allocating memory for graphics!\n");
		return NULL;
	}
	if ((graphics->video_mem = (char *)vg_init(VIDEO_MODE)) == NULL) {
		free(graphics);
		if (DEBUG_MALLOC) printf("Freed graphics\n");
		vg_exit();
		printf("create_graphics: error switching to video mode!\n");
		return NULL;
	}
	if ((graphics->cards_buffer = get_video_buffer()) == NULL) {
		free(graphics);
		if (DEBUG_MALLOC) printf("Freed graphics\n");
		vg_exit();
		printf("create_graphics: error creating cards buffer!\n");
		return NULL;
	}
	if ((graphics->mouse_buffer = get_video_buffer()) == NULL) {
		free(graphics->cards_buffer);
		if (DEBUG_MALLOC) printf("Freed cards_buffer\n");
		free(graphics);
		if (DEBUG_MALLOC) printf("Freed graphics\n");
		vg_exit();
		printf("create_graphics: error creating mouse buffer!\n");
		return NULL;
	}
	if ((graphics->time_buffer = get_video_buffer()) == NULL) {
		free(graphics->mouse_buffer);
		if (DEBUG_MALLOC) printf("Freed mouse_buffer\n");
		free(graphics->cards_buffer);
		if (DEBUG_MALLOC) printf("Freed cards_buffer\n");
		free(graphics);
		if (DEBUG_MALLOC) printf("Freed graphics\n");
		vg_exit();
		printf("create_graphics: error creating time buffer!\n");
		return NULL;
	}
	if ((graphics->menu_buffer = get_video_buffer()) == NULL) {
		free(graphics->time_buffer);
		if (DEBUG_MALLOC) printf("Freed time_buffer\n");
		free(graphics->mouse_buffer);
		if (DEBUG_MALLOC) printf("Freed mouse_buffer\n");
		free(graphics->cards_buffer);
		if (DEBUG_MALLOC) printf("Freed cards_buffer\n");
		free(graphics);
		if (DEBUG_MALLOC) printf("Freed graphics\n");
		vg_exit();
		printf("create_graphics: error creating menu buffer!\n");
		return NULL;
	}
	if ((graphics->backg_buffer = get_video_buffer()) == NULL) {
		free(graphics->menu_buffer);
		if (DEBUG_MALLOC) printf("Freed menu_buffer\n");
		free(graphics->time_buffer);
		if (DEBUG_MALLOC) printf("Freed time_buffer\n");
		free(graphics->mouse_buffer);
		if (DEBUG_MALLOC) printf("Freed mouse_buffer\n");
		free(graphics->cards_buffer);
		if (DEBUG_MALLOC) printf("Freed cards_buffer\n");
		free(graphics);
		if (DEBUG_MALLOC) printf("Freed graphics\n");
		vg_exit();
		printf("create_graphics: error creating backg buffer!\n");
		return NULL;
	}
	copy_buffer(graphics->backg_buffer, graphics->video_mem);
	graphics->h_res = get_h_res();
	graphics->v_res = get_v_res();
	graphics->time_x = 464;
	graphics->time_y = 668;
	graphics->width = 128;
	graphics->height = 192;
	graphics->button_width = 88;
	graphics->button_height = 38;
	graphics->gap = 16;
	graphics->button_gap = 20;
	graphics->cards = get_cards();
	graphics->numbers = get_numbers();
	graphics->letters = get_letters();
	return graphics;
}

void destroy_graphics(Graphics *graphics) {
    free(graphics->letters);
	if (DEBUG_MALLOC) printf("Freed letters\n");
    free(graphics->numbers);
	if (DEBUG_MALLOC) printf("Freed numbers\n");
    free(graphics->cards);
	if (DEBUG_MALLOC) printf("Freed cards\n");
    free(graphics->backg_buffer);
	if (DEBUG_MALLOC) printf("Freed backg_buffer\n");
	free(graphics->menu_buffer);
	if (DEBUG_MALLOC) printf("Freed menu_buffer\n");
	free(graphics->time_buffer);
	if (DEBUG_MALLOC) printf("Freed time_buffer\n");
	free(graphics->mouse_buffer);
	if (DEBUG_MALLOC) printf("Freed mouse_buffer\n");
	free(graphics->cards_buffer);
	if (DEBUG_MALLOC) printf("Freed cards_buffer\n");
	free(graphics);
	if (DEBUG_MALLOC) printf("Freed graphics\n");
}

void *vg_init(unsigned short mode) {
	if (vbe_set_mode(mode)) return NULL;
	vbe_mode_info_t vbe_mode_info;
	if (vbe_get_mode_info(mode, &vbe_mode_info)) {
		printf("vg_init: vbe_get_mode_info failed\n");
		return NULL;
	}
	int r;
	struct mem_range mr;
	/* Save video mode resolution */
	h_res = vbe_mode_info.XResolution;
	v_res = vbe_mode_info.YResolution;
	bits_per_pixel = vbe_mode_info.BitsPerPixel;
	vram_phys_addr = (char *)vbe_mode_info.PhysBasePtr;
	unsigned long size = h_res * v_res * bits_per_pixel;
	/* Allow memory mapping */
	mr.mr_base = vbe_mode_info.PhysBasePtr;
	mr.mr_limit = mr.mr_base + size;
	if(OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_gr: sys_privctl (ADD_MEM) failed: %d\n", r);
	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, size);
	if(video_mem == MAP_FAILED)
		panic("video_txt couldn't map video memory");
	return video_mem;
}

char *get_video_buffer() {
	char* video_buffer;
	unsigned long size = h_res * v_res * bits_per_pixel/8;
	video_buffer = malloc(sizeof(char *) * size);
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for video_buffer\n", sizeof(char *) * size);
	return video_buffer;
}

void copy_buffer(char *destination, char *origin) {
	unsigned long size = h_res * v_res * bits_per_pixel/8;
	memcpy(destination, origin, size);
}

int vg_exit() {
  struct reg86u reg86;
  reg86.u.b.intno = 0x10; /* BIOS video services */
  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/
  if (sys_int86(&reg86) != OK) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

unsigned get_h_res() {
	return h_res;
}

unsigned get_v_res() {
	return v_res;
}

unsigned get_bits_per_pixel() {
	return bits_per_pixel;
}

char *get_vram_phys_addr() {
	return vram_phys_addr;
}

int vg_draw_rectangle(char *base, unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned long color) {
	unsigned bytes_per_pixel = bits_per_pixel / 8;
	/* check if frame fits in the screen */
	if (x + width > h_res || y + height > v_res)
		return 1;
	char *vptr;         									/* pointer to video RAM */
	vptr = base + (y * h_res + x) * bytes_per_pixel;		/* point to upper left corner */
	unsigned short i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			*vptr = color;        							/* change the color of that pixel */
			vptr += bytes_per_pixel;						/* move to the next pixel */
		}
		vptr += (h_res - width) * bytes_per_pixel;			/* move to beggining of the next line */
	}
	return 0;
}

void vg_fill_screen(char *base, unsigned long color) {
	unsigned bytes_per_pixel = bits_per_pixel / 8;
	char *vptr;         									/* pointer to video RAM */
	vptr = base;											/* point to upper left corner of the screen */
	unsigned short i, j;
	for (i = 0; i < v_res; i++) {
		for (j = 0; j < h_res; j++) {
			*vptr = color;        							/* change the color of that pixel */
			vptr += bytes_per_pixel;						/* move to the next pixel */
		}
	}
}

/* using Bresenham's Algorithm */ // this is wrong!!!
int vg_draw_line(char *base, unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {
	unsigned bytes_per_pixel = bits_per_pixel / 8;
	/* check if line fits in the screen */
	if (xi >= h_res || yi >= v_res || xf > h_res || yf > v_res)
		return 1;
	char *vptr;         									/* pointer to video RAM */
	vptr = base + (yi * h_res + xi) * bytes_per_pixel;	/* point to upper starting point */
	short delta_x = xf - xi;
	short delta_y = yf - yi;
	short sign_x = delta_x > 0 ? 1 : -1;
	short sign_y = delta_y > 0 ? 1 : -1;
	if (delta_x == 0) {
		unsigned short i;
		for (i = yi ; i != yf ; ) {
			*vptr = color;        							/* change the color of this pixel */
			vptr += sign_y * h_res * bytes_per_pixel;		/* move to the next line */
			i += sign_y;
		}
	}
	else {
		double m = (double) delta_y / delta_x;
		double abs_m = m * sign_x * sign_y;
		double e = abs_m - 1;
		unsigned short i;
		for (i = xi ; i != xf ; ) {
			*vptr = color;        							/* change the color of this pixel */
			if (e >= 0) {
				vptr += sign_y * h_res * bytes_per_pixel;	/* move to the next line */
				e -= 1;
			}
			i += sign_x;
			vptr += sign_x * bytes_per_pixel;
			e += abs_m;
		}
	}
	return 0;
}

int vg_draw_frame(char *base, unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned long color) {
	unsigned bytes_per_pixel = bits_per_pixel / 8;
	/* check if frame fits in the screen */
	if (x + width > h_res || y + height > v_res)
		return 1;
	char *vptr;         									/* pointer to video RAM */
	vptr = base + (y * h_res + x) * bytes_per_pixel;		/* point to upper left corner */
	unsigned short i, j;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < width; j++) {
			*vptr = color;        							/* change the color of that pixel */
			vptr += bytes_per_pixel;						/* move to the next pixel */
		}
		vptr += (h_res - width) * bytes_per_pixel;			/* move to beggining of the next line */
	}
	for (i = 2; i < height - 2; i++) {
		for (j = 0; j < 2; j++) {
			*vptr = color;        							/* change the color of that pixel */
			vptr += bytes_per_pixel;						/* move to the next pixel */
		}
		vptr += (width - 4) * bytes_per_pixel;				/* move to the column on the other side */
		for (j = 0; j < 2; j++) {
			*vptr = color;        							/* change the color of that pixel */
			vptr += bytes_per_pixel;						/* move to the next pixel */
		}
		vptr += (h_res - width) * bytes_per_pixel;			/* move to beggining of the next line */
	}
	for (i = 0; i < 2; i++) {
		for (j = 0; j < width; j++) {
			*vptr = color;        							/* change the color of that pixel */
			vptr += bytes_per_pixel;						/* move to the next pixel */
		}
		vptr += (h_res - width) * bytes_per_pixel;			/* move to beggining of the next line */
	}
	return 0;
}

int vg_remove_frame(char *base, char *origin, unsigned short x, unsigned short y, unsigned short width, unsigned short height) {
	unsigned bytes_per_pixel = bits_per_pixel / 8;
	/* check if frame fits in the screen */
	if (x + width > h_res || y + height > v_res)
		return 1;
	char *vptr;         									/* pointer to video RAM */
	vptr = base + (y * h_res + x) * bytes_per_pixel;		/* point to upper left corner */
	char *ovptr;         									/* pointer to video RAM */
	ovptr = origin + (y * h_res + x) * bytes_per_pixel;		/* point to upper left corner */
	unsigned short i, j;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < width; j++) {
			*vptr = *ovptr;        							/* remove the color of that pixel */
			vptr += bytes_per_pixel;						/* move to the next pixel */
			ovptr += bytes_per_pixel;						/* move to the next pixel */
		}
		vptr += (h_res - width) * bytes_per_pixel;			/* move to beggining of the next line */
		ovptr += (h_res - width) * bytes_per_pixel;			/* move to beggining of the next line */
	}
	for (i = 2; i < height - 2; i++) {
		for (j = 0; j < 2; j++) {
			*vptr = *ovptr;        							/* remove the color of that pixel */
			vptr += bytes_per_pixel;						/* move to the next pixel */
			ovptr += bytes_per_pixel;						/* move to the next pixel */
		}
		vptr += (width - 4) * bytes_per_pixel;				/* move to the column on the other side */
		ovptr += (width - 4) * bytes_per_pixel;				/* move to the column on the other side */
		for (j = 0; j < 2; j++) {
			*vptr = *ovptr;        							/* remove the color of that pixel */
			vptr += bytes_per_pixel;						/* move to the next pixel */
			ovptr += bytes_per_pixel;						/* move to the next pixel */
		}
		vptr += (h_res - width) * bytes_per_pixel;			/* move to beggining of the next line */
		ovptr += (h_res - width) * bytes_per_pixel;			/* move to beggining of the next line */
	}
	for (i = 0; i < 2; i++) {
		for (j = 0; j < width; j++) {
			*vptr = *ovptr;        								/* remove the color of that pixel */
			vptr += bytes_per_pixel;						/* move to the next pixel */
			ovptr += bytes_per_pixel;						/* move to the next pixel */
		}
		vptr += (h_res - width) * bytes_per_pixel;			/* move to beggining of the next line */
		ovptr += (h_res - width) * bytes_per_pixel;			/* move to beggining of the next line */
	}
	return 0;
}

int vg_draw_pixmap(char *base, unsigned short xi, unsigned short yi, char *pixmap, unsigned short width, unsigned short height) {
	unsigned bytes_per_pixel = bits_per_pixel / 8;
	/* check if pixmap fits in the screen */
	if (xi + width > h_res || yi + height > v_res)
		return 1;
	char *vptr;         											/* pointer to video RAM */
	vptr = base + (yi * h_res + xi) * bytes_per_pixel;				/* point to upper left corner */
	unsigned short i, j;
	unsigned long color;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			color = *(pixmap + (i * width + j));					/* get pixmap color */
			if (color < NUMBER_COLORS) *vptr = color ;      		/* change the color of the pixel */
			vptr += bytes_per_pixel;								/* move to the next pixel */
		}
		vptr += (h_res - width) * bytes_per_pixel;					/* move to beggining of the next line */
	}
	return 0;
}

int vg_draw_pixmap_black(char *base, unsigned short xi, unsigned short yi, char *pixmap, unsigned short width, unsigned short height) {
	unsigned bytes_per_pixel = bits_per_pixel / 8;
	/* check if pixmap fits in the screen */
	if (xi + width > h_res || yi + height > v_res)
		return 1;
	char *vptr;         											/* pointer to video RAM */
	vptr = base + (yi * h_res + xi) * bytes_per_pixel;				/* point to upper left corner */
	unsigned short i, j;
	unsigned long color;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			color = *(pixmap + (i * width + j));					/* get pixmap color */
			if (color < NUMBER_COLORS) *vptr = color ;      		/* change the color of the pixel */
			else *vptr = 0;
			vptr += bytes_per_pixel;								/* move to the next pixel */
		}
		vptr += (h_res - width) * bytes_per_pixel;					/* move to beggining of the next line */
	}
	return 0;
}

int vg_clear_pixmap(char *base, char *origin, unsigned short xi, unsigned short yi, unsigned short width, unsigned short height) {
	unsigned bytes_per_pixel = bits_per_pixel / 8;
	/* check if pixmap fits in the screen */
	if (xi + width > h_res || yi + height > v_res)
		return 1;
	char *vptr;         											/* pointer to base video RAM */
	vptr = base + (yi * h_res + xi) * bytes_per_pixel;				/* point to upper left corner */
	char *ovptr;         											/* pointer to original video RAM */
	ovptr = origin + (yi * h_res + xi) * bytes_per_pixel;			/* point to upper left corner */
	unsigned short i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			*vptr = *ovptr ;      									/* clear the color of the pixel */
			vptr += bytes_per_pixel;								/* move to the next pixel */
			ovptr += bytes_per_pixel;								/* move to the next pixel */
		}
		vptr += (h_res - width) * bytes_per_pixel;					/* move to beggining of the next line */
		ovptr += (h_res - width) * bytes_per_pixel;					/* move to beggining of the next line */
	}
	return 0;
}

void vg_draw_mouse(char *base, char *origin, Mouse *mouse) {
	copy_buffer(base, origin);
	if (mouse->first_packet) return;
	int width, height;
	char **xpm = get_mouse_xpm();
	char *pixmap = read_xpm(xpm, &width, &height);
	unsigned bytes_per_pixel = bits_per_pixel / 8;
	int draw_width, draw_height;
	if (mouse->x + width >= h_res) draw_width = h_res - mouse->x;
	else draw_width = width;
	if (mouse->y + height >= h_res) draw_height = v_res - mouse->y;
	else draw_height = height;
	char *vptr;         											/* pointer to video RAM */
	vptr = base + (mouse->y * h_res + mouse->x) * bytes_per_pixel;	/* point to upper left corner */
	unsigned short i, j;
	unsigned long color;
	for (i = 0; i < draw_height; i++) {
		for (j = 0; j < draw_width; j++) {
			color = *(pixmap + (i * width + j) * bytes_per_pixel);	/* get pixmap color */
			if (color < 64) *vptr = color ;      					/* change the color of the pixel */
			vptr += bytes_per_pixel;								/* move to the next pixel */
		}
		vptr += (h_res - draw_width) * bytes_per_pixel;				/* move to beggining of the next line */
	}
	free(pixmap);
}

void vg_draw_time(char *base, char *origin, Timer *timer, char ***numbers, unsigned short xi, unsigned short yi) {
	copy_buffer(base, origin);
	unsigned short digits[8];
	digits[0] = timer->hours / 10;
	digits[1] = timer->hours % 10;
	digits[2] = 10;
	digits[3] = timer->minutes / 10;
	digits[4] = timer->minutes % 10;
	digits[5] = 10;
	digits[6] = timer->seconds / 10;
	digits[7] = timer->seconds % 10;
	char **xpm;
	char *pixmap;
	int width, height;
	unsigned short i;
	for (i = 0; i < 8; i++) {
		xpm = numbers[digits[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap_black(base, xi, yi, pixmap, width, height);
		free(pixmap);
		xi += width;
	}
}

void vg_draw_highscore(char *base, char *origin, HighScore *highscore, char ***numbers, char ***letters, unsigned short xi, unsigned short yi) {
	copy_buffer(base, origin);
	unsigned short name[5];
	short i;
	char **xpm;
	char *pixmap;
	int width, height;
	for (i = 0; i < 5; i++) {
		xpm = letters[highscore->name[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, xi, yi, pixmap, width, height);
		free(pixmap);
		xi += width;
	}
	for (i = 0; i < 4; i++) {
		xpm = letters[0];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, xi, yi, pixmap, width, height);
		free(pixmap);
		xi += width;
	}
	unsigned short digits[19];
	digits[0] = highscore->datetime->day / 10;
	digits[1] = highscore->datetime->day % 10;
	digits[2] = 11;
	digits[3] = highscore->datetime->month / 10;
	digits[4] = highscore->datetime->month % 10;
	digits[5] = 11;
	digits[6] = highscore->datetime->year / 1000;
	digits[7] = (highscore->datetime->year % 1000) / 100;
	digits[8] = (highscore->datetime->year % 100) / 10;
	digits[9] = highscore->datetime->year % 10;
	digits[10] = 12;
	digits[11] = highscore->datetime->hours / 10;
	digits[12] = highscore->datetime->hours % 10;
	digits[13] = 10;
	digits[14] = highscore->datetime->minutes / 10;
	digits[15] = highscore->datetime->minutes % 10;
	digits[16] = 10;
	digits[17] = highscore->datetime->seconds / 10;
	digits[18] = highscore->datetime->seconds % 10;
	for (i = 0; i < 19; i++) {
		xpm = numbers[digits[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, xi, yi, pixmap, width, height);
		free(pixmap);
		xi += width;
	}
	for (i = 0; i < 4; i++) {
		xpm = letters[0];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, xi, yi, pixmap, width, height);
		free(pixmap);
		xi += width;
	}
	unsigned short score_digits[5];
	unsigned score = highscore->score;
	for (i = 4; i >= 0; i--) {
		if (score == 0) score_digits[i] = 12;
		else {
			score_digits[i] = score % 10;
			score /= 10;
		}
	}
	for (i = 0; i < 5; i++) {
		xpm = numbers[score_digits[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, xi, yi, pixmap, width, height);
		free(pixmap);
		xi += width;
	}
}

void vg_draw_background(char *base) {
	int width, height;
	char **xpm = get_background();
	char *pixmap = read_xpm(xpm, &width, &height);
	vg_draw_pixmap(base, 190, 97, pixmap,
			(unsigned short)width, (unsigned short)height);
	free(pixmap);
}

void vg_draw_random_background(char *base) {
	unsigned bytes_per_pixel = bits_per_pixel / 8;
	char *vptr;         									/* pointer to video RAM */
	char *uptr;         									/* pointer to video RAM */
	char *wptr;         									/* pointer to video RAM */
	vptr = base;											/* point to upper left corner of the screen */
	uptr = base;											/* point to upper left corner of the screen */
	wptr = base;											/* point to upper left corner of the screen */
	unsigned long colors[6] = {2, 9, 29, 36, 38, 54};
	unsigned short black, change;
	unsigned short i, j;
	black = rand() % 10;
	if (!black) *vptr = colors[rand() % 6];     	/* change the color of that pixel */
	vptr += bytes_per_pixel;						/* move to the next pixel */
	for (j = 1; j < h_res; j++) {
		change = rand() % 3;
		if (change) *vptr = *wptr;
		else {
			black = rand() % 10;
			if (!black) *vptr = colors[rand() % 6];
		}
		vptr += bytes_per_pixel;
		wptr += bytes_per_pixel;
	}
	for (i = 1; i < v_res; i++) {
		for (j = 0; j < h_res; j++) {
			change = rand() % 5;
			if (change > 2) *vptr = *wptr;
			else if (change) *vptr = *uptr;
			else {
				black = rand() % 10;
				if (!black) *vptr = colors[rand() % 6];
			}
			vptr += bytes_per_pixel;
			uptr += bytes_per_pixel;
			wptr += bytes_per_pixel;
		}
	}
}

unsigned short vg_draw_random_square_background(char *base, unsigned short min_number, unsigned short max_number, unsigned short min_side, unsigned short max_side, unsigned short height) {
	if (min_number > max_number || min_side < 1 || min_side > max_side || height > v_res) return 1;
	unsigned bytes_per_pixel = bits_per_pixel / 8;
	unsigned long colors[6] = {2, 9, 29, 36, 38, 54};
	unsigned short number = rand() % (max_number - min_number) + min_number;
	unsigned short i, x, y, side, color;
	for (i = 0; i < number; i++) {
		side = rand() % (max_side - min_side) + min_side;
		x = rand() % (h_res - side);
		y = rand() % (height - side);
		color = rand() % 6;
		vg_draw_rectangle(base, x, y, side, side, colors[color]);
	}
	return 0;
}

void vg_draw_buttons(char *base, unsigned short card_gap, unsigned short card_width, unsigned short card_height, char play_pause) {
	unsigned short x = card_gap;
	unsigned short y = card_gap + 3 * (card_height + card_gap);
	int width, height;
	char **xpm = get_set_button();
	char *pixmap = read_xpm(xpm, &width, &height);
	vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
	free(pixmap);
	x += card_width + card_gap;
	xpm = get_nosets_button();
	pixmap = read_xpm(xpm, &width, &height);
	vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
	free(pixmap);
	x += card_width + card_gap;
	xpm = get_hint_button();
	pixmap = read_xpm(xpm, &width, &height);
	vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
	free(pixmap);
	x += 2 * (card_width + card_gap);
	if (play_pause == GAME_PAUSE) xpm = get_play_button();
	else xpm = get_pause_button();
	pixmap = read_xpm(xpm, &width, &height);
	vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
	free(pixmap);
	x += card_width + card_gap;
	xpm = get_save_button();
	pixmap = read_xpm(xpm, &width, &height);
	vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
	free(pixmap);
	x += card_width + card_gap;
	xpm = get_exit_button();
	pixmap = read_xpm(xpm, &width, &height);
	vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
	free(pixmap);
}

void vg_draw_menu_init(char *base, char *origin, char ***letters) {
	copy_buffer(base, origin);
	unsigned short x = 311;
	unsigned short y = 200;
	int width, height;
	char **xpm = get_set_logo();
	char *pixmap = read_xpm(xpm, &width, &height);
	vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
	free(pixmap);
	unsigned short new_game[] = {14, 5, 23, 0, 7, 1, 13, 5};
	y += 274;
	x = 456;
	unsigned short i;
	for (i = 0; i < 8; i++) {
		xpm = letters[new_game[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
	unsigned short load_game[] = {12, 15, 1, 4, 0, 7, 1, 13, 5};
	y += 72;
	x = 449;
	for (i = 0; i < 9; i++) {
		xpm = letters[load_game[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
}

void vg_draw_menu_new(char *base, char *origin, char ***letters) {
	copy_buffer(base, origin);
	int width, height;
	char **xpm;
	char *pixmap;
	unsigned short x = 421;
	unsigned short y = 190;
	unsigned short single_player[] = {19, 9, 14, 7, 12, 5, 0, 16, 12, 1, 25, 5, 18};
	unsigned short i;
	for (i = 0; i < 13; i++) {
		xpm = letters[single_player[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
	x = 456;
	y += 122 ;
	unsigned short hot_seat[] = {8, 15, 20, 0, 19, 5, 1, 20};
	for (i = 0; i < 8; i++) {
		xpm = letters[hot_seat[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
	x = 428;
	y += 122 ;
	unsigned short sudden_death[] = {19, 21, 4, 4, 5, 14, 0, 4, 5, 1, 20, 8};
	for (i = 0; i < 12; i++) {
		xpm = letters[sudden_death[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
	x = 442;
	y += 122 ;
	unsigned short two_player[] = {20, 23, 15, 0, 16, 12, 1, 25, 5, 18};
	for (i = 0; i < 10; i++) {
		xpm = letters[two_player[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
}

void vg_draw_menu_load_save(char *base, char *origin, char ***numbers, char ***letters) {
	copy_buffer(base, origin);
	int width, height;
	char **xpm;
	char *pixmap;
	unsigned short x = 470;
	unsigned short y = 190;
	unsigned short slot[] = {19, 12, 15, 20, 0};
	unsigned short i, j;
	for (i = 1; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			xpm = letters[slot[j]];
			pixmap = read_xpm(xpm, &width, &height);
			vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
			free(pixmap);
			x += width;
		}
		xpm = numbers[i];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x = 470;
		y += 122 ;
	}
}

void vg_draw_menu_single(char *base, char *origin, unsigned short name[5], char ***letters) {
	copy_buffer(base, origin);
	int width, height;
	char **xpm;
	char *pixmap;
	unsigned short x = 421;
	unsigned short y = 190;
	unsigned short single_player[] = {19, 9, 14, 7, 12, 5, 0, 16, 12, 1, 25, 5, 18};
	unsigned short i;
	for (i = 0; i < 13; i++) {
		xpm = letters[single_player[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
	x = 442;
	y += 122;
	unsigned short name_place[] = {14, 1, 13, 5, 0};
	for (i = 0; i < 5; i++) {
		xpm = letters[name_place[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
	for (i = 0; i < 5; i++) {
		xpm = letters[name[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
}

void vg_draw_menu_hot(char *base, char *origin, unsigned short number, unsigned short names[9][5], char ***numbers, char ***letters) {
	copy_buffer(base, origin);
	int width, height;
	char **xpm;
	char *pixmap;
	unsigned short x = 456;
	unsigned short y = 114;
	unsigned short hot_seat[] = {8, 15, 20, 0, 19, 5, 1, 20};
	unsigned short i;
	for (i = 0; i < 8; i++) {
		xpm = letters[hot_seat[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
	x = 379;
	y += 122;
	unsigned short number_players[] = {14, 21, 13, 2, 5, 18, 0, 15, 6, 0, 16, 12, 1, 25, 5, 18, 19, 0};
	for (i = 0; i < 18; i++) {
		xpm = letters[number_players[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
	if (number > 0) {
		xpm = numbers[number];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
	}
	unsigned short name_place[] = {14, 1, 13, 5, 0};
	unsigned short j;
	x = 428;
	y += 44;
	for (i = 1; i < 10; i++) {
		for (j = 0; j < 5; j++) {
			xpm = letters[name_place[j]];
			pixmap = read_xpm(xpm, &width, &height);
			vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
			free(pixmap);
			x += width;
		}
		xpm = numbers[i];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x+= width;
		xpm = letters[0];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x+= width;
		for (j = 0; j < 5; j++) {
			xpm = letters[names[i - 1][j]];
			pixmap = read_xpm(xpm, &width, &height);
			vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
			free(pixmap);
			x += width;
		}
		x = 428;
		y += 44 ;
	}
}

void vg_draw_menu_sudden(char *base, char *origin, unsigned short name[5], char ***letters) {
	copy_buffer(base, origin);
	int width, height;
	char **xpm;
	char *pixmap;
	unsigned short x = 428;
	unsigned short y = 190;
	unsigned short sudden_death[] = {19, 21, 4, 4, 5, 14, 0, 4, 5, 1, 20, 8};
	unsigned short i;
	for (i = 0; i < 12; i++) {
		xpm = letters[sudden_death[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
	x = 442;
	y += 122;
	unsigned short name_place[] = {14, 1, 13, 5, 0};
	for (i = 0; i < 5; i++) {
		xpm = letters[name_place[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
	for (i = 0; i < 5; i++) {
		xpm = letters[name[i]];
		pixmap = read_xpm(xpm, &width, &height);
		vg_draw_pixmap(base, x, y, pixmap, (unsigned short)width, (unsigned short)height);
		free(pixmap);
		x += width;
	}
}
