#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

int bytesPerPyxel;
static void *video_mem;
static void *back_buffer;
int xRes;
int yRes;
vbe_mode_info_t info;




int (set_graphics_mode)(uint16_t mode){
  vbe_get_mode_info(mode,&info);
  int size;
  bytesPerPyxel=(info.BitsPerPixel+7)/8;
  xRes=info.XResolution;
  yRes=info.YResolution;
  size=(bytesPerPyxel*info.XResolution*info.YResolution);
  back_buffer=malloc(size);
  struct minix_mem_range mr;
  mr.mr_base=info.PhysBasePtr;
  mr.mr_limit=mr.mr_base+size;
  if( OK != (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
    return 1;
  }
  video_mem=vm_map_phys(SELF,(void*)mr.mr_base,size);

  reg86_t r86;
  memset(&r86, 0, sizeof(r86));	
  r86.intno = 0x10; 
  r86.ax= 0x4F02;
  r86.bx = mode|BIT(14);

  if( sys_int86(&r86) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}


int (changeColorPixel)(uint16_t x, uint16_t y, uint32_t color) {

  uint8_t * ptr;
  ptr = (uint8_t*)back_buffer + (x * bytesPerPyxel) + (y * bytesPerPyxel * xRes);
  memcpy(ptr,&color,bytesPerPyxel);
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0 ; i < len ; i++) {
    if(changeColorPixel(x + i,y,color) != 0) {
      return 1;
    }
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for (int i = 0 ; i < height ; i++) {
    if(vg_draw_hline(x, y + i, width, color) != 0) {
      return 1;
    }
  }
  return 0;
}

void (vg_copy)(){
  int space = xRes * yRes * bytesPerPyxel;
  memcpy(video_mem,back_buffer,space);
}






