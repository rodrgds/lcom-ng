#include <minix/syslib.h>
#include <minix/drivers.h>

#include <machine/int86.h>

#include <sys/mman.h>
#include <sys/types.h>

#include <math.h>

#include "vbe.h"
#include "lmlib.h"

#include "ascii_map.h"

/* Constants for VBE 0x105 mode */

#define VRAM_PHYS_ADDR      0xF0000000
#define H_RES               1024
#define V_RES               768
#define BITS_PER_PIXEL      8

#define CHAR_MAX_HEIGHT     9
#define CHAR_MAX_WIDTH      7

#define SCORE_MAX_DIGITS    10

#define SET_PIXEL_NUMBER(m) for (i = 0; i < CHAR_MAX_HEIGHT; i++) {\
                                for (j = 0; j < CHAR_MAX_WIDTH; j++) {\
                                    vg_set_pixel(x+j, y+i, m[i][j]);\
                                }\
                            }

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void * vg_init(unsigned short mode) {
	struct reg86u reg86;
	vbe_mode_info_t vbe_inf;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.w.ax = 0x4F02;
	reg86.u.w.bx = 1<<14|mode;

	if( sys_int86(&reg86) != OK ) {
	    exit(0);
	}

	if (vbe_get_mode_info(mode, &vbe_inf) == 1) {
		printf("\tvg_init: vbe_get_mode_info() failed. using fallback... \n");

		h_res = H_RES;
		v_res = V_RES;
		bits_per_pixel = BITS_PER_PIXEL;
	} else {
		h_res = vbe_inf.XResolution;
		v_res = vbe_inf.YResolution;
		bits_per_pixel = vbe_inf.BitsPerPixel;
	}

    int r;
    struct mem_range mr;
    
    /* Allow memory mapping */
    
    mr.mr_base = (phys_bytes)(VRAM_PHYS_ADDR);
    mr.mr_limit = mr.mr_base + h_res * v_res * BITS_PER_PIXEL;
    
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);
    
    /* Map memory */
    
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, h_res * v_res * BITS_PER_PIXEL);
    
    if(video_mem == MAP_FAILED)
        panic("video_txt couldn't map video memory");

	return video_mem;
}

int vg_get_res(int *h, int *v) {
    if (!h_res || !v_res)
        return 1;
    
    *h = h_res;
    *v = v_res;
    
    return 0;
}

int vg_fill(unsigned long color) {
	if (color > 255)
		return 1;

	char *vptr;

    vptr = video_mem;

	int i = 0;

	int8_t byte = (char)color; // forcing 8 bit char

	for (i = 0; i < v_res * h_res; i++) {
		vptr[i] = byte;
	}

	return 0;
}

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color) {
	if (y+1 > v_res || x+1 > h_res)
		return 1;

	char *vptr;

	vptr = video_mem;

	int i = 0;

	int8_t byte = (char)color; // forcing 8 bit char

	vptr[x + y * h_res] = byte;

	return 0;
}

long vg_get_pixel(unsigned long x, unsigned long y) {
	if (y+1 > v_res || x+1 > h_res)
		return 1;

	unsigned int pos = y * h_res + x;

	char *vptr;

	vptr = video_mem;

	return (long)vptr[pos];
}

int vg_draw_line(unsigned long xi, unsigned long yi, 
		 unsigned long xf, unsigned long yf, unsigned long color) {
	if (xi == xf || yi == yf) { // vertical/horizontal lines
		int y = yi;

		while (y <= yf) {
			int x = xi;

			while (x <= xf) {
				int res = vg_set_pixel(x, y, color);

				if (res)
					return res;

				x++;
			}

			y++;
		}
	} else { // diagonal lines
		int a = (yi-yf)/(xi-xf);

		int b = yi;

		int x = xi;

		for (; x <= xf; x++) {
			int y = a*x + b; // y = ax + b

			vg_set_pixel(x, y, color);
		}
	}

	return 0;
}

int vg_draw_rect(unsigned long xi, unsigned long yi,
                 unsigned long xf, unsigned long yf,
                 unsigned long color, unsigned int fill) {
    if (!(xi < xf))
        return 1;
    
    if (!(yi < yf))
        return 1;
    
    vg_draw_line(xi, yi, xf, yi, color);
    vg_draw_line(xi, yf, xf, yf, color);
    vg_draw_line(xi, yi, xi, yf, color);
    vg_draw_line(xf, yi, xf, yf, color);
    
    if (fill) {
        unsigned int i = yi;
        
        for (; i <= yf; i++) {
            unsigned int j = xi;
            
            for (; j <= xf; j++) {
                vg_set_pixel(j, i, color);
            }
        }
    }
    
    return 0;
}

unsigned int _uint_digits(unsigned int number) {
    unsigned int tmp = 1;
    
    while (number > 9) {
        number /= 10;
        tmp++;
    }
    
    return tmp;
}

int vg_draw_int(unsigned long x, unsigned long y, unsigned long color, unsigned int number) {
    number *= 10;
    
    unsigned int i = 0;
    unsigned int runs = _uint_digits(number);
    
    unsigned int score[SCORE_MAX_DIGITS] = {0};
    
    for (i = 0; i < runs; i++) {
        score[i] = number % 10;
        
        number = floor(number / 10);
    }
    
    unsigned int rev[SCORE_MAX_DIGITS] = {0};
    
    for (i = 0; i < SCORE_MAX_DIGITS; i++)
        rev[SCORE_MAX_DIGITS - i] = score[i];
    
    for (i = 0; i < SCORE_MAX_DIGITS; i++)
        vg_draw_number(x + 8 * i, y, color, rev[i]);
    
    return 0;
}

int vg_draw_time(unsigned long x, unsigned long y, unsigned long color, unsigned int h, unsigned int m, unsigned long s) {
    int i, j;
    
    unsigned int _cl_blink = (s % 2);
    
    /*
     *  Hours
     */
    
    if (h > 9)
        vg_draw_number(x, y, color, h / 10);
    else
        vg_draw_number(x, y, color, 0);
    
    x += 8;
    
    vg_draw_number(x, y, color, h % 10);
    
    x += 8;
    
    if (!_cl_blink)
        SET_PIXEL_NUMBER(pm_doubledot);
    
    x += 8;
    
    /*
     *  Minutes
     */
    
    if (m > 9)
        vg_draw_number(x, y, color, m / 10);
    else
        vg_draw_number(x, y, color, 0);
    
    x += 8;
    
    vg_draw_number(x, y, color, m % 10);
    
    x += 8;
    
    if (!_cl_blink)
        SET_PIXEL_NUMBER(pm_doubledot);
    
    x += 8;
    
    /*
     *  Seconds
     */
    
    if (s > 9)
        vg_draw_number(x, y, color, s / 10);
    else
        vg_draw_number(x, y, color, 0);
    
    x += 8;
    
    vg_draw_number(x, y, color, s % 10);
    
    x += 8;
    
    return 0;
}

int vg_draw_number(unsigned long x, unsigned long y, unsigned long color, unsigned int number) {
    if (number < 0 || number > 9)
        return 1;
    
    unsigned int i = 0, j = 0;
    
    switch (number) {
        case 0:
            SET_PIXEL_NUMBER(pm_zero);
            
            break;
            
        case 1:
            SET_PIXEL_NUMBER(pm_one);
            
            break;
            
        case 2:
            SET_PIXEL_NUMBER(pm_two);
            
            break;
            
        case 3:
            SET_PIXEL_NUMBER(pm_three);
            
            break;
            
        case 4:
            SET_PIXEL_NUMBER(pm_four);
            
            break;
            
        case 5:
            SET_PIXEL_NUMBER(pm_five);
            
            break;
            
        case 6:
            SET_PIXEL_NUMBER(pm_six);
            
            break;
            
        case 7:
            SET_PIXEL_NUMBER(pm_seven);
            
            break;
            
        case 8:
            SET_PIXEL_NUMBER(pm_eight);
            
            break;
            
        case 9:
            SET_PIXEL_NUMBER(pm_nine);
            
            break;
            
        default:
            return 1;
            
            break;
    }
    
    return 0;
}

int vg_draw_player(unsigned long x, unsigned long y, unsigned long color) {
    int i, j;
    
    SET_PIXEL_NUMBER(pm_p);
    x += 8;
    
    SET_PIXEL_NUMBER(pm_l);
    x += 8;
    
    SET_PIXEL_NUMBER(pm_a);
    x += 8;
    
    SET_PIXEL_NUMBER(pm_y);
    x += 8;
    
    SET_PIXEL_NUMBER(pm_e);
    x += 8;
    
    SET_PIXEL_NUMBER(pm_r);
    x += 8;
    
    return 0;
}

int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}
