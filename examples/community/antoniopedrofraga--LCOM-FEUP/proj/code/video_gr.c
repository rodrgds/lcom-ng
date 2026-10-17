#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "vbe.h"




/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

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



void *vg_init(unsigned short mode){

	struct mem_range mr;
	int ar;
	struct reg86u r;
	r.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = 1<<14|mode; // set bit 14: linear framebuffer
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
	}


	vbe_mode_info_t vmi;
	vbe_get_mode_info(mode, &vmi);

	h_res = vmi.XResolution;
	v_res = vmi.YResolution;
	bits_per_pixel = vmi.BitsPerPixel;

	mr.mr_base = vmi.PhysBasePtr;
	mr.mr_limit = mr.mr_base + (vmi.XResolution * vmi.YResolution * vmi.BitsPerPixel / 8);

	if( OK != (ar = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("vg_init: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, (vmi.XResolution * vmi.YResolution * vmi.BitsPerPixel / 8));

	if(video_mem == MAP_FAILED)
		panic("vg_init couldn't map video memory");

	return video_mem;
}
void * vg_adress(){
	return video_mem;
}

short get_v_res(){
	return v_res;
}

short get_h_res(){
	return h_res;
}
