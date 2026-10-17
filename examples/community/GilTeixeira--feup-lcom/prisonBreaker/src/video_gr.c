#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "defs.h"

/* Private global variables */

static short *video_mem; /* Process (virtual) address to which VRAM is mapped */
static short *buffer; /* The Buffer in which the Bitmaps are written */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

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

void *vg_init(unsigned short mode) {

	struct reg86u reg86;
	vbe_mode_info_t vmi_p;

	reg86.u.b.ah = VBE_CALL; // VBE call,
	reg86.u.b.al = SET_VBE_MODE; //function 02 -- set VBE mode
	reg86.u.w.bx = SET_LINEAR_FRAME_BUFFER | mode; // set bit 14| mode
	reg86.u.b.intno = VIDEO_CARD_INTERRUPT;

	if (sys_int86(&reg86) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	if (vbe_get_mode_info(mode, &vmi_p) != OK) {
		return NULL;
	}

	h_res = vmi_p.XResolution;
	v_res = vmi_p.YResolution;
	bits_per_pixel = vmi_p.BitsPerPixel;

	int r;
	struct mem_range mr;
	unsigned int vram_base = vmi_p.PhysBasePtr; /* VRAM's physical addresss */
	unsigned int vram_size = h_res * v_res * bits_per_pixel / 8; /* VRAM's size, but you can use the frame-buffer size, instead */

	/* Allow memory mapping */

	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;

	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
	buffer = (short*) malloc(h_res * v_res * bits_per_pixel / 8);

	if (video_mem == MAP_FAILED)
		panic("couldn't map video memory");

	return video_mem;

}

void flipDisplay() {
	memcpy(video_mem, buffer, h_res * v_res * bits_per_pixel / 8);

}

short getHorResolution() {
	return h_res;
}

short getVerResolution() {
	return v_res;
}

char* getGraphicsBuffer() {
	return (char *) buffer;
}

void setColorPixel(int x, int y, short color, short * ptr) {
	*(ptr + y * h_res + x) = color;
	int offset = (y * h_res) + x;

}

int validCoord(int x, int y) {
	if (x < h_res && y < v_res)
		return 1;
	else
		return 0;

}
