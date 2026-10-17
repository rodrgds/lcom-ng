#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>


#include <assert.h>

#include "vt_info.h"

/* Color definitions */

#define	BLACK	0x00
#define GREY 0x08
#define YELLOW 0x06
#define PURPLE 0x05
#define RED	(0x01 << 2)
#define BLUE_GREEN 0x03
#define GREEN	(0x01 << 1)
#define BLUE	0x01 
#define WHITE	(RED | GREEN | BLUE)

/* Shift size for background and foreground colors */

#define BACK	12	
#define FORE	8


/* Other character attributes */

#define BRIGHT	0x08	
#define BLINK	0x80

/*Character codes to draw single border frames */

#define VERT_BAR 	0xB3 
#define HOR_BAR 	0xC4
#define LL_CORNER 	0xC0
#define UL_CORNER	0xDA
#define LR_CORNER	0xD9
#define UR_CORNER	0xBF

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

#define size (scr_width*scr_lines)

void vt_fill(char ch, char attr)
{
	int i;
	for (i=0; i<size*2; i++)
	{
		video_mem[i] = ch;
		i++;
		video_mem[i] = attr;
	}
}

void vt_blank(void)
{
	int i;
	for (i=0; i<size*2; i++)
	{
		video_mem[i] = 00;
		i++;
		video_mem[i] = 00;
	}
}

int vt_print_char(char ch, int r, int c, char attr)
{
	unsigned int pos= (scr_width*r+c) *2;
	video_mem[pos]= ch;
	pos++;
	video_mem[pos]= attr;
}

int vt_print_string(char *str, int r, int c, char attr)
{
	unsigned int pos = scr_width*r + c;
	int i;
	int s = strlen(str);
	for (i=0 ; i<s ; i++)
	{
		vt_print_char(str[i], r, c+i, attr);
	}

}

int vt_print_int(int num, int r, int c, char attr)
{	int n = 16, j, a=0, num_size = 0;
	char buf[17];
	char *numero;
	buf[n]=0;
	do
	{ n--;
	j = num % 10;
	buf[n] = j + 0x30;
	num = num/10;
	num_size++;
	}while (num);

	numero = (char*)malloc((num_size + 1) * sizeof(char));

	for (n = num_size - 1; n >= 0; n--)
	{
		numero[n] = buf[15 - a];
		a++;
	}
	numero[num_size] = 0;
	return vt_print_string(numero,r,c,attr);
}


int vt_draw_frame(int width, int height, int r, int c, char attr)
{
	int i, j, n;
	for (i=0; i<width; i++)
	{
		if(i==0)
		{
			vt_print_char(UL_CORNER, r, c, attr);
			for(j=1; j<(height-1); j++)
			{
				vt_print_char(VERT_BAR, r+j, c, attr);
			}
			vt_print_char(LL_CORNER, r+(height-1), c, attr);
		}
		else if (i==(width - 1))
		{
			vt_print_char(UR_CORNER, r, c+i, attr);
			for(n=1; n<(height-1); n++)
			{
				vt_print_char(VERT_BAR, r+n, c+i, attr);
			}
			vt_print_char(LR_CORNER, r+(height-1), c+i, attr);
		}
		else
		{
			vt_print_char(HOR_BAR, r, c+i, attr);
			vt_print_char(HOR_BAR, r+(height-1), c+i, attr);
		}
	}
}


char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
