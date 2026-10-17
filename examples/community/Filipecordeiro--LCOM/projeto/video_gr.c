#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "log.h"

#include "video_gr.h"
#include "read_xpm.h"
#include "vbe.h"

char rgb(short r, short g, short b){

  int red = r*31/255;
  int green = g*63/255;
  int blue = b*31/255;

  return (red << 11) | (green<<5) | blue;
}

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */



void *vg_init(unsigned short mode)
{
	struct reg86u r;
	r.u.w.ax = SET_VBE_MODE; 					// VBE call, function 02 - set VBE mode
	r.u.w.bx = BIT(LINEAR_MODEL_BIT) | mode; 	// set bit 14: linear framebuffer
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ){
		printf("vg_init(%u): sys_int86() failed \n", mode);
		return NULL;
	}
	/* Get mode info */
	vbe_mode_info_t vmi_p;
	if (0 != vbe_get_mode_info(mode, &vmi_p)){
		printf("vg_init(%u): vbe_get_mode_info() failed \n", mode);
		return NULL;
	}

	h_res = vmi_p.XResolution;
	v_res = vmi_p.YResolution;
	bits_per_pixel = vmi_p.BitsPerPixel;
	void * vram_base = (void *)vmi_p.PhysBasePtr;
	vram_size = h_res*v_res*bits_per_pixel/8;

	int res;
	struct mem_range mr;

	/* Allow memory mapping */
	mr.mr_base = (phys_bytes)vram_base;
	mr.mr_limit = mr.mr_base + vram_size;
	if( OK != (res = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		printf("video_gr: sys_privctl (ADD_MEM) failed: %d\n", res);

	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
	if(video_mem == MAP_FAILED) printf("video_gr couldn't map video memory");

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

int vg_clear(char *address){
  char *ptr = address;
  unsigned int i;
  for(i=0;i<vram_size;i++){
    if(*ptr!=0) *ptr=0;
    ptr++;
  }
  return (int)ptr;
}

int vg_drawSquare(char *address, unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

  char * ptr;
  unsigned short i, j;
  for(i=0; i < size; i++){
    ptr = address + x + (y+i)*h_res;
    for( j=0; j < size; j++){
      *ptr = color;
      ptr++;
    }
  }
  return (int)ptr;
}

int vg_drawRectangle(char *address, unsigned short x, unsigned short y, unsigned short h, unsigned short w, unsigned long color) {
  char * ptr;
  unsigned short i, j;
  for(i=0; i < h; i++){
    ptr = address + x + (y+i)*h_res*bits_per_pixel/8;
    if(ptr > address+vram_size){
      Log("####ERROR#### Video address OUT_OF_RANGE!\n");
      return 0;
    }
    for( j=0; j < w; j++){
      *ptr = color;
      ptr++;
    }
  }
  return (int)ptr;
}

int vg_drawLine(char *address, unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color){

  char *ptr;
  /*
   * Bresenham's line algorithm
   */
  int x0 = xi,
      x1 = xf,
      y0 = yi,
      y1 = yf;
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    ptr = address + x0 + y0*h_res;
    *ptr = color;

    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
  /*
   * -----------------------
   */
  return (int)ptr;
}

int vg_drawXPM(char *address, unsigned short xi, unsigned short yi, char *xpm[]){
  int wd, ht;
  char *pix = read_xpm(xpm, &wd, &ht);

  char *ptr;

  if(xi > h_res-wd) xi = h_res-wd;
  if(yi > v_res-ht) yi = v_res-ht;

  unsigned short i,j, pixel = 0;
  for(i=0; i < ht; i++){
    ptr = address + xi + (yi+i)*h_res;
    for( j=0; j < wd; j++){
      *ptr = pix[pixel];
      pixel++;
      ptr++;
    }
  }
  return (int)ptr;
}

