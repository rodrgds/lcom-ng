#include "graphics.h"

uint8_t* double_buffer;

unsigned int get_frame_size() {
  unsigned int bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8;
  return vmi.XResolution * vmi.YResolution * bytes_per_pixel;
}

int set_frame(uint16_t mode){
  memset(&vmi, 0, sizeof(vmi));
  if(vbe_get_mode_info(mode, &vmi)) return 1;

  unsigned int frameSize = get_frame_size();

  struct minix_mem_range mr;

  mr.mr_base = vmi.PhysBasePtr; 
  mr.mr_limit = mr.mr_base + frameSize;
  
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) {
    printf("Physical memory allocation error\n");
    return 1;
  }

  videoMemory = vm_map_phys(SELF, (void*) mr.mr_base, frameSize);
  if (videoMemory == NULL) {
    printf("Virtual memory allocation error\n");
    return 1;
  }
  return 0;
}

int (set_video_mode)(uint16_t mode){

  double_buffer = (uint8_t*) malloc(get_frame_size());

  if (double_buffer == NULL) {
    printf("Failed to allocate double buffer\n");
    return 1;
  }

  //BIOS
  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.ah = VBE_SET_MODE_FUNCTION;
  r.al = VBE_SET_MODE_SUBFUNCTION;
  r.bx = BIT(14) | mode; //LINEAR MODE
  r.intno = BIOS_INTERRUPT_GRAPHICS_CARD;

  if(sys_int86(&r) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int draw_pixel(uint16_t x, uint16_t y, uint32_t color){
  //Out of resolution
  if (x >= vmi.XResolution || y >= vmi.YResolution) return 1;

  unsigned int bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8;
  unsigned int index = (y * vmi.XResolution + x) * bytes_per_pixel;

  memcpy(double_buffer + index, &color, bytes_per_pixel);
  return 0;
}

int draw_rectangle(int x, int y, int width, int height, uint32_t color) {
  for (int i = y; i < y + height; i++) {
    for (int j = x; j < x + width; j++) {
      if (draw_pixel(j, i, color) != OK) {
        return 1; 
      }
    }
  }
  return 0;
}

int draw_xpm(uint8_t *map, uint16_t x, uint16_t y, xpm_image_t img){
  for (int h = 0 ; h < img.height ; h++) {
    for (int w = 0 ; w < img.width ; w++) {
      uint8_t color = map[h * img.width + w]; // selects color by index
      
      if (color != 0){
        draw_pixel(x + w, y + h, color);
      }
    }
  }
  return 0;
}

void update_screen() {
  memcpy(videoMemory, double_buffer, get_frame_size());
}
