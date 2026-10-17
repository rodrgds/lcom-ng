#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "video_gr.h"

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static char *video_mem_sec;
static mmap_t mem_map;
unsigned h_res;	        /* Horizontal resolution in pixels */
unsigned v_res;	        /* Vertical resolution in pixels */
unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
extern uint32_t COUNTER;


int (vbe_get_info)(uint16_t mode, vbe_mode_info_t * mode_info) {
  lm_alloc(sizeof(vbe_mode_info_t), &mem_map);
  reg86_t reg;
  memset(&reg,0,sizeof(reg));
  reg.ax = 0x4F01; /* vbe get mode info */
  /* translate the buffer linear adress to a far pointer */
  reg.es = PB2BASE(mem_map.phys); /* set a segment base */
  reg.di = PB2OFF(mem_map.phys); /* set the offset accordingly */
  reg.cx = mode;
  reg.intno = VIDEO_CARD;

  if (sys_int86(&reg) != OK) {return 1;} 

  memcpy(mode_info,mem_map.virt, mem_map.size);
  lm_free(&mem_map);
  return 0;
}

void *(vg_init)(uint16_t mode){
  vbe_mode_info_t vmi;
  vbe_get_info(mode, &vmi);
  
  h_res = vmi.XResolution;
  v_res = vmi.YResolution;
  bits_per_pixel = vmi.BitsPerPixel;

  struct minix_mem_range mr, mr_secondary;

  unsigned int vram_base = vmi.PhysBasePtr;
  unsigned int vram_size = h_res * v_res * ((bits_per_pixel+7)/8);
  
  unsigned int vram_base_secondary_buffer = vmi.PhysBasePtr + vram_size;
  int r;
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size *2;
  mr_secondary.mr_base = (phys_bytes) vram_base_secondary_buffer;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);


  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  video_mem_sec = vm_map_phys(SELF, (void *) mr_secondary.mr_base, vram_size);
  if(video_mem == MAP_FAILED ||video_mem_sec == MAP_FAILED )
    panic("couldn't map video memory");
  memset(video_mem, 0, vram_size);
  memset(video_mem_sec, 0, vram_size);
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));
  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02;
  r86.bx = 1<<14|mode;
  if( sys_int86(&r86) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return NULL;
  }
  return 0;
}

int (vg_draw_pixel) (uint16_t x, uint16_t y, uint32_t color){
    uint32_t temp;
    char *pix_ad =  video_mem_sec + x * ((bits_per_pixel+7)/8) + (y * h_res *((bits_per_pixel+7)/8));
      temp = color;
      memcpy(pix_ad,&temp,(bits_per_pixel+7)/8);
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  for(int i = x;i<x+len;i++){
        vg_draw_pixel(i,y,color);
  }
  return 0;
}
  
 int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
     for(int i = y;i<y+height;i++){
       vg_draw_hline(x,i,width,color);
     }
     return 0;
 }

void draw_xpm(uint8_t *map, xpm_image_t * img, uint16_t x, uint16_t y){
   for(unsigned int i=0; i<img->height; i++){
    for(unsigned int j= 0; j<img->width; j++){
      uint32_t color = 0;
      for (size_t byte = 0; byte < (bits_per_pixel+7)/8; byte++) {
        color |= *(map + (j + i * img->width) * ((bits_per_pixel+7)/8) + byte) << (byte * 8);
      }
      if(color!=xpm_transparency_color(img->type))
       vg_draw_pixel(x + j, y + i, color);
    }
  }
}

int get_xpm(xpm_map_t xmap, uint16_t x, uint16_t y){
  uint8_t *map;
  xpm_image_t img;
  map = xpm_load(xmap,XPM_8_8_8,&img);
  if(map == NULL) return 1;
  draw_xpm(map,&img,x,y);
  return 0;
}



int delete_xpm(xpm_image_t img, uint16_t x, uint16_t y){
  for(unsigned int i=0; i<img.height; i++){
    for(unsigned int j= 0; j<img.width; j++){
       vg_draw_pixel(x + j, y + i, 0);
    }
  }
  return 0;
}


void swap_buffer() {
 memcpy(video_mem,video_mem_sec,h_res * v_res * ((bits_per_pixel+7)/8));
 memset(video_mem_sec,0,h_res * v_res * ((bits_per_pixel+7)/8));
}

void (double_buffer)() {
  memcpy(video_mem, video_mem_sec, v_res * h_res * ((bits_per_pixel+7)/8));
}
