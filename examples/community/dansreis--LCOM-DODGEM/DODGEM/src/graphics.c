#include "DODGEM.h"
#include "graphics.h"
#include "video_gr.h"
#include "utilities.h"
#include "minix/syslib.h"
#include "bitmap.h"
#include <stdbool.h>


static uint16_t *video_mem;
static uint16_t *DOUB_BUF;
static uint16_t *TRIPLE_BUF;

short * getTripleBuffer()
{
	return TRIPLE_BUF;
}

short * getVideoBuffer()
{
	return DOUB_BUF;
}

short * getVideoMem()
{
	return video_mem;
}

void graphicsStart(unsigned short mode)
{
	video_mem = (short*)vg_init(mode);
	DOUB_BUF = ( uint16_t *)malloc(MODE1024_H_RES * MODE1024_V_RES * 2);
	TRIPLE_BUF =  (uint16_t *)malloc(MODE1024_H_RES * MODE1024_V_RES * 2);
}

void graphicsExit()
{
	vg_exit();
}


void set_pixel(unsigned short x, unsigned short y, unsigned long color) {
	*(video_mem + x + y * MODE1024_H_RES) = color;
}

void graphics_fill(unsigned long color) {

	int x, y;
	for (y = 0; y < MODE1024_V_RES; y++)
		for(x=0; x < MODE1024_H_RES;x++)
			set_pixel(x,y,color);
}





