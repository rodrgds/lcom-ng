#include <lcom/lcf.h>

#include "graphic.h"

#include "VBE.h"

vbe_mode_info_t mode_info;

static uint8_t *video_mem;
static void *buffer;
static void *current;

static unsigned horizontal;
static unsigned vertical;
static unsigned bits_per_pixel;
static unsigned bytes_per_pixel;
static unsigned vram_size;

void *(vg_init) (uint16_t mode) {
  if (vbe_get_mode_info(mode, &mode_info)) return NULL;

  horizontal = mode_info.XResolution;
  vertical = mode_info.YResolution;
  bits_per_pixel = mode_info.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel + 7) / 8;

  struct minix_mem_range mr;
  unsigned int vram_base = mode_info.PhysBasePtr;
  vram_size = horizontal * vertical * bytes_per_pixel;

  struct minix_mem_range mr_buffer;
  unsigned int buffer_base = vram_base + vram_size;

  mr.mr_base = (phys_bytes) vram_base;
  mr_buffer.mr_base = (phys_bytes) buffer_base;

  mr.mr_limit = mr.mr_base + vram_size * 2;
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) return NULL;

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED) return NULL;

  buffer = vm_map_phys(SELF, (void *) mr_buffer.mr_base, vram_size);
  if (buffer == MAP_FAILED) return NULL;

  memset(video_mem, 0, vram_size);
  memset(buffer, 0, vram_size);
 
  reg86_t r;

  memset(&r, 0, sizeof(r));
  r.intno = 0x10;
  r.ah = 0x4F;
  r.al = 0x02;
  r.bx = mode | BIT(14);
 
  if (sys_int86(&r)) return NULL;

  current = buffer;

  return video_mem;
}

int(video_swap_buffers)() {
 
  reg86_t r;
 
  memset(&r, 0, sizeof(r));
  r.intno = 0x10;
  r.ah = 0x4F;
  r.al = 0x07;
  r.bh = 0x00;
  r.bl = 0x01;

  if (sys_int86(&r)) return 1;

  uint16_t dx;
  if (r.dx == 0) {
    dx = vertical;
    current = video_mem;
    memcpy(current, buffer, vram_size);
  } else if (r.dx == vertical) {
    dx = 0;
    current = buffer;
    memcpy(current, video_mem, vram_size);
  }
  else return 1;

  memset(&r, 0, sizeof(r));
  r.intno = 0x10;
  r.ah = 0x4F;
  r.al = 0x07;
  r.bh = 0x00;
  r.bl = 0x00;
  r.cx = 0;  
  r.dx = dx; 

 
  if (sys_int86(&r)) return 1;
  return 0;
}

int(video_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x < 0 || x >= horizontal || y < 0 || y >= vertical) return 1;

  uint8_t *byte = current;
  byte += (x + y * horizontal) * bytes_per_pixel;

  return memcpy(byte, &color, bytes_per_pixel) == NULL;
}

int(video_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (uint16_t i = 0; i < len; i++) {
    if (video_draw_pixel(x + i, y, color)) {
      return 1;
    }
  }
  return 0;
}

int(video_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i = 0; i < height; i++) {
    if (video_draw_hline(x, y + i, width, color)) {
      return 1;
    }
  }
  return 0;
}


int (video_draw_background)(uint8_t *background) {
  return memcpy(current, background, vram_size) == NULL;
}
