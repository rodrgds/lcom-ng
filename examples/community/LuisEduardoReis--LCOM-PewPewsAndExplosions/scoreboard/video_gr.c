#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>

#include "vbe.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab I304 is 0xF0000000
 * Better run my version of lab2 as follows:
 *     service run `pwd`/lab2 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR 0xE0000000
#define H_RES             1024
#define V_RES    768
#define BITS_PER_PIXEL   8

/* Private global variables */

static char *video_mem;  /* Process address to which VRAM is mapped */

static unsigned h_res;  /* Horizontal screen resolution in pixels */
static unsigned v_res;  /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void * vg_init(unsigned short mode) {

 /* MUDAR O MODO DA PLACA GRAFICA */
 struct reg86u r; // Abstração para os registos do processador
 r.u.w.ax = 0x4F02; //0x4F = codigo da BIOS; 0x02 codigo para mudar o modo da placa grafica
 r.u.w.bx = 1<<14 | mode; //Codigo do modo da placa grafica
 r.u.b.intno = 0x10; // Codigo da Interrupção
 if (sys_int86(&r) != OK) { // Chamada à função que cria a Interrupção
  printf("set_vbe_mode: sys_int86() failed \n");
  return NULL;
 }
 else {

	 /* ADQUIRIR INFO SOBRE O MODO DA PLACA GRAFICA */
	    vbe_mode_info_t vmi;

	    vbe_get_mode_info(mode, &vmi);

	    phys_bytes vram_phys_addr =  vmi.PhysBasePtr;//VRAM_PHYS_ADDR;
	    h_res = vmi.XResolution;
	    v_res = vmi.YResolution;
	    bits_per_pixel = vmi.BitsPerPixel;


 /* MAPEAR A MEMORIA PARA O ESPAÇO DE MEMORA DO PROCESSO */

    int r;
    unsigned long size = h_res*v_res*(bits_per_pixel<<3);
    struct mem_range mr;

    /* Allow memory mapping */

    mr.mr_base = vram_phys_addr;
    mr.mr_limit = mr.mr_base + size;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
     panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, size);

    if(video_mem == MAP_FAILED) {
     video_mem = NULL;
     return NULL;
    }

    return video_mem;
 }
}

int vg_demo(unsigned short mode)
{
	if (vg_init(mode)!=NULL) {       /* Change to video mode */

		int i;

		for ( i = 0; i < 5; i++) {
			vg_draw_demo(rand() %64);
			sleep(2);
					 }
		 }


}

int vg_draw_demo(unsigned long color) {
	// left square
	vg_square(0,h_res/2 - 3, 0,v_res - 1, color);

	//horizontal lines
	int i; unsigned long x,y,c;
	for(i = 0; i<8; i++) {
		c = 20 + 3*i;
		y = i*(v_res/16);
	vg_square(h_res/2,h_res-1, y, y +(v_res/16)-1,c);
	}
	//vertical lines
	for(i = 0; i<8; i++) {
		c = 30 + 2*i;

		x = i*(h_res/16) + h_res/2;

		vg_square( x, x+(h_res/16)-1, v_res/2, v_res-1, c);
	}

}


int vg_square(unsigned long x1, unsigned long x2,
		unsigned long y1, unsigned long y2, unsigned long color) {
	if (x1 < 0 || x2<0 || y1 < 0 || y2 < 0 ||
		x1 >=h_res || x2 >= h_res || y1 >= v_res || y2>= v_res) return 1;
	char * p = video_mem + (x1 + y1*h_res);
	char * t;

	int i, j;

	for (i = 0; i < (y2 - y1) ; i++ )
	{
		t = p;
		for (j = 0; j < (x2 - x1) ; j++) {
			*t = color;
			t++;
		}
		p+=h_res;
	}
	return 0;
}



int vg_fill(unsigned long color) {
 char * p = video_mem;
 int i;
 for(i = 0; i < h_res*v_res; i++) {
  *p = color;
  p+= (bits_per_pixel>>3);
 }
 return 0;
}

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color) {

	if ((x > h_res) || ( y > v_res)) return -1;
	else
	video_mem[y*h_res+x] = color;
 return 0;
}

long vg_get_pixel(unsigned long x, unsigned long y) {

	return video_mem[y*h_res+x];

}

int vg_draw_line(unsigned long xi, unsigned long yi, 
   unsigned long xf, unsigned long yf, unsigned long color) {

	int i;
	 signed long dx,dy;
	 dx = xf - xi;
	 dy = yf - yi;
	 if (dx!=0) {
	  for(i = xi; i<=xf; i++) {
	   vg_set_pixel(i, yi + (dy*(i - xi)/dx),color);
	  }}
	 if (dy!=0) {
	  for(i = yi; i<=yf; i++) {
	   vg_set_pixel(xi + (dx*(i - yi)/dy),i,color);
	  }}
	 return 0;

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
   printf("%x\n",video_mem);
      return 0;
}
