#include <lcom/lcf.h>
#include "lcom/video_gr.h"

#include <minix/driver.h>

#include <minix/sysutil.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include <sys/mman.h>
#include "video_gr.h"
#include <math.h>

#include "timer/i8254.h"


uint8_t* vram;
int h_res;
int v_res;
int bpp;
int bytes;
int colorModel;
uint8_t* imBuffer;
uint8_t* drawBuffer;
vbe_mode_info_t vbe;


int (set_graphic_mode)(uint16_t submode){
    reg86_t reg;
    memset(&reg, 0, sizeof(reg)); 
    reg.intno = 0x10;                                      // parte menos significativa de AX. 0x02 no caso de modo gráfico
    reg.ax = 0x4F02;             
    reg.bx = submode | BIT(14);     
    if (sys_int86(&reg)) return 1;     
    return 0;
}

void* (vg_init)(uint16_t mode){
    
    struct minix_mem_range mr;

    memset(&vbe, 0, sizeof(vbe));
    if (vbe_get_mode_info(mode, &vbe)) return NULL;

    h_res = vbe.XResolution;
    v_res = vbe.YResolution;
    bpp = vbe.BitsPerPixel;
    colorModel = vbe.MemoryModel;
    bytes = (bpp+7)/8;
    unsigned int vram_size = h_res * v_res * bytes;

    mr.mr_base = vbe.PhysBasePtr;
    mr.mr_limit = vbe.PhysBasePtr + vram_size;

    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK) return NULL;

    if((vram = vm_map_phys(SELF, (void*)mr.mr_base, vram_size)) == MAP_FAILED) return NULL;

    return 0;
}

int vg_drawpixel(uint16_t x, uint16_t y, uint32_t color){
  if (x<0 || y<0 || x>=vbe.XResolution || y>=vbe.YResolution){
    return 1;
  }
  int offset = ((y*vbe.XResolution)+x)*bytes;
  if ((memcpy(&imBuffer[offset],&color,bytes))==NULL) return 1;


  return 0;  
}

int vg_drawline(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  int compr;
  if (x+len > vbe.XResolution){
    compr = vbe.XResolution - x;
  } 
  else {
    compr = len;
  }
  for (int i=0;i<compr;i++){
    if (vg_drawpixel(x+i,y,color)) return 1;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for (int i = 0;i<height;i++){
      if (vg_drawline(x,y+i,width,color)) return 1;
    }  
    return 0;
}

void allocate_img_buffer(){
  imBuffer = (uint8_t*)calloc(vbe.XResolution*vbe.YResolution,bytes);
}

void display_image(){
  memcpy(vram,imBuffer,vbe.XResolution*vbe.YResolution*bytes);
}

void clear_im_buffer(){
  memset(imBuffer,0,vbe.XResolution*vbe.YResolution*bytes);
}

void free_im_buffer(){
  free(imBuffer);
  imBuffer = NULL;
}




