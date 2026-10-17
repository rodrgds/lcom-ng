#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "game.h"

/* Constants for VBE 0x105 mode */
/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000
 * Currently on lab I304 is 0xF0000000
 * Better run my version of lab2 as follows:
 *     service run `pwd`/lab2 -args "mode 0x105"
 */

#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */
static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void * vg_init(unsigned short mode) {
    
	struct  reg86u reg86;
	struct mem_range mr;
	int r;
    
	vbe_mode_info_t vmi;
    
	vbe_get_mode_info(mode, &vmi);
    
    
    
	/* set video mode */
    
	reg86.u.b.intno = 0x10;	// BIOS video service
	reg86.u.b.ah = 0x4F;	// set video mode
	reg86.u.b.al = 0x02;  // video mode
	reg86.u.w.bx = 1 << 14 |  mode;
    
    
	if( OK != sys_int86(&reg86) )
	{
		printf("set_video_mode error");
		return NULL;
	}
    
	mr.mr_base = vmi.PhysBasePtr;
    
	mr.mr_limit = mr.mr_base + vmi.XResolution * vmi.YResolution*(vmi.BitsPerPixel);
    
    
	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);
    
    
	video_mem = vm_map_phys(SELF,(void *) mr.mr_base, vmi.XResolution * vmi.YResolution * (vmi.BitsPerPixel));
    
    
	if(video_mem == MAP_FAILED)
		panic("video_txt couldn't map video memory");
    
    
	h_res = vmi.XResolution;
	v_res = vmi.YResolution;
	bits_per_pixel = vmi.BitsPerPixel;
    
	return NULL;
}

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color) {
    
	// if position not valid
	if(x < 0 || x > h_res || y < 0 || y > v_res)
		return -1;
	else
		*(video_mem + x + y*h_res) = color;	// ini_position + x_position + y_position
	return 0;
}

long vg_get_pixel(unsigned long x, unsigned long y) {
	return *(video_mem + x + y*h_res);
}

void swap(unsigned long *a, unsigned long *b)
{
	unsigned long tmp;
	tmp = *a;
	*a=*b;
	*b = tmp;
}

void drawCube(unsigned long x, unsigned long h, unsigned long y, unsigned long w, unsigned int color, unsigned int border){
	// if position not valid
	if(x < 0 || x+h >h_res || y < 0 || y+w > v_res){
		return;
	}
    unsigned long x_init = x;
    unsigned long y_init = y;
	unsigned long h_aux = x+h;
	unsigned long w_aux = y+w;
	unsigned long x_aux = x;
	for( ; y < w_aux ; y++){
		for( ; x_aux < h_aux ; x_aux++){
			vg_set_pixel(x_aux,y, color);
			
				if(y==(w_aux-1)) {
	                vg_set_pixel(x_aux,y,border);
	            } else if (x_aux==(h_aux-1)) {
	                vg_set_pixel(x_aux,y,border);
	            }
	            else if (x_aux==STARTBOARDX) {
	                vg_set_pixel(x_aux,y,border);
	            }
	            else if (y==STARTBOARDY) {
	                vg_set_pixel(x_aux,y,border);
	            }
			
            
        }

    x_aux = x;
}
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




