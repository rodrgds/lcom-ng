#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "video_gr.h"

/* Private global variables */

static char *video_mem; /* Process (virtual) address to which VRAM is mapped */
static char *double_buffer; /* Double buffering */

static unsigned int vram_size;

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void *vg_init(unsigned short mode) {

	struct reg86u reg86;

	reg86.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode

	reg86.u.w.bx = 1 << 14 | mode; // set bit 14: linear framebuffer
	reg86.u.b.intno = 0x10;

	if (sys_int86(&reg86) != OK) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return (void *) 1;
	}

	if(reg86.u.b.ah != 0) {
		return NULL;
	}

	vbe_mode_info_t mode_info;
	vbe_get_mode_info(mode, &mode_info);

	h_res = mode_info.XResolution;
	v_res = mode_info.YResolution;
	bits_per_pixel = mode_info.BitsPerPixel;

	int r;
	struct mem_range mr;
	unsigned int vram_base = mode_info.PhysBasePtr;
	vram_size = h_res * v_res * bits_per_pixel / 8;

	/* Allow memory mapping */

	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;

	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

	//Allocate memory to other buffer
	double_buffer = malloc (vram_size);

	if (video_mem == MAP_FAILED)
		panic("couldn't map video memory");
	return (void *) video_mem;

}

void vg_circle(unsigned short x0, unsigned short y0, unsigned short r0, unsigned long color) {

    int x = r0 - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (r0 << 1);

    while (x >= y)
    {
        vg_paintPixel(x0 + x, y0 + y, color);
        vg_paintPixel(x0 + y, y0 + x, color);
        vg_paintPixel(x0 - y, y0 + x, color);
        vg_paintPixel(x0 - x, y0 + y, color);
        vg_paintPixel(x0 - x, y0 - y, color);
        vg_paintPixel(x0 - y, y0 - x, color);
        vg_paintPixel(x0 + y, y0 - x, color);
        vg_paintPixel(x0 + x, y0 - y, color);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (r0 << 1);
        }
    }
}

void vg_paintPixel(unsigned short x, unsigned short y, unsigned long color) {

	//Check if it should print
	if(x >= h_res || y >= v_res)
		return;

	char * pixel = double_buffer + y*h_res + x;

	*pixel = color;
}

void vg_sprite(char *xpm[], unsigned short xi, unsigned short yi, int option) {

	Sprite * sprite = create_sprite(xpm, xi, yi);

	if(option)
		vg_drawSprite(sprite);
	else
		vg_eraseSprite(sprite);

}

void vg_nextFrame() {

	//Copy from the second buffer to first
	memcpy(video_mem, double_buffer, vram_size);

	//Clear the second buffer
	vg_clear();

}

void vg_clear() {

	char * init = double_buffer;

	int i,j;

	//Paint size columns
	for(i = 0; i < h_res; i++) {

		//Paint one line
		for(j = 0; j < v_res; j++) {
			*init = 0;
			init++;
		}
	}
}

void vg_drawRectangle(int life) {

	if(life < 0)
		return;

	if(life > 100)
		life = 100;

	//Make the outer border of the rectangle
	int i;
	for(i = 0;i < 40;i++)
	{
		vg_paintPixel(1023-i,584,63);
		vg_paintPixel(1023-i,184,63);
	}

	for(i = 0;i < 400; i++)
	{
		vg_paintPixel(1023,184+i,63);
		vg_paintPixel(983,184+i,63);
	}

	//Fill in the rectangle
	int j;
	for(i = 1; i < life * 4 - 1;i++)
	{
		for(j = 1; j < 40; j++)
		{
			vg_paintPixel(1023-j,584-i,4);
		}
	}
}
