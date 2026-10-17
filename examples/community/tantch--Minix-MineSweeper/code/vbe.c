static char *video_mem; /* Process address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	mmap_t address;
	struct reg86u r;

	lm_init();

	lm_alloc(256, &address);

	r.u.w.ax = 0x4F01;
	r.u.w.es = PB2BASE(address.phys);
	r.u.w.di = PB2OFF(address.phys);
	r.u.w.cx = 1 << 14 | mode;
	r.u.b.intno = 0x10;
	sys_int86(&r);
	vbe_unpack_mode_info(address.virtual, vmi_p);
	lm_free(&address);
	return 1;
}

static void vbe_unpack_mode_info(void *buf, vbe_mode_info_t *dst) {

	vbe_mode_info_t *src = buf;

	dst->XResolution = src->XResolution;
	dst->YResolution = src->YResolution;

	dst->BitsPerPixel = src->BitsPerPixel;

	dst->PhysBasePtr = src->PhysBasePtr;

}

void * vg_init(unsigned short mode) {

	struct reg86u reg86;
	vbe_mode_info_t info;
	struct mem_range range;
	int r;

	reg86.u.b.intno = 0x10;
	reg86.u.w.ax = 0x4F02;
	reg86.u.w.bx = 1 << 14 | mode;

	if (sys_int86(&reg86) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	vbe_get_mode_info(mode, &info);
	range.mr_base = info.PhysBasePtr;
	range.mr_limit = range.mr_base
			+ info.XResolution * info.YResolution * info.BitsPerPixel / 8;

	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &range)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);
	video_mem = vm_map_phys(SELF, (void *) range.mr_base,
			info.XResolution * info.YResolution * info.BitsPerPixel / 8);
	h_res = info.XResolution;
	v_res = info.YResolution;
	bits_per_pixel = info.BitsPerPixel;
	return NULL;
}


void vg_set_pixel(int i,unsigned long color) {

	*(video_mem + i) = color;
}

long vg_get_pixel(unsigned long x, unsigned long y) {
	if ((x >= 0) && (x <= h_res) && (y >= 0) && (y <= v_res)) {
		return *(video_mem + bits_per_pixel * (x + h_res * y));
	}
	return -1;
}

int vg_draw_line(unsigned long xi, unsigned long yi, unsigned long xf,
		unsigned long yf, unsigned long color) {

	unsigned long x, y, div, r, i, p, j;
	long dx, dy, fy;
	int s = 0;

	dx = xf - xi;

	dy = yf - yi;

	if (dy < 0) {
		dy = dy * -1;
		s = 1;
	}

	if (xi == xf && yi == yf) {
		set_pixel(xi, yi, color);
		return 0;
	}

	if (xi == xf) {
		x = xi;
		y = yi;
		do {
			set_pixel(x, y, color);
			if (s)
				y--;
			else
				y++;
		} while ((y <= yf && s == 0) || (y >= yf && s == 1));
		return 0;
	}
	if (yi == yf) {
		x = xi;
		y = yi;
		do {
			set_pixel(x, y, color);
			x++;
		} while (x <= xf);
		return 0;
	}

	if (dx == dy) {
		x = xi;
		y = yi;
		do {
			set_pixel(x, y, color);
			x++;
			if (s)
				y--;
			else
				y++;
		} while (x <= xf);
		return 0;
	}
	if (dx > dy) {
		x = xi;
		y = yi;
		div = dx / (dy + 1);
		r = dx % (dy + 1);
		if (r != 0)
			p = dy / r;
		j = 0;
		do {
			for (i = 0; i < div; i++) {
				set_pixel(x, y, color);
				x++;
			}
			if (r != 0 && j % p == 0) {
				set_pixel(x, y, color);
				x++;
				r--;
			}
			j++;
			if (s)
				y--;
			else
				y++;

		} while ((y <= yf && s == 0) || (y >= yf && s == 1));
		return 0;
	}
	if (dy > dx) {
		x = xi;
		y = yi;
		div = dy / (dx + 1);
		r = dy % (dx + 1);
		if (r != 0)
			p = dy / r;
		j = 0;
		do {
			for (i = 0; i < div; i++) {
				set_pixel(x, y, color);
				if (s)
					y--;
				else
					y++;
			}
			if (r != 0 && j % p == 0) {
				set_pixel(x, y, color);
				if (s)
					y--;
				else
					y++;
				r--;
			}
			x++;
			j++;
		} while (x <= xf);
		return 0;
	}

}


int vg_exit() {
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

