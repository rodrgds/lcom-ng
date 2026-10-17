#include <lcom/lcf.h>
#include <machine/int86.h> // /usr/src/include/arch/i386
#include <stdint.h>
#include <stdio.h>
#include <minix/sysutil.h>
#include <stdbool.h>
#include "vbe_macros.h"
#include "graphics.h"
#include "game.h"



uint8_t GlobalRedScreeMask, GlobalGreenScreeMask, GlobalBlueScreeMask;
int h_res, v_res;

int space = 0;
int arrived = 1;
extern bool explosion;
static char * doubleBuff;

void* (vg_init)(uint16_t mode){

//mode_global= mode;
	struct reg86u reg86;

    lm_init(true);

  	vbe_mode_info_t *vmi_p = malloc(sizeof(vbe_mode_info_t));

  if(vbe_info(mode, vmi_p) != 0){
    printf("Erro na função vbe_get_mode_info\n");
    return NULL;
  }

  if(map_vram(vmi_p) != 0){
    printf("Erro na função map_vram\n");
    return NULL;
  }

  memset(&reg86, 0, sizeof(reg86)); /* zero the structure */

  reg86.u.w.ax = VBE_MODE;
  reg86.u.w.bx = LINEAR | mode;
  reg86.u.b.intno = INIT;

  if(sys_int86(&reg86) != OK) {
      printf("vg_init(): sys_int86() failed \n");
      return NULL;
    }

  return video_mem;
}

int vbe_info(uint16_t mode,  vbe_mode_info_t *vmi_p){
  struct reg86u r;
  mmap_t m;
  lm_alloc(sizeof(vbe_mode_info_t), &m);  
  phys_bytes buf = m.phys;

  memset(&r, 0, sizeof(r)); /* zero the structure */

  r.u.w.ax = MODE_INFO;          /* VBE get mode info */
  /* translate the buffer linear address to a far pointer */
  r.u.w.es = PB2BASE(buf);    /* set a segment base */
  r.u.w.di = PB2OFF(buf);     /* set the offset accordingly */
  r.u.w.cx = mode;
  r.u.b.intno = 0x10;


  if( sys_int86(&r) != OK ){
    printf("vg_get_info(): sys_int86() failed \n");
    return 1;
  }

  if (r.u.b.ah != 0)
  {
    lm_free(&m);
    return 1;
  }

  *vmi_p = *(vbe_mode_info_t *) m.virt;

  lm_free(&m);

  return 0;
}


int map_vram(vbe_mode_info_t *vmi_p){

	h_res = vmi_p->XResolution;
	v_res = vmi_p->YResolution;
  bits_per_pixel = vmi_p->BitsPerPixel;
  num_bytes_mode = bits_per_pixel/8;
  doubleBuff=(char*) malloc(h_res*v_res*bits_per_pixel/8);              /* use liblm.a to initialize buf */

  if (bits_per_pixel == 15)
  {
    num_bytes_mode = 2;
  }
  GlobalRedScreeMask = vmi_p->RedMaskSize;
  GlobalGreenScreeMask = vmi_p->GreenMaskSize;
  GlobalBlueScreeMask = vmi_p->BlueMaskSize;


	int r;
	struct minix_mem_range mr;
	unsigned int vram_base = vmi_p->PhysBasePtr;  /* VRAM's physical addresss */
	unsigned int vram_size = h_res*v_res*num_bytes_mode;  /* VRAM's size, but you can use
				    the frame-buffer size, instead */

	/* Allow memory mapping */

	mr.mr_base = (phys_bytes) vram_base;	
	mr.mr_limit = mr.mr_base + vram_size;  

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
   		panic("sys_privctl (ADD_MEM) failed: %d\n", r);
   		return 1;
   	}

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

	if(video_mem == MAP_FAILED){
  	 	panic("couldn't map video memory");
  	 	return 1;
  	 }

  	return 0;
}

int vg_draw_xpm(unsigned char* pic,  xpm_image_t* xpm, uint16_t x, uint16_t y)
{
  int p = 0;
  uint16_t old_x = x;
  for (int i = 0; i < xpm->height; ++i,y++)
  {
   x=old_x;
   for (int j = 0; j < xpm->width; ++j,x++)
  {
    //char *temp = video_mem;
    int temp = (h_res*y+x)*num_bytes_mode;
    if(pic[p] != (unsigned char) 0x0588 && pic[p+1] != (unsigned char) 0x0588)
      { 
        
          *(doubleBuff +temp) = pic[p];
          *(doubleBuff + temp+1) = pic[p+1];
        
      }
    p += 2;
  }
}

return 0;
}



void double_buffering()
{
  memcpy(video_mem, doubleBuff,h_res*v_res*bits_per_pixel/8);
}


/*
bool check_colision(unsigned char* pic, int x, int y,int width, int height)
{
  int p = 0;
  uint16_t old_x = x;
  for (int i = 0; i < height; ++i,y++)
  {
   x=old_x;
   for (int j = 0; j < width; ++j,x++)
  {
    //char *temp = video_mem;
    int temp = (h_res*y+x)*num_bytes_mode;
    if(pic[p] != *(doubleBuff +temp) && pic[p+1] != *(doubleBuff + temp+1))
      { 
        return true;
      }
    p += 2;
  }
}

return false;

}*/



bool check_colision(unsigned char* pic,int x, int y,int width, int height)
{
  int p = 0;
  uint16_t old_x = x;
  for (int i = 0; i < height; ++i,y++)
  {
   x=old_x;
   for (int j = 0; j < width; ++j,x++)
  {
    //char *temp = video_mem;
    int temp = (h_res*y+x)*num_bytes_mode;
     if(   ((uint8_t) *(doubleBuff +temp) == 0x00 && (uint8_t) *(doubleBuff +temp + 1) == 0xF8) && 
          pic[p] != (unsigned char) 0x0000 && pic[p+1] != (unsigned char) 0x0000)
      { 
        return true;
      }
    p += 2;
  }
}

return false;

}


