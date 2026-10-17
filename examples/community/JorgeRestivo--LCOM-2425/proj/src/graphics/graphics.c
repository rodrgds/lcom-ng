#include "graphics.h"
#include <lcom/lcf.h>
#include <math.h>
#include "constants.h"
#include "font8x8.h"

uint8_t *frame_buffer;
uint8_t *double_buffer;
unsigned int frame_size;

int(set_graphic_mode(uint16_t submode)) {
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;
  reg86.ah = 0x4F;
  reg86.al = 0x02;
  reg86.bx = submode | BIT(14);

  if (sys_int86(&reg86) != 0) {
    printf("Graphic mode failed \n"); //TODO: doesnt return 1 on fail
  }

  return 0;
}

int(set_text_mode)() {
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;
  reg86.ah = 0x00;
  reg86.al = 0x03;
  reg86.bx = 0x0000;

  if (sys_int86(&reg86) != 0) {
    printf("Text mode failed \n");
  }

  return 0;
}

int(set_frame_buffer)(uint16_t mode) {

  memset(&mode_info, 0, sizeof(mode_info));

  if (vbe_get_mode_info(mode, &mode_info)) {
    return 1;
  }

  unsigned int bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;
  frame_size = mode_info.XResolution * mode_info.YResolution * bytes_p_pixel;

  struct minix_mem_range phys_addr;
  phys_addr.mr_base = mode_info.PhysBasePtr;
  phys_addr.mr_limit = phys_addr.mr_base + frame_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &phys_addr)) {
    printf("Physical memory alloc error \n");
    return 1;
  }

  frame_buffer = vm_map_phys(SELF, (void *) phys_addr.mr_base, frame_size);
  if (frame_buffer == NULL) {
    printf("Virtual memory alloc error \n");
    return 1;
  }


  double_buffer = malloc(frame_size);

  return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {

  if (x > mode_info.XResolution || y > mode_info.YResolution) {
    return 1;
  }

  unsigned BytesPixel = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int index = (mode_info.XResolution * y + x) * BytesPixel;

  if (BytesPixel == 1) {
    double_buffer[index] = (uint8_t) color;
  } else {
    memcpy(&double_buffer[index], &color, BytesPixel);
  }

  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

  for (unsigned i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color) != 0) {
      return 1;
    }
  }

  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  for (unsigned i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color) != 0) {
      vg_exit();
      return 1;
    }
  }

  return 0;
}

int normalize_color(uint32_t color, uint32_t *new_color) {

  if (mode_info.BitsPerPixel == 32) {
    *new_color = color;
  }
  else {
    *new_color = color & (BIT(mode_info.BitsPerPixel) - 1);
  }

  return 0;
}

uint32_t(direct_mode)(uint32_t R, uint32_t G, uint32_t B) {
  return (R << mode_info.RedFieldPosition) | (G << mode_info.GreenFieldPosition) | (B << mode_info.BlueFieldPosition);
}

uint32_t(indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
  return (first + (row * n + col) * step) % (1 << mode_info.BitsPerPixel);
}

uint32_t(Red)(unsigned j, uint8_t step, uint32_t first) {
  return (R(first) + j * step) % (1 << mode_info.RedMaskSize);
}

uint32_t(Green)(unsigned i, uint8_t step, uint32_t first) {
  return (G(first) + i * step) % (1 << mode_info.GreenMaskSize);
}

uint32_t(Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first) {
  return (B(first) + (i + j) * step) % (1 << mode_info.BlueMaskSize);
}

uint32_t(R)(uint32_t first) {
  return ((1 << mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition);
}

uint32_t(G)(uint32_t first) {
  return ((1 << mode_info.GreenMaskSize) - 1) & (first >> mode_info.GreenFieldPosition);
}

uint32_t(B)(uint32_t first) {
  return ((1 << mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition);
}

int(print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  xpm_image_t image;

  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &image);

  for (int i = 0; i < image.height; i++) {
    for (int j = 0; j < image.width; j++) {

      if (vg_draw_pixel(x + j, y + i, *colors) != 0) {
        return 1;
      }
      colors++;
    }
  }

  return 0;
}

int (load_xpm)(const char *const *map, xpm_image_t *img) {
    
    if (map == NULL || img == NULL) return 1;

    xpm_map_t aux = (xpm_map_t)(uintptr_t) map;
    uint8_t *colors = xpm_load(aux, XPM_INDEXED, img);
    if (colors == NULL) return 1;
    
    img->bytes = (uint8_t *)colors;
    return 0;
  }
  
  /**
   * Draws a sprite with transparency (0x000000 = transparent)
   */
  void (draw_sprite)(xpm_image_t *sprite, uint16_t x, uint16_t y) {
    if (sprite == NULL || sprite->bytes == NULL) return;
    
    for (uint16_t i = 0; i < sprite->height; i++) {
      for (uint16_t j = 0; j < sprite->width; j++) {

        uint8_t color = sprite->bytes[i * sprite->width + j];
        if (color != 0) { 
          vg_draw_pixel(x + j, y + i, color);
        }
      }
    }
  }
  int (draw_background)(xpm_image_t *bg) {
    if (bg == NULL) return 1;

    uint8_t *vram = double_buffer;
    uint8_t *source = bg->bytes;
    unsigned bytes_p_pixel = (mode_info.BitsPerPixel + 7) / 8;

    for(uint16_t v = 0; v < bg->height; v++){
      for(uint16_t h = 0; h < bg->width; h++){
        size_t i = (v*bg->width + h) * bytes_p_pixel;
        size_t vram_i = (v*mode_info.XResolution + h)*bytes_p_pixel;

        memcpy(&vram[vram_i], &source[i], bytes_p_pixel);
      }
    }
    return 0;
  }


void* (vg_initialize)(uint16_t mode){
  if (set_graphic_mode(mode) != 0){
    return NULL;
  }
  if (set_frame_buffer(mode) != 0){
    return NULL;
  }

  return frame_buffer;
}

void (vg_clear_screen)() {
  memset(double_buffer, 0, frame_size);
}

void (vg_draw_text)(int x, int y, const char *text, uint32_t color) {
  const int char_width = 8;
  const int char_height = 8;

  for (int i = 0; text[i] != '\0'; i++) {
    int px = x + i * (char_width + 1);
    vg_draw_rectangle(px, y, char_width, char_height, color);
  }
}




void swap_buffer(){
  memcpy(frame_buffer, double_buffer, frame_size);
}


