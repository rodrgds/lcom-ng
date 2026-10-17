#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "video_gr.h"

int xCursor;
int yCursor;

static uint16_t hRes; //number of pixels per line 
static uint16_t vRes; //number of pixels per column 
static uint8_t bits_per_pixel;  //number of bits in the VRAM, per pixel
static char *video_mem; /*frame-buffer VM address*/
static char *video_mem2;
unsigned int vram_base;  /*VRAM’s physical address for flat memory fram buffer*/
unsigned int vram_size;  /*VRAM’s size*/

static uint8_t bytes_per_pixel;

vbe_mode_info_t vmi;

void *(vg_init)(uint16_t mode){

  //Get vbe mode information
  if(vbe_get_mode_info(mode, &vmi)==1){
    printf("Failed getting mode!\n");
    return NULL;
  }

  //Initializes static global variables with the resolution of the screen and the color depth
  hRes = vmi.XResolution;
  vRes = vmi.YResolution;
  bits_per_pixel = vmi.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel+7) / 8;


  vram_base = vmi.PhysBasePtr;
  vram_size = hRes*vRes*bytes_per_pixel;

  video_mem2 = malloc(vram_size);

  //Accessing memory
  int r;
  struct minix_mem_range mr; /*physical memory range*/

  /*Allow memory mapping*/
  mr.mr_base = (phys_bytes)vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return NULL;
  }
    
  /*Map memory*/
  video_mem = vm_map_phys(SELF, (void*)
  mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED){
    panic("couldn’t map video memory");
    return NULL;
  }

  /*Changes mode*/
  reg86_t reg;

  memset(&reg,0,sizeof(reg));

  reg.ah = 0x4F; 
  reg.al = 0x02; 
  reg.intno = 0x10;
  reg.bx = (mode | BIT(14));

  if( sys_int86(&reg) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return NULL;
  }
  if(reg.ah != 0)
    return NULL;

  return video_mem;
}

int set_pixel(uint16_t x, uint16_t y, uint32_t color){
  if(x>=hRes || y>=vRes)
    return 1;

  for(int i=0;i<bytes_per_pixel;i++){
    //*(video_mem + (y*hRes) + x) --> desenha no pixel de coordenadas x,y
    *(video_mem2 + (y * hRes + x)*bytes_per_pixel + i) = color;
    color>>=8; //discards lsb
  }
  return 0;
}

void doubleBuffering(){
  memcpy(video_mem, video_mem2,vram_size);
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  for(int i=0;i<len;i++){
    if(set_pixel(x+i,y,color)==1)
      return 1;
  }  
  return 0;
} 		

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for(int i=0; i<height;i++){
    if(vg_draw_hline(x, y+i, width, color)==1)
      return 1;
    }
  return 0;
}

int vg_draw_img(xpm_image_t img, uint16_t x, uint16_t y){

  for(int i=0;i<img.height;i++){
    for(int j=0;j<img.width;j++){
      uint32_t color = 0;
      for(int k=0;k<bytes_per_pixel;k++){
        color |= img.bytes[(img.width*i+j)*bytes_per_pixel+k] << 8*k;
      }
      if(color!=xpm_transparency_color(XPM_8_8_8))
        set_pixel(x+j,y+i,color);
    }
  }
  return 0;
}

void update_mouse_position(int deltaX, int deltaY){
  if(xCursor+deltaX<0)
    xCursor=0;
  
  else xCursor+=deltaX;

  if(xCursor+deltaX>=798)
    xCursor=790;
  
  else xCursor += deltaX;

  if(yCursor-deltaY<0)
    yCursor=0;
  else yCursor -=deltaY;

  if(yCursor-deltaY>=598)
    yCursor = 593;
  else yCursor -= deltaY;
}
