#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"

/* Constants for VBE 0x105 mode */

#define VRAM_PHYS_ADDR    0xD0000000
#define H_RES             800
#define V_RES		  600
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned hres;		/* Horizontal screen resolution in pixels */
static unsigned vres;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void * vg_init(unsigned long mode) {

	struct reg86u registers;
	struct mem_range range;
	vbe_mode_info_t vmi;
	registers.u.b.intno = 0x10;
	registers.u.b.ah = 0x4F;
	registers.u.b.al = 0x02;
	registers.u.w.bx = 1 << 14 | mode;
	sys_int86(&registers);
	vbe_get_mode_info(mode, &vmi);
	range.mr_base = vmi.PhysBasePtr;
	range.mr_limit = range.mr_base + vmi.XResolution * vmi.YResolution *(vmi.BitsPerPixel / 8);
	sys_privctl(SELF, SYS_PRIV_ADD_MEM, &range);
	video_mem = vm_map_phys(SELF, (void *) range.mr_base, vmi.XResolution * vmi.YResolution * (vmi.BitsPerPixel / 8));
	vres = vmi.YResolution;
	hres = vmi.XResolution;
	bits_per_pixel = vmi.BitsPerPixel;
	vg_fill(00);
	return NULL;
}

int vg_fill(unsigned long color) {
	int i;
	if(color<0)
		return 1;
	for(i=0;i<hres*vres;i++)
	{
		*(video_mem+i)=color;
	}
	return 0;
}

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color) {
	int pos;
	if(y>vres-1||x>hres-1||x<0||y<0||color<0)
	{
		  return -1;
	}
	pos=(x+y*hres);
	*(video_mem+pos)=color;
	return 0;
}

long vg_get_pixel(unsigned long x, unsigned long y) {
	int pos;
	if(y>vres-1||x>hres-1||x<0||y<0)
	{
		  return -1;
	}
	pos=(x+y*hres);
	return *(video_mem+pos);
}

int vg_draw_line(unsigned long xi, unsigned long yi, 
		 unsigned long xf, unsigned long yf, unsigned long color) {
	int pos, temp, i;
	float m, b, y;
	if(yi>vres-1||yf>vres-1||xi>hres-1||xf>hres-1||xi<0||xf<0||yi<0||yf<0||color<0)
	{
		  return 1;
	}

	if(xf<xi)
	{
		temp=xf;
		xf=xi;
		xi=temp;
	}
	if(yf<yi)
	{
		temp=yf;
		yf=yi;
		yi=temp;
	}
	if(xi==xf)
	{
		for (i=0; i<yf-yi; i++)
			vg_set_pixel(xi, yi+i, color);
		return 0;
	}
	if(yi==yf)
	{
		for (i=0; i<xf-xi; i++)
			vg_set_pixel(xi+i, yi, color);
		return 0;
	}
	m=(float)(yf-yi)/(float)(xf-xi);
	b=yi-m*xi;
	for(i=xi;i<=xf;i++)
	{
		y=m*i+b;
		vg_set_pixel(i,y,color);
	}

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

void printChar(unsigned long x, unsigned long y, char ch, unsigned long fg, unsigned long bg){
}
