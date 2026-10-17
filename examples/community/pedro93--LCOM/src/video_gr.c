#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"

/* Constants for VBE 0x105 mode */

#define VRAM_PHYS_ADDR    	0xD0000000
#define H_RES           	1024
#define V_RES		 	 	768
#define BITS_PER_PIXEL	  	8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void * vg_init(unsigned short mode)
{
	struct reg86u reg86;
	struct mem_range mrange;
	vbe_mode_info_t vmi;
	int r;

	//Initialize graphic mode
	reg86.u.b.intno=0x10;
	reg86.u.w.ax=0x4F02;
	reg86.u.w.bx=1<<14|mode;


	if(sys_int86(&reg86) != OK)
	{
		printf("set_vbe_mode: sys_int86() failed\n");
		return NULL;
	}

	else
	{
		//Memory Mapping
		vbe_get_mode_info(mode, &vmi);

		//Defining the static global variables using the mapped information which vbe_get_mode_info put on the struct vmi
		h_res = vmi.XResolution;
		v_res = vmi.YResolution;
		bits_per_pixel = vmi.BitsPerPixel;

		mrange.mr_base = vmi.PhysBasePtr;
		mrange.mr_limit = mrange.mr_base +(h_res*v_res*(bits_per_pixel)/8);

		if((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mrange)) != OK)
		{
			panic("vg_init: sys_privctl (ADD_MEM) failed: %d\n", r);
			return NULL;
		}
		else
		{
			video_mem = vm_map_phys(SELF, (void *)mrange.mr_base, (h_res * v_res * (bits_per_pixel / 8)));
		}
	}
	return NULL;
}

int vg_fill(unsigned long color) {

	int horizontal,vertical,pixels;
	unsigned long copy_color;
	char *ptr;
	ptr=video_mem;

	for(vertical=0;vertical<V_RES;vertical++)
		for(horizontal=0;horizontal<H_RES;horizontal++)
		{
			copy_color=color;
			for(pixels=0;pixels<BITS_PER_PIXEL/8;pixels++)
			{
				*ptr=(char)copy_color;
				ptr++;
				copy_color>>8;
			}
		}

	return 0;
}

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color) {

	char *ptr;
	int pixels;
	ptr=video_mem;

	if((y>=v_res)||(x>=h_res))
	{
		return -1;
	}

	if(color == 30)
		return 0;

	ptr += ((size_t)y*h_res+x) * (bits_per_pixel/8);
	for(pixels=0;pixels<bits_per_pixel/8;pixels++)
	{
		*ptr=(char)color;
		ptr++;
	}

	return 0;
}

long vg_get_pixel(unsigned long x, unsigned long y) {

	char *ptr;
	ptr=video_mem;

	if((y>v_res)||(x>h_res))
	{
		return -1;
	}

	ptr += y*h_res+x;
	ptr++;
	printf("color : %d",(int)*ptr);
	return (int)*ptr;
}

int vg_draw_line(unsigned long xi, unsigned long yi, unsigned long xf, unsigned long yf, unsigned long color) {

	int horizontal,vertical,pixels;

	if((yi>v_res)||(xi>h_res) ||(yf>v_res)||(xf>h_res))
	{
		return -1;
	}

	//vertical line ->
	if((yi==yf)&&(xf>xi))
	{
		for(horizontal=xi;horizontal!=xf;horizontal++)
		{
			vg_set_pixel(horizontal,yi,color);
		}
	}
	else //straight line <-
	{
		if((yi==yf)&&(xi>xf))
		{
			for(horizontal=xi;horizontal>xf;horizontal--)
			{
				vg_set_pixel(horizontal,yi,color);
			}
		}
		else
		{ //vertical line |
			// ? ? ? ? ? ? ?v
			if((xi==xf)&&(yf>yi))
			{
				for(vertical=yi;vertical<yf;vertical++)
				{
					vg_set_pixel(xi,vertical,color);
				}
			}
			else
			{ //vertical line   ^
				// ? ? ? ? ? ? ?|
				if((xi==xf)&&(yi>yf))
				{
					for(vertical=yi;vertical>yf;vertical--)
					{
						vg_set_pixel(xi,vertical,color);
					}
				}
				else
				{ /*Diagonal \
\.*/
					if((xf>xi)&&(yf>yi))
					{
						for(vertical=yi,horizontal=xi;(vertical<yf)&&(horizontal<xf);vertical++,horizontal++)
						{
							vg_set_pixel(horizontal,vertical,color);
						}
					}
					else
					{
						/*Diagonal .\
\ */
						if((xi>xf)&&(yi>yf))
						{
							for(vertical=yi,horizontal=xi;(vertical>yf)&&(horizontal>xf);vertical--,horizontal--)
							{
								vg_set_pixel(horizontal,vertical,color);
							}
						}
						else
						{ /*Diagonal /.
/ */
							if((xf>xi)&&(yi>yf))
							{
								for(vertical=yi,horizontal=xi;(vertical>yf)&&(horizontal<xf);vertical--,horizontal++)
								{
									vg_set_pixel(horizontal,vertical,color);
								}
							}
							else
							{ /* Diagonal ?/
./ */
								if((xi>xf)&&(yf>yi))
								{
									for(vertical=yi,horizontal=xi;(vertical<yf)&&(horizontal>xf);vertical++,horizontal--)
									{
										vg_set_pixel(horizontal,vertical,color);
									}
								}
							}
						}
					}
				}
			}
		}
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
