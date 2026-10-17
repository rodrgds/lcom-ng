#include "video_graphics.h"
#include <lcom/lcf.h>

uint8_t *frame_buffer;
uint8_t *frame_buffer2;


int init_video(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.ah = BIO_CALL;
  r.al = VBE_MODE;
  r.bx = LINEAR_FB|mode; 
  r.intno = VBE_INTNO;
  if( sys_int86(&r) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int (flip_page)() {
  reg86_t r2;
  memset(&r2, 0, sizeof(r2));
  unsigned int offset;
  if (current_buffer == ONE) {
    offset = mode_conf.YResolution;
    current_buffer = TWO;
  } else {
    offset = 0;  
    current_buffer = ONE;
  }

  r2.ah = BIO_CALL;
  r2.al = START_OF_DISPLAY; 
  r2.bh = 0x00;   
  r2.bl = 0x80;    
  r2.cx = 0;  
  r2.dx = offset;
  r2.intno = VBE_INTNO;

  if( sys_int86(&r2) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
  return 0; 
}

int (set_double_frame_buffer)() {
  int r;
  current_buffer = ONE;
  struct minix_mem_range mr; 
  unsigned int vram_base;
  unsigned int vram_size; 
  vram_base = mode_conf.PhysBasePtr;
  unsigned Bytes_per_pixel = (mode_conf.BitsPerPixel + 7) / 8;
  vram_size = mode_conf.XResolution * mode_conf.YResolution * Bytes_per_pixel;
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return 1;
  }
  frame_buffer = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(frame_buffer == MAP_FAILED) {
    panic("couldn’t map video memory");
    return 1;
  }
  struct minix_mem_range mr2; 
  mr2.mr_base = mr.mr_limit;
  mr2.mr_limit = mr2.mr_base + vram_size;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr2))) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return 1;
  }
  frame_buffer2 = vm_map_phys(SELF, (void *)mr2.mr_base, vram_size);
  if(frame_buffer2 == MAP_FAILED) {
    panic("couldn’t map video memory");
    return 1;
  }
  return 0;
}

int (get_mode_conf)(uint16_t mode) {
  memset(&mode_conf, 0, sizeof(mode_conf));
  if (vbe_get_mode_info(mode, &mode_conf) != 0) return 1;
  return 0;
}

int (draw_pixel) (uint32_t color, uint16_t x, uint16_t y) {
  if (((x > mode_conf.XResolution) || (y > mode_conf.YResolution))) return 1;
  unsigned Bytes_per_pixel = ((mode_conf.BitsPerPixel + 7) / 8);
  unsigned int pos = ((mode_conf.XResolution * y + x) * Bytes_per_pixel);
  if (color != xpm_transparency_color(XPM_8_8_8_8)) {
    if (current_buffer == ONE) {
      if (memcpy(&frame_buffer2[pos], &color, Bytes_per_pixel) == NULL) return 1;
    }
    else if (current_buffer == TWO){
      if (memcpy(&frame_buffer[pos], &color, Bytes_per_pixel) == NULL) return 1;
    }
    return 0;
  }
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = x; i < x + len; i++) {
    if (draw_pixel(color, i, y) != 0) return 1;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int j = y; j < y + height; j++) {
    if (vg_draw_hline(x, j, width, color) != 0) {
      vg_exit();
      return 1;
    }
  }
  return 0;
}

int (draw_Sprite)(Sprite* s, uint16_t x, uint16_t y) {
  for (int i = 0; i < s->h; i++) {
    for (int j = 0; j < s->w; j++) {
      if (draw_pixel(s->color_map[j + i * s->w], x + j, y + i) != 0) return 1;
    }
  }
  return 0;
}

int (draw_background)() {
  unsigned Bytes_per_pixel = ((mode_conf.BitsPerPixel + 7) / 8);
  if (current_buffer == ONE) {
    if (memcpy(frame_buffer2, background, mode_conf.XResolution * mode_conf.YResolution * Bytes_per_pixel) == NULL) return 1;
  }
  else if (current_buffer == TWO){
    if (memcpy(frame_buffer, background, mode_conf.XResolution * mode_conf.YResolution * Bytes_per_pixel) == NULL) return 1;
  }
  return 0;
}

int (clear_screen)() {
  unsigned Bytes_per_pixel = ((mode_conf.BitsPerPixel + 7) / 8);
  if (current_buffer == ONE) {
    if (memset(frame_buffer2, 0, mode_conf.XResolution * mode_conf.YResolution * Bytes_per_pixel) == NULL) return 1;
  }
  else if (current_buffer == TWO){
    if (memset(frame_buffer, 0, mode_conf.XResolution * mode_conf.YResolution * Bytes_per_pixel) == NULL) return 1;
  }
  return 0;
}

unsigned (get_frame_size)() {
  unsigned Bytes_per_pixel = ((mode_conf.BitsPerPixel + 7) / 8);
  return mode_conf.XResolution * mode_conf.YResolution * Bytes_per_pixel;
}

int (loadBackground)(Sprite* s) {
  for (int i = 0; i < s->h; i++) {
    for (int j = 0; j < s->w; j++) {
      unsigned Bytes_per_pixel = ((mode_conf.BitsPerPixel + 7) / 8);
      unsigned int pos = ((mode_conf.XResolution * i + j) * Bytes_per_pixel);
      if (memcpy(&background[pos], &s->color_map[j + i * s->w], Bytes_per_pixel) == NULL) return 1;
    }
  }
  return 0;
}

int (drawSpriteToBackground)(Sprite* s, uint16_t x, uint16_t y) {
  for (int i = 0; i < s->h; i++) {
    for (int j = 0; j < s->w; j++) {
      unsigned Bytes_per_pixel = ((mode_conf.BitsPerPixel + 7) / 8);
      unsigned int pos = ((mode_conf.XResolution * (y + i) +  x + j) * Bytes_per_pixel);
      if (s->color_map[j + i * s->w] != xpm_transparency_color(XPM_8_8_8_8)) {
        if (memcpy(&background[pos], &s->color_map[j + i * s->w], Bytes_per_pixel) == NULL) return 1;
      }
    }
  }
  return 0;
}

int (drawRectangleToBackground)(uint32_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t heigth) {
  for (int i = 0; i < heigth; i++) {
    for (int j = 0; j < width; j++) {
      unsigned Bytes_per_pixel = ((mode_conf.BitsPerPixel + 7) / 8);
      unsigned int pos = ((mode_conf.XResolution * (y + i) +  x + j) * Bytes_per_pixel);
      if (memcpy(&background[pos], &color, Bytes_per_pixel) == NULL) return 1;
    }
  }
  return 0;
}

void free_buffers() {
  free(frame_buffer);
  free(frame_buffer2);
}


