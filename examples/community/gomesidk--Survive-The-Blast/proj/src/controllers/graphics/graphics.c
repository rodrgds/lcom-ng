#include <lcom/lcf.h>
#include "graphics.h"

static vbe_mode_info_t mode_info;
static uint8_t *video_mem;
static uint32_t screen_size;
static uint8_t bytes_per_pixel;
static uint8_t *front_buffer;
static uint8_t *back_buffer;
static uint8_t cur_buffer = 1; // 1 = front, 0 = back

vbe_mode_info_t get_mode_info() {
  return mode_info;
}

void set_video_mode(uint16_t mode) {
  struct reg86 args;
  memset(&args, 0, sizeof(args)); 
  args.ah = 0x4F; // VBE function
  args.al = 0x02; // "set mode" function
  args.bx = VBE_LINEAR_FB | mode; 
  args.intno = 0x10; // BIOS video service
  sys_int86(&args);

  if (args.al != 0x4F || args.ah != 0x00) panic("VBE call failed");
}

void map_graphics_vram(uint16_t mode) {
  vbe_get_mode_info(mode, &mode_info); 
  
  bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
  screen_size = mode_info.BytesPerScanLine * mode_info.YResolution;

  struct minix_mem_range mr;
  memset(&mr, 0, sizeof(mr));
  mr.mr_base = (phys_bytes)mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + 2 * (screen_size);

  // Request permission for our process to map memory
  sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr);

  // Map memory
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, mr.mr_limit - mr.mr_base);
  if (video_mem == MAP_FAILED) panic("Failed to map video memory");

  front_buffer = video_mem;
  back_buffer = video_mem + screen_size;
  cur_buffer = 1;
}

void set_display_start(uint32_t addr) {

  struct reg86 r;
  memset(&r, 0, sizeof(r));
  r.ax = 0x4F07;      
  r.bx = 0x0000;      
  r.cx = 0;      
  r.dx = addr;      
  r.intno = 0x10;  

  if (sys_int86(&r) != OK) {
    panic("set_display_start failed");
  }
}

void vg_flip_buffer() {
  if (cur_buffer) {
    front_buffer = video_mem + screen_size;
    back_buffer = video_mem;
    cur_buffer = 0;
    set_display_start(mode_info.YResolution);
  } else {
    front_buffer = video_mem;
    back_buffer = video_mem + screen_size;
    cur_buffer = 1;
    set_display_start(0); 
  }
  
}

void clear_back_buffer() {
  memset(back_buffer, 0, screen_size);
}

void draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= mode_info.XResolution || y >= mode_info.YResolution) {
    panic("Coordinates out of bounds");
  }
  
  unsigned int index = (mode_info.XResolution * y + x) * bytes_per_pixel;

  if (color == CHROMA_KEY_GREEN_888) { 
    return; // Skip drawing transparent color
  }

  if (memcpy(back_buffer + index, &color, bytes_per_pixel) == NULL) {
    panic("Failed to copy color to video memory");
  }
}

int vg_draw_xpm(uint8_t* xpm_map, uint16_t x, uint16_t y, uint16_t width, uint16_t height){
  if (xpm_map == NULL) {
    return 1;
  }

  for (uint16_t i = 0; i < height; i++) {
    for (uint16_t j = 0; j < width; j++) {

      uint32_t color = 0;
      memcpy(&color, xpm_map, bytes_per_pixel);
      
      draw_pixel(x + j, y + i, color);
      xpm_map += bytes_per_pixel;
    }
  }

  return 0;
}

int vg_draw_xpm_by_line(uint8_t* xpm_map, uint16_t x,uint16_t y, uint16_t width, uint16_t height) {
  if (xpm_map == NULL) {
    return 1;
  }

  if (x + width > mode_info.XResolution || y + height > mode_info.YResolution) {
    return 1; // Out of bounds
  }

  size_t offset = (mode_info.XResolution * y + x) * bytes_per_pixel;

  for (uint16_t i = 0; i < height; i++) {
    memcpy(back_buffer + offset, xpm_map, width * bytes_per_pixel);
    xpm_map += width * bytes_per_pixel;
    offset += mode_info.BytesPerScanLine;
  }

  return 0;
}

int vg_draw_xpm_by_image(uint8_t* xpm_map, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  if(xpm_map == NULL || width != mode_info.XResolution || height != mode_info.YResolution) {
    return 1; // Invalid parameters
  }

  memcpy(back_buffer, xpm_map, screen_size);
  return 0; // Success
}

