#include "video_gr.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */



/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void *vg_init(unsigned short mode)
{
	struct reg86u reg86;
	vbe_mode_info_t info;
	struct mem_range mr;
	int r;

	reg86.u.b.intno = BIOS_VIDEOCARD;
	reg86.u.w.ax = SET_VBE;
	reg86.u.w.bx =mode | BUFFER_MODE;

	if(sys_int86(&reg86) != OK)
	{
		printf("vg_init(): sys_int86() failed");
		return NULL;
	}


	if(vbe_get_mode_info(mode, &info)!=0)
	{
		printf("vbe_get_mode_info(): sys_int86() failed");
		return NULL;
	}

	unsigned int vram_size = info.XResolution * info.YResolution * info.BitsPerPixel/8;
	//Allow memory mapping

	mr.mr_base = (phys_bytes)info.PhysBasePtr;
	mr.mr_limit = mr.mr_base + vram_size;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);


	//	if(video_mem == MAP_FAILED)
	///	panic("couldn't map video memory");

	h_res= info.XResolution;
	v_res = info.YResolution;
	bits_per_pixel = info.BitsPerPixel;

	return video_mem;
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

unsigned get_h_res() {
	return h_res;
}

unsigned get_v_res() {
	return v_res;
}

unsigned get_bitsperpixel() {
	return bits_per_pixel;
}

char* get_video_mem() {
	return video_mem;
}


