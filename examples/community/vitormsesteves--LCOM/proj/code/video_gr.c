#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "video_gr.h"
#include "i8042.h"

void* vg_init(unsigned short mode) {
	struct mem_range mr;
	vbe_mode_info_t vmi_p;
	struct reg86u reg86;

	reg86.u.b.intno = BIOS_VIDEO;
	reg86.u.b.ah = VBE;
	reg86.u.b.al = SET_VBE;
	reg86.u.w.bx = BIT(LINEAR_BIT) | mode;

	if (sys_int86(&reg86)) {
		printf("vg_init()::bios call didn't return 0\n");
		return NULL;
	}

	if (vbe_get_mode_info(mode, &vmi_p) == -1) {
		printf("vg_init()::failed in vbe_get_mode_info");
		return NULL;
	}

	h_res = vmi_p.XResolution;
	v_res = vmi_p.YResolution;
	bits_per_pixel = vmi_p.BitsPerPixel;
	if (bits_per_pixel / 8 > 0)
		bytes_per_pixel = bits_per_pixel / 8;
	else
		bytes_per_pixel = 1;
	vram_size = h_res * v_res * bytes_per_pixel;

	int r;

	/* Allow memory mapping */

	mr.mr_base = vmi_p.PhysBasePtr;
	mr.mr_limit = mr.mr_base + vram_size;

	if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0) {
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);
		return NULL;
	}

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
	buffer = malloc(vram_size);

	if (video_mem == MAP_FAILED) {
		panic("video_txt couldn't map video memory");
		return NULL;
	}

	return video_mem;

}

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = BIOS_VIDEO; /* BIOS video services */

	reg86.u.b.ah = VIDEO; /* Set Video Mode function */
	reg86.u.b.al = SET_TEXT; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else {
		return 0;
	}
}

void draw_pixel(unsigned int x, unsigned int y, int color, char* buf) {
	int i;
	char* vptr;

	if (x >= h_res || y >= v_res) {
		return;
	}

	i = (y * h_res + x) * bytes_per_pixel;

	vptr = &buf[i];

	for (i = 0; i < bytes_per_pixel; i++) {
		*vptr = (char) color;
		vptr++;
		color >>= 8;
	}
}

int draw_rectangle(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, int color, char* buf) {

	if (xf < xi) {
		SWAP(xi, xf);
	}

	if (yf < yi) {
		SWAP(yi, yf);
	}

	if (xf > h_res)
		xf = h_res;
	if (yf > v_res)
		yf = v_res;
	unsigned short x, y;

	for (x = xi; x < xf; x++) {
		for (y = yi; y < yf; y++) {
			draw_pixel(x, y, color, buf);
		}
	}
	return 0;
}

// algoritmo http://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles
void draw_circle(int x0, int y0, int radius, int color, char* buf) {
	int x = radius;
	int y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;
	int i;
	while (x >= y) {
		for (i = x0 - x; i <= x0 + x; i++) {
			draw_pixel(i, y0 + y, color, buf);
			draw_pixel(i, y0 - y, color, buf);
		}
		for (i = x0 - y; i <= x0 + y; i++) {
			draw_pixel(i, y0 + x, color, buf);
			draw_pixel(i, y0 - x, color, buf);
		}

		y++;
		radiusError += yChange;
		yChange += 2;
		if (((radiusError << 1) + xChange) > 0) {
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}
}

void see_circle(int x0, int y0, int radius, int color, char* buf) {
	int x = radius;
	int y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;
	int i, j;
	for (i = 0; i < y0-x; i++)
		for (j = 0; j < h_res; j++)
			draw_pixel(j, i, color, buf);
	for (i = y0+x; i < v_res; i++)
			for (j = 0; j < h_res; j++)
				draw_pixel(j, i, color, buf);
	while (x >= y) {
		for (i = 0; i <= x0 - x; i++) {
			draw_pixel(i, y0 + y, color, buf);
			draw_pixel(i, y0 - y, color, buf);
		}
		for (i = x0 + x; i <= getH_res(); i++) {
			draw_pixel(i, y0 + y, color, buf);
			draw_pixel(i, y0 - y, color, buf);
		}
		for (i = 0; i <= x0 - y; i++) {
			draw_pixel(i, y0 + x, color, buf);
			draw_pixel(i, y0 - x, color, buf);
		}
		for (i = x0 + y; i <= getH_res(); i++) {
			draw_pixel(i, y0 + x, color, buf);
			draw_pixel(i, y0 - x, color, buf);
		}

		y++;
		radiusError += yChange;
		yChange += 2;
		if (((radiusError << 1) + xChange) > 0) {
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}
}

char* getGraphicsBuffer() {
	return buffer;
}

void flipScreen() {
	memcpy(video_mem, buffer, vram_size);
}

void cleanScreen() {
	memset(getGraphicsBuffer(), 0, vram_size);
}

unsigned getH_res() {
	return h_res;
}

unsigned getV_res() {
	return v_res;
}

int rgb(unsigned char r, unsigned char g, unsigned char b) {
	if (r < 0 || 255 < r || g < 0 || 255 < g || b < 0 || b > 255)
		return -1;
	int red = r * 31 / 255;
	int green = g * 63 / 255;
	int blue = b * 31 / 255;

	return (red << 11) | (green << 5) | blue;
}
