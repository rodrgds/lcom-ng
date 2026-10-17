/** 
 * @file graphics.c
 * @brief Source code file for all the logic related to the graphics and vbe
 */



#include "graphics.h"


static char* buffer = NULL; ///< Secondary buffer for the video memory
static char* video_mem = NULL; ///< Pointer to the real video memory
static vbe_mode_info_t mode_info; ///< Stores the current mode info
static int bytes_per_pixel; ///< Stores the number of bytes per pixel to avoid repetitive divisions

void* (vg_get_video_mem)(){
  return buffer;
}

vbe_mode_info_t (vg_get_current_mode_info)(){
  return mode_info;
}

unsigned int (vg_get_hres)(){
  return mode_info.XResolution;
}

unsigned int (vg_get_vres)(){
  return mode_info.YResolution;
}

unsigned int (vg_get_bits_per_pixel)(){
  return mode_info.BitsPerPixel;
}

bool (vg_is_direct_color_model)(){
  return mode_info.MemoryModel == DIRECT_COLOR;
}

uint32_t (vg_get_red_color_field)(const uint32_t color){
  return (color >> mode_info.RedFieldPosition) & ((1 << mode_info.RedMaskSize) - 1);
}

uint32_t (vg_get_green_color_field)(const uint32_t color){
  return (color >> mode_info.GreenFieldPosition) & ((1 << mode_info.GreenMaskSize) - 1);
}

uint32_t (vg_get_blue_color_field)(const uint32_t color){
  return (color >> mode_info.BlueFieldPosition) & ((1 << mode_info.BlueMaskSize) - 1);
}

int (vg_graphics_init)(uint16_t mode){
  if(vg_map_video_memory(mode)) return 1;
  return vg_set_VBE_mode(mode);
}

int (vg_set_VBE_mode)(uint16_t mode){
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.intno = VIDEO_INT_INSTRUCTION;
  r86.ah = VBE_FUNCTION;
  r86.al = VBE_SET_MODE;
  r86.bx = LINEAR_FRAME_BUFFER | mode;
  
  if(sys_int86(&r86) != OK) return 1;
  if(r86.al != VBE_FUNCTION_SUPPORTED) return 1; // function not supported
  if(r86.ah) return 1; // function not successful

  return 0;
}


int (vg_map_video_memory)(uint16_t mode){
  struct minix_mem_range mr;
  unsigned int vram_base;
  unsigned int vram_size;
  
  int r;    

  /* Use VBE function 0x01 to initialize vram_base and vram_size */

  // get the mode information
  if(vbe_get_mode_info(mode, &mode_info)) return 1;

  // ceil accounts for bits per pixel like mode 0x110 (not multiple of 8)
  bytes_per_pixel = ceil((double)vg_get_bits_per_pixel()/8);

  vram_base = mode_info.PhysBasePtr;
  vram_size = vg_get_hres()*vg_get_vres()*bytes_per_pixel;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  

  if(OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return 1;
  }

  /* Map memory */

  buffer = (char*)malloc(vram_size);
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED) {
    panic("couldn't map video memory");
    return 1;
  }

  return 0;
}


int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if(buffer == NULL) return 1;
  if(x >= vg_get_hres() || y >= vg_get_vres()) return 1;

  unsigned offset = (y * vg_get_hres() + x) * bytes_per_pixel;
  memcpy(buffer + offset, &color, bytes_per_pixel);

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if(buffer == NULL) return 1;
  if(x >= vg_get_hres() || y >= vg_get_vres()) return 1;

  unsigned offset = (y * vg_get_hres() + x) * bytes_per_pixel;
  for(unsigned int i = x; i < x + len && i < vg_get_hres(); ++i){
    memcpy(buffer + offset, &color, bytes_per_pixel);
    offset += bytes_per_pixel;
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if(buffer == NULL) return 1;
  if(x >= vg_get_hres() || y >= vg_get_vres()) return 1;

  for(int i = y; i < y + height; ++i){
    vg_draw_hline(x, i, width, color);
  }

  return 0;
}

int (vg_draw_rectangle32)(int32_t x, int32_t y, uint16_t width, uint16_t height, uint32_t color) {
  if(buffer == NULL) return 1;
  if(x >= (int32_t)vg_get_hres() || x + (int32_t)width < 0 ||
     y >= (int32_t)vg_get_vres() || y + (int32_t)height < 0 ) return 1;

  if(color == COLOR32_TRANSPARENT) return 0; // no need to even draw

  uint32_t x_start = MAX(0, x), x_end = MIN((int32_t)vg_get_hres(), x + (int32_t)width);
  uint32_t y_start = MAX(0, y), y_end = MIN((int32_t)vg_get_vres(), y + (int32_t)height);

  uint32_t* vmem_address = (uint32_t *)buffer + y_start * vg_get_hres();

  for(uint32_t y_off = y_start; y_off < y_end; ++y_off){
    for(uint32_t x_off = x_start; x_off < x_end; ++x_off){
      memcpy(vmem_address + x_off, &color, 4);
    }
    vmem_address += vg_get_hres(); // move to next vmem line
  }

  return 0;
}

int (vg_draw_image32)(int32_t x, int32_t y, xpm_image_t* img_info) {
  if(buffer == NULL) return 1;
  if(x >= (int32_t)vg_get_hres() || x + (int32_t)img_info->width < 0 ||
     y >= (int32_t)vg_get_vres() || y + (int32_t)img_info->height < 0 ) return 1;

  uint32_t x_start = MAX(0, x), x_end = MIN((int32_t)vg_get_hres(), x + (int32_t)img_info->width);
  uint32_t y_start = MAX(0, y), y_end = MIN((int32_t)vg_get_vres(), y + (int32_t)img_info->height);

  uint32_t* vmem_address = (uint32_t *)buffer + y_start*vg_get_hres();
  uint32_t* img_address = (uint32_t *)img_info->bytes + (y_start - y)*img_info->width;

  for(uint32_t y_off = y_start; y_off < y_end; ++y_off){
    for(uint32_t x_off = x_start; x_off < x_end; ++x_off){
      if(*(img_address + x_off - x) != COLOR32_TRANSPARENT)
        memcpy(vmem_address + x_off, img_address + x_off - x, 4);
    }
    vmem_address += vg_get_hres(); // move to next vmem line
    img_address += img_info->width; // move to next image line
  }

  return 0;
}

int (vg_draw_image_section32)(int32_t x, int32_t y, xpm_image_t* img_info, uint16_t x_section,  uint16_t y_section, uint16_t width, uint16_t height) {
  if(buffer == NULL) return 1;
  if(x >= (int32_t)vg_get_hres() || x + (int32_t)width < 0 ||
     y >= (int32_t)vg_get_vres() || y + (int32_t)height < 0 ||
     x_section >= img_info->width || y_section >= img_info->height) return 1;

  uint32_t x_start = MAX(0, x), x_end = MIN((int32_t)vg_get_hres(), x + (int32_t)width);
  uint32_t y_start = MAX(0, y), y_end = MIN((int32_t)vg_get_vres(), y + (int32_t)height);

  uint32_t* vmem_address = (uint32_t *)buffer + y_start*vg_get_hres();
  uint32_t* img_address = (uint32_t *)img_info->bytes + y_section*img_info->width + x_section - x_start;

  for(uint32_t y_off = y_start; y_off < y_end; ++y_off){
    for(uint32_t x_off = x_start; x_off < x_end; ++x_off){
      if(*(img_address + x_off) != COLOR32_TRANSPARENT)
        memcpy(vmem_address + x_off, img_address + x_off, 4);
    }
    vmem_address += vg_get_hres(); // move to next vmem line
    img_address += img_info->width; // move to next image line
  }

  return 0;
}

int (vg_show_frame)() {
  if(video_mem == NULL) return 1;
  memcpy(video_mem, buffer, vg_get_hres()*vg_get_vres()*bytes_per_pixel);
  return 0;
}
