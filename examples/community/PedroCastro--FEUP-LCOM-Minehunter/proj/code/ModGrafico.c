#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "ModGrafico.h"
#include "bitmap.h"
#include "pixmap.h"
#include "vbe.h"
#include "ModRTC.h"
#include "Menu.h"








void update_screen(int cor,int turn,int relog){
	drawRato(cor);
	if(relog == 1)
	{
		int hor1 = (hour / 10) % 10;
		int hor2 = hour%10;
		int minut1 = (min / 10) % 10;
		int minut2 = min%10;
		////
		printf("%d\n",minut2);
		switch(minut2)
		{
		case 0:
			drawBitmap(Zero,994,0,ALIGN_LEFT,bufferRato);
			break;
		case 1:
			drawBitmap(One,994,0,ALIGN_LEFT,bufferRato);
			break;
		case 2:
			drawBitmap(Two,994,0,ALIGN_LEFT,bufferRato);
			break;
		case 3:
			drawBitmap(Three,994,0,ALIGN_LEFT,bufferRato);
			break;
		case 4:
			drawBitmap(Four,994,0,ALIGN_LEFT,bufferRato);
			break;
		case 5:
			drawBitmap(Five,994,0,ALIGN_LEFT,bufferRato);
			break;
		case 6:
			drawBitmap(Six,994,0,ALIGN_LEFT,bufferRato);
			break;
		case 7:
			drawBitmap(Seven,994,0,ALIGN_LEFT,bufferRato);
			break;
		case 8:
			drawBitmap(Eight,994,0,ALIGN_LEFT,bufferRato);
			break;
		case 9:
			drawBitmap(Nine,994,0,ALIGN_LEFT,bufferRato);
			break;
		}
		switch(minut1)
		{
		case 0:
			drawBitmap(Zero,964,0,ALIGN_LEFT,bufferRato);
			break;
		case 1:
			drawBitmap(One,964,0,ALIGN_LEFT,bufferRato);
			break;
		case 2:
			drawBitmap(Two,964,0,ALIGN_LEFT,bufferRato);
			break;
		case 3:
			drawBitmap(Three,964,0,ALIGN_LEFT,bufferRato);
			break;
		case 4:
			drawBitmap(Four,964,0,ALIGN_LEFT,bufferRato);
			break;
		case 5:
			drawBitmap(Five,964,0,ALIGN_LEFT,bufferRato);
			break;
		case 6:
			drawBitmap(Six,964,0,ALIGN_LEFT,bufferRato);
			break;
		case 7:
			drawBitmap(Seven,964,0,ALIGN_LEFT,bufferRato);
			break;
		case 8:
			drawBitmap(Eight,964,0,ALIGN_LEFT,bufferRato);
			break;
		case 9:
			drawBitmap(Nine,964,0,ALIGN_LEFT,bufferRato);
			break;
		}
		drawBitmap(Points,944,0,ALIGN_LEFT,bufferRato);
		switch(hor2)
		{
		case 0:
			drawBitmap(Zero,914,0,ALIGN_LEFT,bufferRato);
			break;
		case 1:
			drawBitmap(One,914,0,ALIGN_LEFT,bufferRato);
			break;
		case 2:
			drawBitmap(Two,914,0,ALIGN_LEFT,bufferRato);
			break;
		case 3:
			drawBitmap(Three,914,0,ALIGN_LEFT,bufferRato);
			break;
		case 4:
			drawBitmap(Four,914,0,ALIGN_LEFT,bufferRato);
			break;
		case 5:
			drawBitmap(Five,914,0,ALIGN_LEFT,bufferRato);
			break;
		case 6:
			drawBitmap(Six,914,0,ALIGN_LEFT,bufferRato);
			break;
		case 7:
			drawBitmap(Seven,914,0,ALIGN_LEFT,bufferRato);
			break;
		case 8:
			drawBitmap(Eight,914,0,ALIGN_LEFT,bufferRato);
			break;
		case 9:
			drawBitmap(Nine,914,0,ALIGN_LEFT,bufferRato);
			break;
		}
		switch(hor1)
		{
		case 0:
			drawBitmap(Zero,884,0,ALIGN_LEFT,bufferRato);
			break;
		case 1:
			drawBitmap(One,884,0,ALIGN_LEFT,bufferRato);
			break;
		case 2:
			drawBitmap(Two,884,0,ALIGN_LEFT,bufferRato);
			break;
		case 3:
			drawBitmap(Three,884,0,ALIGN_LEFT,bufferRato);
			break;
		case 4:
			drawBitmap(Four,884,0,ALIGN_LEFT,bufferRato);
			break;
		case 5:
			drawBitmap(Five,884,0,ALIGN_LEFT,bufferRato);
			break;
		case 6:
			drawBitmap(Six,884,0,ALIGN_LEFT,bufferRato);
			break;
		case 7:
			drawBitmap(Seven,884,0,ALIGN_LEFT,bufferRato);
			break;
		case 8:
			drawBitmap(Eight,884,0,ALIGN_LEFT,bufferRato);
			break;
		case 9:
			drawBitmap(Nine,884,0,ALIGN_LEFT,bufferRato);
			break;
		}
	}
	if(turn != 0)
	{
		Bitmap* seta;
		if(turn == 1)
		{
			seta = loadBitmap(PATH"Arrow.bmp");
			drawBitmap(seta,144,50,ALIGN_LEFT,bufferRato);
		}
		else{
			seta = loadBitmap(PATH"Arrow_inv.bmp");
			drawBitmap(seta,734,50,ALIGN_LEFT,bufferRato);
		}
		deleteBitmap(seta);
	}
	trocarVideo_Mem_Rato();
}

int draw_string (char* str, double scale, unsigned long x, unsigned long y, unsigned long color){

}

char** retXPM(char *xpm){
	if(strncmp(xpm, "pic1", strlen("pic1")) == 00)
	{
		return pic1;
	}
	else if(strncmp(xpm, "pic2", strlen("pic21")) == 00)
	{
		return pic2;
	}
	else if(strncmp(xpm, "pic3", strlen("pic3")) == 00)
	{
		return pic3;
	}
	else if(strncmp(xpm, "cross", strlen("cross")) == 00)
	{
		return cross;
	}
	else if(strncmp(xpm, "penguin", strlen("penguin")) == 00)
	{
		return penguin;
	}
}

char *read_xpm(char *map[], int *wd, int *ht)
{
  int width, height, colors;
  char sym[256];
  int  col;
  int i, j;
  char *pix, *pixtmp, *tmp, *line;
  char symbol;

  /* read width, height, colors */
  if (sscanf(map[0],"%d %d %d", &width, &height, &colors) != 3) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }
#ifdef DEBUG
  printf("%d %d %d\n", width, height, colors);
#endif
  if (width > HRES || height > VRES || colors > 256) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }

  *wd = width;
  *ht = height;

  for (i=0; i<256; i++)
    sym[i] = 0;

  /* read symbols <-> colors */
  for (i=0; i<colors; i++) {
    if (sscanf(map[i+1], "%c %d", &symbol, &col) != 2) {
      printf("read_xpm: incorrect symbol, color at line %d\n", i+1);
      return NULL;
    }
#ifdef DEBUG
    printf("%c %d\n", symbol, col);
#endif
    if (col > 256) {
      printf("read_xpm: incorrect color at line %d\n", i+1);
      return NULL;
    }
    sym[col] = symbol;
  }

  /* allocate pixmap memory */
  pix = pixtmp = malloc(width*height);

  /* parse each pixmap symbol line */
  for (i=0; i<height; i++) {
    line = map[colors+1+i];
#ifdef DEBUG
    printf("\nparsing %s\n", line);
#endif
    for (j=0; j<width; j++) {
      tmp = memchr(sym, line[j], 256);  /* find color of each symbol */
      if (tmp == NULL) {
    	  printf("read_xpm: incorrect symbol at line %d, col %d\n", colors+i+1, j);
    	  return NULL;
      }
      *pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-) */
#ifdef DEBUG
      printf("%c:%d ", line[j], tmp-sym);
#endif
    }
  }

  return pix;
}

void vg_set_pixel(unsigned int x,unsigned int y, unsigned long color){
	uint16_t *ptr = video_mem;
	ptr += x;
	ptr += HRES*y;
	*ptr = color;
}

void vg_set_pixel_bufferRato(unsigned int x,unsigned int y, unsigned long color){
	uint16_t *ptr = bufferRato;
	ptr += x;
	ptr += HRES*y;
	*ptr = color;
}

void vg_set_pixel_buffer(unsigned int x,unsigned int y, unsigned long color){
	uint16_t *ptr = buffer;
	ptr += x;
	ptr += HRES*y;
	*ptr = color;
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

void *vg_init(unsigned short mode){

	struct reg86u r;
	r.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = 1<<14|mode; // set bit 14: linear framebuffer
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}
	int s;
	struct mem_range mr;


	/* Allow memory mapping */
	mr.mr_base = (phys_bytes)(VRAM_PHYS_ADDR);
	mr.mr_limit = mr.mr_base + (HRES*VRES*BITS_PER_PIXEL/8);

	if( OK != (s = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", s);

	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, HRES*VRES*BITS_PER_PIXEL/8);

	if(video_mem == MAP_FAILED)
		panic("video_txt: couldn't map video memory");
	return video_mem;
}

void trocarRato_buffer(){
	memcpy(bufferRato,buffer,videoMemSize* BITS_PER_PIXEL	/8);
}

void trocarVideo_Mem_Rato(){
	memcpy(video_mem,bufferRato,videoMemSize* BITS_PER_PIXEL	/8);
}

void darker_screen(){
	printf("darker_screen\n");
	int i;
	int total;
	for(i = 0; i < HRES*VRES;i++)
	{
		unsigned char red,blue,green;
		red = 0;
		green = 0;
		blue = 0;
		red -= red*(1/4);
		blue -= blue*(1/4);
		green -= green*(1/4);
		total = (red << 11)|(green<<5)|blue;
		memcpy(&total,buffer+i*2,2);
	}
}
