#include "video_gr.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "vbe.h"
#include "utils.h"
#include "sprite.h"
#include "Bitmap.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */


unsigned video_mem_size;
char *video_mem;
char *double_buffer;

#define VRAM_PHYS_ADDR	0xE0000000
#define H_RES           1024
#define V_RES		  	768
#define BITS_PER_PIXEL	8

/* Private global variables */

static unsigned h_res = 1024;
static unsigned v_res = 768;
static unsigned bits_per_pixel = 8;

/**
 *
 * Codigo obtido por intermedio de Henrique Ferrolho
 * http://difusal.blogspot.pt/2014/09/minixtutorial-8-loading-bmp-images.html
 */
void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment)
{
	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	if (x + width < 0 || x > getHRES() || y + height < 0
			|| y > getVRES())
		return;

	int xCorrection = 0;
	if (x < 0)
	{
		xCorrection = -x;
		drawWidth -= xCorrection;
		x = 0;

		if (drawWidth > getHRES())
			drawWidth = getHRES();
	}
	else if (x + drawWidth >= getHRES())
	{
		drawWidth = getHRES() - x;
	}

	unsigned long color;

	int i;
	for (i = 0; i < height; i++)
	{
		int j, xbit;

		for (j = 0; j < width; j++)
		{
			color = (bmp->bitmapData[j * 2 + i * width * 2] << 0) | (bmp->bitmapData[j * 2 + i * width * 2 + 1] << 8);
			colorPixel(x + j, height - i - 1 + y, double_buffer, color);
		}
	}
}

void *vg_init(unsigned short mode)
{
	unsigned video_mem_size;
	vbe_mode_info_t data;

	struct reg86u reg86;
	struct mem_range mr;

	reg86.u.b.intno = VBE_INT10;
	reg86.u.w.ax = VBE_AX;
	reg86.u.w.bx = 1 << 14 | mode;

	if (sys_int86(&reg86) != OK)
	{
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	int aux = vbe_get_mode_info(mode, &data);
	if (aux == 1)
	{
		printf("Error on get_mode_info()\n");
		return NULL;
	}

	h_res = data.XResolution;
	v_res = data.YResolution;
	bits_per_pixel = data.BitsPerPixel;

	video_mem_size = h_res * v_res * bits_per_pixel;

	mr.mr_base = data.PhysBasePtr;
	mr.mr_limit = mr.mr_base + video_mem_size;

	if (OK != (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	{
		panic("sys_privctl (ADD_MEM) failed: %d\n", reg86);
	}

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, video_mem_size);

	if (video_mem == MAP_FAILED)
		panic("Couldn't map video memory\n");

	double_buffer = (char*)malloc((h_res+50)*(v_res+39)*bits_per_pixel/8);
	return video_mem;
}

unsigned getHRES()
{
	return h_res;
}

unsigned getVRES()
{
	return v_res;
}

unsigned getBitsPerPixel()
{
	return bits_per_pixel;
}

void colorPixel(int x, int y, char *video_mem, short color)
{
	video_mem += (x + y*h_res)*bits_per_pixel/8;
	*video_mem=(0xFF  & (color));
	video_mem++;
	*video_mem=(0xFF & (color >> 8));
}

void drawSlopingLine(int xi, int yi, int xf, int yf, short color)
{
	int aux_x, aux_y;

	if (xi > xf && yi > yf)
	{
		aux_x = xf;
		aux_y = yf;
		xf = xi;
		xi = aux_x;
		yf = yi;
		yi = aux_y;
	}

	if (xf < xi && yf > yi)
	{
		aux_x = xf;
		aux_y = yf;
		yf = yi;
		yi = aux_y;
		xf = xi;
		xi = aux_x;
	}

	//Line drawing algorithm
	double dx = xf - xi;
	double dy = yf - yi;

	int x, y;
	for (x = xi; x < xf; x++)
	{
		y = yi + dy * (x - xi) / dx;
		colorPixel(x, y, double_buffer, color);
	}
}

void drawSprite(Sprite * s)
{
	int heighttmp, widthtmp;
	for (heighttmp = 0; heighttmp < s->height; heighttmp++)
	{
		for (widthtmp = 0; widthtmp < s->width; widthtmp++)
		{
			colorPixel((unsigned short) (widthtmp + s->x), (unsigned short) (heighttmp + s->y), double_buffer ,s->map[(heighttmp * s->width + widthtmp) * bits_per_pixel / 8]);
		}
	}
}

void drawLine(int xi, int yi, int xf, int yf, char * video_mem, short color)
{
	if (xi != xf && yi != yf)
	{
		drawSlopingLine(xi, yi, xf, yf, color);
		return;
	} else if (xi == xf) //VERTICAL
	{
		int cnt = 0;
		int i = 0;
		while (cnt < (yf - yi))
		{
			colorPixel(xi, yi + i, double_buffer, color);
			cnt++;
			i++;
		}
	} else //HORIZONTAL
	{
		int cnt = 0;
		int i = 0;
		while (cnt < (xf - xi))
		{
			colorPixel(xi + i, yi, double_buffer, color);
			cnt++;
			i++;
		}
	}
	return;
}

int vg_exit()
{
	struct reg86u reg86;

	reg86.u.b.intno = VBE_INT10; /* BIOS video services */
	reg86.u.b.ah = VBE_SET_VIDEO_MODE; /* Set Video Mode function */
	reg86.u.b.al = VBE_SET_TEXT_MODE; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK)
	{
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	}
	return 0;
}

int rgb(unsigned char r, unsigned char g, unsigned char b)
{
	if (r < 0 || 255 < r || g < 0 || 255 < g || b < 0 || b > 255)
		return -1;

	int red = r * 31 / 255;
	int green = g * 63 / 255;
	int blue = b * 31 / 255;

	return (red << 11) | (green << 5) | blue;
}

void clearScreen()
{
	memset(double_buffer,0,h_res * v_res *bits_per_pixel/8);
	int height, width;
	int x = 0;
	int y = 0;

	for (height = 0; height < v_res; height++)
	{
		for (width = 0; width < h_res; width++)
		{
			if (height > 60)
				colorPixel(x + width, y + height, double_buffer, rgb(0,195,5));
		}
	}
}

void topInfoBar()
{
	int height, width;
	int x = 0;
	int y = 0;

	for (height = 0; height < v_res; height++)
	{
		for (width = 0; width < h_res; width++)
		{
			if (height <= 60)
				colorPixel(x + width, y + height, double_buffer, rgb(192,192,192));
		}
	}
}

char* getVideoMem()
{
	return video_mem;
}

void changeVideoMem()
{
	memcpy(video_mem, double_buffer, h_res * v_res *bits_per_pixel/8);
	memset(double_buffer,0,h_res * v_res *bits_per_pixel/8);
}
