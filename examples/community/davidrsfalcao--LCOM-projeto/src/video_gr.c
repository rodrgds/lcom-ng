#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "lmlib.h"


#include "vbe.h"


/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xE0000000
#define H_RES             1024
#define V_RES		      768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *s_buffer = NULL;
static char *video_mem;		/* Process address to which VRAM is mapped */
static long vram_size;
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


void *vg_init(unsigned short mode) {

	lm_init();

	if ( vbe_set_mode(VBE_MODE, mode) == 1)
	{
		return NULL;
	}


	h_res = H_RES ;
	v_res = V_RES ;
	bits_per_pixel = BITS_PER_PIXEL;
	vram_size = (H_RES * V_RES * BITS_PER_PIXEL) / 8;


	int r;
	struct mem_range mr;

	mr.mr_base = (VRAM_PHYS_ADDR);
	mr.mr_limit = mr.mr_base + (H_RES * V_RES * BITS_PER_PIXEL) / 8;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, (H_RES * V_RES * BITS_PER_PIXEL)  / 8);

	s_buffer = (char *) malloc(vram_size);

	if(video_mem == MAP_FAILED)
		panic("video_txt couldn't map video memory");


	if (video_mem == NULL)
	{
		printf("vg_init: Error!\n");
		vg_exit();
		return NULL;
	}
	return video_mem;
}

char* getSBuffer()
{
	return s_buffer;
}

char* getVideoMem(){

	return video_mem;
}

unsigned getHRes(){

	return h_res;
}

unsigned getVRes(){

	return v_res;
}

void clear_screan()
{
	memset(video_mem, 0, h_res * v_res);
}

void clear_double()
{
	memset(s_buffer, 0, h_res * v_res);
}

void updateBuffer()
{
	unsigned vram_size = (H_RES * V_RES * BITS_PER_PIXEL) / 8;
	memcpy(video_mem , s_buffer , vram_size);
}
