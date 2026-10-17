#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>

#include "vbe.h"
#include "video_gr.h"

#include "util.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab I304 is 0xF0000000
 * Better run my version of lab2 as follows:
 *     service run `pwd`/lab2 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR 0xE0000000
#define H_RES       640
#define V_RES    480
#define BITS_PER_PIXEL   24

/* Private global variables */

static char *video_mem;  /* Process address to which VRAM is mapped */
static char *video_buffer;


static unsigned int h_res;  /* Horizontal screen resolution in pixels */
static unsigned int v_res;  /* Vertical screen resolution in pixels */
static unsigned int bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned int size;

static int video_bufferOx,video_bufferOy;

void * vg_init(unsigned short mode) {

 /* MUDAR O MODO DA PLACA GRAFICA */
 struct reg86u r; // Abstraï¿½ï¿½o para os registos do processador
 r.u.w.ax = 0x4F02; //0x4F = codigo da BIOS; 0x02 codigo para mudar o modo da placa grafica
 r.u.w.bx = 1<<14 | mode; //Codigo do modo da placa grafica
 r.u.b.intno = 0x10; // Codigo da Interrupï¿½ï¿½o
 if (sys_int86(&r) != OK) { // Chamada ï¿½ funï¿½ï¿½o que cria a Interrupï¿½ï¿½o
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
	    size = (h_res*v_res*bits_per_pixel)>>3;

	    printf("Mode Atributes: %.5x, h_res: %d, v_res: %d, bpp: %d, size: %d\n",vmi.ModeAttributes, h_res,v_res,bits_per_pixel,size);


	    if (r.u.b.ah == 0x01) {
	    	printf("Erro a mudar o modo da placa grafica.\n");
	    	return NULL;
	    }

 /* MAPEAR A MEMORIA PARA O ESPAÇO DE MEMORA DO PROCESSO */

    int r;
    struct mem_range mr;

    /* Allow memory mapping */

    mr.mr_base = vram_phys_addr;
    mr.mr_limit = mr.mr_base + size;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
     panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, size);

    if(video_mem == MAP_FAILED) {
     printf("Video Ram Mapping Failed.\n");
     video_mem = NULL;
     return NULL;
    }

    /* Criar Buffer */

    video_buffer = malloc(size);
    return video_mem;
 }
}


int vg_fill(unsigned long color) {
	char * p = video_buffer;
	int i;
	for(i = 0; i < h_res*v_res; i++) {
		memcpy(p,&color,3);
		p += (bits_per_pixel>>3);
	}
	return 0;
}


int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color) {

	if ((x > h_res) || ( y > v_res)) return -1;
	else
	video_buffer[y*h_res+x] = color;
 return 0;
}

int vg_draw_rectangle(int x, int y, int w, int h, unsigned long color) {
	x = min(max(x,0),h_res-1);
	y = min(max(y,0),v_res-1);
	w = min(x+w,h_res) - x;
	h = min(y+h,v_res) - y;
	int i,j; unsigned char* p;
	for(i = 0; i < h; i++) {
		p = video_buffer + 3*((y+i)*h_res + x);
		for(j = 0; j < w; j++) {
		memcpy(p, &color, bits_per_pixel>>3);
		p+=3;
		}
	}
};

long vg_get_pixel(unsigned long x, unsigned long y) {

	return video_buffer[y*h_res+x];

}

int vg_exit() {
	/* free buffer */
	//free(video_buffer);

  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  }
  //else printf("%x\n",video_mem);
   return 0;
}


int vg_set_buffer_offset(int ox, int oy) {
	video_bufferOx = ox;
	video_bufferOy = oy;
}

int vg_swap_buffers() {
	if (video_bufferOx == 0 && video_bufferOy == 0)
		memcpy(video_mem, video_buffer, size);
	else {
		int i,j, p;
		for(i = 0; i < v_res; i++) {
			if (i + video_bufferOy < 0 || i + video_bufferOy >= v_res) continue;
			for(j = 0; j < h_res; j++) {
				if (j + video_bufferOx < 0 || j + video_bufferOx >= h_res) continue;
				p = 3*((i + video_bufferOy) * h_res + j + video_bufferOx);

				memcpy(video_mem + p, video_buffer + 3*(i*h_res + j), 3);
			}
		}

	}
	return 0;
}

screen_info vg_getScreenInfo() {
	screen_info r;
	r.screen = video_buffer;
	r.WIDTH = h_res;
	r.HEIGHT = v_res;
	r.BPP = bits_per_pixel;
	return r;
};

char* vg_getScreen() {return video_buffer;}
