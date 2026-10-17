#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "lmlib.h"
#include "i8254.h"
#include "i8042.h"
#include "graphics.h"

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static char *double_buffer; /*Images drawn here*/

static char *mouse_buffer; /*Images copied and mouse drawn*/

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
  } else{
	  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
      return 0;
  }
}

void *vg_init(unsigned short mode){

	/* To be completed */

	vbe_mode_info_t vmi_p;
	struct reg86u reg86;
	struct mem_range mr;
	unsigned long size_VRAM = 0;

	reg86.u.b.intno = VIDEO_CARD;
	reg86.u.w.ax = VBE_SET_MODE;
	reg86.u.w.bx = (LINEAR_MODE | mode);

	if (sys_int86(&reg86) != OK) {
		return NULL;
	}

	switch (reg86.u.w.ax) {
		case VBE_CALL_FAILED:
			return NULL;
			break;
		case VBE_UNSUPPORTED:
			return NULL;
			break;
		case VBE_INVALID_MODE:
			return NULL;
			break;
		default:
			break;
	}

	if (lm_init() == NULL) {
		return NULL;
	}

	if (vbe_get_mode_info(mode, &vmi_p) != OK) {
		return NULL;
	}

	//global variables provided
	h_res = vmi_p.XResolution;
	v_res = vmi_p.YResolution;
	bits_per_pixel = vmi_p.BitsPerPixel;

	size_VRAM = h_res*v_res*bits_per_pixel/8;

	mr.mr_base = (phys_bytes)(vmi_p.PhysBasePtr);
	mr.mr_limit = mr.mr_base + size_VRAM;

	if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK){
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n");
		//return NULL;
	}

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, size_VRAM);

	if(video_mem == MAP_FAILED){
		return NULL;
	}

	double_buffer = (char*) malloc(size_VRAM);

	mouse_buffer = (char*) malloc(size_VRAM);

	return video_mem;
}

void set_pixel(unsigned short x, unsigned short y, unsigned short color) {

	*(double_buffer+ y*h_res + x) = (char) color;
	//*(double_buffer+ y*h_res*(bits_per_pixel/8) + x*(bits_per_pixel/8)) = (char) color;

	return;
}

int vg_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

	if ((x < 0) || (x >= h_res) || (y < 0) || (y >= v_res) || (color < 0) || (color > 256)) {
		return 1;
	}

	int height, width;
	unsigned int bytes_pixel;
	char *pixel;

	pixel = double_buffer;

	if ((bits_per_pixel/8) <= 0)
		bytes_pixel = 1;
	else
		bytes_pixel = bits_per_pixel/8;


	for (height = 0; height < size; height++){

		pixel += (y + height) * bytes_pixel * h_res + x * bytes_pixel;

		for (width = 0; width < size; width++){
			*pixel = (char) color;
			pixel += bytes_pixel;
		}

		pixel = double_buffer;
	}

	return 0;
}

int vg_line(unsigned short xi, unsigned short yi,  unsigned short xf, unsigned short yf, unsigned long color){

	if ((xi < 0) || (xi >= h_res) || (yi < 0) || (yi >= v_res) || (color < 0) || (color > 256)) {
		return 1;
	}

	if ((xf < 0) || (xf >= h_res) || (yf < 0) || (yf >= v_res) || (color < 0) || (color > 256)) {
		return 1;
	}

	unsigned int i;

	if(xf == xi){
		for(i = yi; i <= yf; i++){
			set_pixel(xi, i, color);
		}

		return 0;
	}

	if (yf == yi){
		for (i = xi; i < xf; i++){
			set_pixel(i, yi, color);
		}

		return 0;
	}

	unsigned int diff;
	float slope = ((double) (yf - yi)) / (xf - xi);//slope of the straight line between the 2 points

	if (slope < 0)
		diff = 1;
	else diff = 0;

	if ((slope > -1) && (slope < 1)) {
		for (i = xi; i <= xf; i++) {
			set_pixel(i, diff * yi + (int) (i * slope), color);//"calculates" the next point
		}
	} else {
		for (i = yi; i <= yf; i++) {
			set_pixel(diff * xi + (int) (i / slope), i, color);//"calculates" the next point
		}
	}

	return 0;
}

void clear_screen(){
/*	unsigned int i;
	unsigned int j;
	unsigned short color; color = 0;
	for(i = 0; i < v_res; i++){
		for(j = 0; j < h_res; j++){
			set_pixel(j, i, color);
		}
	}*/
	memset(double_buffer, (char) 0, v_res*h_res*bits_per_pixel/8);
}

unsigned get_hres(){
	return h_res;
}


unsigned get_vres() {
	return v_res;

}

int get_bitsPerPixel(){
	return bits_per_pixel;
}

char* getGraphicsBuffer(){
	return double_buffer;
}

void flip_Mouse(){
	memcpy(mouse_buffer, double_buffer, h_res*v_res*(bits_per_pixel/8));
	//clear_screen();
}

void flip_Display(){
	memcpy(video_mem, mouse_buffer, h_res*v_res*(bits_per_pixel/8));
	//memset(mouse_buffer, 0, v_res*h_res*bits_per_pixel/8);
	//clear_screen();
}
