#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "graphics.h"


/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES           1024
#define V_RES		  	768
#define BITS_PER_PIXEL  8
/* Private global variables */

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static char *doubleBuffer;


static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
unsigned int vram_size;

int getHorResolution() {
	return H_RES;
}

int getVerResolution() {
	return V_RES;
}

char *getGraphicsBuffer() {
	return doubleBuffer;
}

void initDoubleBuffer()
{
	doubleBuffer = malloc (vram_size);
}

void bufferToVideoMem() {
	memcpy(video_mem, doubleBuffer, vram_size);
}

void clearBuffer() {
	memset(doubleBuffer, 0, vram_size);
}

void *vg_init(unsigned short mode) {
	int r;
	struct mem_range mr;
	struct reg86u reg86;
	vbe_mode_info_t vmi_p;

	vbe_get_mode_info(mode, &vmi_p);


	h_res = vmi_p.XResolution;
	v_res = vmi_p.YResolution;
	bits_per_pixel = vmi_p.BitsPerPixel;
	vram_size = (h_res * v_res * bits_per_pixel ) / 8;


	reg86.u.b.intno = 0x10; //interrupt
	reg86.u.b.ah = 0x4F; // quando e invocado uma funcao vbe -> para distinguir das funcoes padrao da BIOS
	reg86.u.b.al = 0x02; // set vbe mode
	reg86.u.w.bx = (1 << 14) | mode;  // bit 14: linear framebuffer



	if (sys_int86(&reg86) != OK) {
		printf("\tvg_init()::sys_int86() failed \n");
		return NULL;
	}



	/* Allow memory mapping */

	mr.mr_base = (phys_bytes) vmi_p.PhysBasePtr;
	mr.mr_limit = mr.mr_base + vram_size;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

	printf("VRAM SIZE:%d, ADDR: %p\n", vram_size, vmi_p.PhysBasePtr);


	if(video_mem == MAP_FAILED){
		printf("couldn't map video memory");
		return 0;
	}

	return video_mem;

}

int drawSquare(unsigned short x, unsigned short y, unsigned short size, unsigned short color)
{
	unsigned x2 = x;
	unsigned y2 = y;
	char * ptr = video_mem + h_res * y + x2;

	size_t i = 0;

	while(i < size + y2)
	{
		size_t j = 0;
		while(j < size + x2)
		{
			* ptr = color;
			++ ptr;
			++ j;
		}
		ptr = video_mem +  h_res  * y  + x2;
		++ y;
		++ i;
	}
	return 0;
}

void paintPixel(int x, int y, char color, char* buf) {
	if (x < h_res && y < v_res) {
		*(buf + y * h_res + x) = color;
	}
}

void drawLine(unsigned xi, unsigned yi, unsigned xf, unsigned yf, char color) {
	float dx = xf - xi;
	float dy = yf - yi;
	float m; //declive //y = mx + b ; m = dy /dx

	int signX ;

	// Check signals to inclination of line

	if(dx > 0)
		signX = 1;
	else
		signX = -1;

	int signY ;
	if(dy > 0)
		signY = 1;
	else
		signY = -1;

	int i;
	if ( dx >= dy) {
		m = dy / dx;

		for (i = 0; i != dx; i += signX) {
			paintPixel(xi + i, yi + i * m, color, video_mem);
		}
	}
	else {
		m = dx / dy;
		for (i = 0; i != dy; i += signY) {
			paintPixel(xi + i * m, yi + i, color, video_mem);
		}
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
