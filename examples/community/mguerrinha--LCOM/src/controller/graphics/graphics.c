#include "graphics.h"

int (set_graphics_mode)(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.ax = 0x4F02;
  r.bx = mode | BIT(14);
  r.intno = 0x10;
  if (sys_int86(&r)) return 1;
  return 0;
}

int (set_text_mode)() {
  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.ax = 0x0003;
  r.bx = 0x0000;
  r.intno = 0x10;
  if (sys_int86(&r)) return 1;
  return 0;
}

int (set_frame_buffer)(uint16_t mode, uint8_t **frame_buffer) {
  memset(&mode_info, 0, sizeof(mode_info));
  if(vbe_get_mode_info(mode, &mode_info)) return 1;
  unsigned int bpp = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bpp;
  struct minix_mem_range mr;
  mr.mr_base = mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + frame_size;
  if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) return 1;
  *frame_buffer = vm_map_phys(SELF, (void*) mr.mr_base, frame_size);
  if(frame_buffer == NULL) return 1;
  return 0;
}

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *frame_buffer) {
  if (x >= mode_info.XResolution || y >= mode_info.YResolution) return 1;
  unsigned int bpp = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int index = (y * mode_info.XResolution + x) * bpp;
  if (memcpy(&frame_buffer[index], &color, bpp) == NULL) return 1;
  return 0;
}

int (draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t *frame_buffer) {
  for (unsigned int i = 0; i < len; i++) {
    if (draw_pixel(x, y+i, color, frame_buffer)) return 1;
  }
  return 0;
}
