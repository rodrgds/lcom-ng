/**
 * @file graphics.c
 * @author
 * @brief
 * @version 0.1
 * @date 2023-05-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "graphics.h"
#include <lcom/lcf.h>
#include <math.h>

int(set_graphic_mode)(uint16_t mode) {
  reg86_t register86;
  memset(&register86, 0, sizeof(register86));
  register86.intno = 0x10;
  register86.ax = 0x4F02;
  register86.bx = mode | BIT(14);
  return sys_int86(&register86);
}

int(set_fb)(uint16_t mode, uint8_t **fb) {
  memset(&mode_info, 0, sizeof(mode_info));
  vbe_get_mode_info(mode, &mode_info);
  unsigned int frame_size = (mode_info.XResolution * mode_info.YResolution * mode_info.BitsPerPixel) / 8;
  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = mode_info.PhysBasePtr;
  physic_addresses.mr_limit = physic_addresses.mr_base + frame_size;
  sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses);
  *fb = vm_map_phys(SELF, (void *) physic_addresses.mr_base, frame_size);
  if (fb == NULL)
    return 1;
  return 0;
}

int(draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *fb) {
  if (x >= mode_info.XResolution || y >= mode_info.YResolution)
    return 1;
  unsigned bpp = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int i = (mode_info.XResolution * y + x) * bpp;
  memcpy(&fb[i], &color, bpp);
  return 0;
}

int(draw_horizontal_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t *fb) {
  for (unsigned i = 0; i < len; i++) draw_pixel(x + i, y, color, fb);
  return 0;
}

int(draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *fb) {
  for (unsigned i = 0; i < height; i++) draw_horizontal_line(x, y + i, width, color, fb);
  return 0;
}
