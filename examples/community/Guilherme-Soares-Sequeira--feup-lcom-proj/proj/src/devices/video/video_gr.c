// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/video_gr.h>

#include <math.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/color.h"
#include "vbe.h"
#include "video_gr.h"

vbe_mode_info_t vg_mode_info; /**< @brief current graphics mode information */
void *front_buffer;           /**< @brief the front buffer which is directly mapped to VRAM */
void *back_buffer;            /**< @brief the back buffer which is mapped in memory and replaces the front buffer in every render */

int bytes_per_pixel = 0; /**< @brief number of bytes per pixel */
bool indexed = false;    /**< @brief weather the current graphics mode is indexed or not */

void *(vg_get_back_buffer) () {
  return back_buffer;
}

void(_get_mode_info)(in_port_t mode) {
  vbe_get_mode_info(mode, &vg_mode_info);
}

void(_map_memory)() {
  front_buffer = 0;
  struct minix_mem_range mr;                         /* physical memory range */
  unsigned int vram_base = vg_mode_info.PhysBasePtr; /* VRAM’s physical addresss */
  bytes_per_pixel = (vg_mode_info.BitsPerPixel + vg_mode_info.LinRsvdMaskSize) / 8;
  unsigned int vram_size = bytes_per_pixel * vg_mode_info.XResolution * vg_mode_info.YResolution;

  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  int r;
  if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK)
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  front_buffer = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (front_buffer == MAP_FAILED)
    panic("couldn't map video memory");

  back_buffer = malloc(vram_size);
}

void *(vg_init) (in_port_t graphics_mode) {
  _get_mode_info(graphics_mode);
  _map_memory();

  struct reg86 r;
  memset(&r, 0, sizeof(r));

  r.intno = VBE_INTERFACE;
  r.ax = VBE_SET_MODE;
  r.bx = graphics_mode | LINEAR_MODE;

  if (sys_int86(&r) || r.al != 0x4F || r.ah != 0)
    return NULL;

  indexed = graphics_mode == 0x105;

  return back_buffer;
}

#define COLOR_SIZE_MASK(x) (BIT(x) - 1)

int(vg_draw_pixel_index)(uint32_t i, uint32_t color) {
  memcpy((uint8_t *) back_buffer + i, &color, bytes_per_pixel);

  return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= vg_mode_info.XResolution || x < 0 || y >= vg_mode_info.YResolution || y < 0)
    return 0;

  color &= COLOR_SIZE_MASK(vg_mode_info.BitsPerPixel);

  int pixel_offset = x + y * vg_mode_info.XResolution;
  int byte_offset = pixel_offset * bytes_per_pixel;

  uint8_t *pixel_start = (uint8_t *) back_buffer + byte_offset;

  for (int j = 0; j < bytes_per_pixel; j++) {
    pixel_start[j] = color & 0xFF;
    color >>= 8;
  }

  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  int i = y * vg_mode_info.XResolution + x;

  memset((uint8_t *) back_buffer + i * bytes_per_pixel, color, len * bytes_per_pixel);

  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  int i = (x + y * vg_mode_info.XResolution) * bytes_per_pixel;
  int offset = vg_mode_info.XResolution * bytes_per_pixel;
  width *= bytes_per_pixel;

  for (int j = 0; j < height; ++j, i += offset)
    memset((uint8_t *) back_buffer + i, color, width);

  return 0;
}

int(vg_draw_circle)(uint16_t x, uint16_t y, uint16_t radius, uint32_t color) {
  int32_t top_left_x = x - radius;
  int32_t top_left_y = y - radius;
  int32_t max_distance = radius * radius;
  int32_t x32 = x, y32 = y; // So that calculations don't overflow

  for (int32_t i = top_left_x; i <= top_left_x + 2 * radius; i++) {
    for (int32_t j = top_left_y; j <= top_left_y + 2 * radius; j++) {
      if ((x32 - i) * (x32 - i) + (y32 - j) * (y32 - j) <= max_distance) {
        if (vg_draw_pixel(i, j, color)) {
          return 1;
        }
      }
    }
  }

  return 0;
}

int(_vg_draw_line_low)(uint16_t from_x, uint16_t from_y, uint16_t to_x, uint16_t to_y, int16_t dx, int16_t dy, int color) {
  int yi = 1;

  if (dy < 0) {
    yi = -1;
    dy = -dy;
  }

  int D = 2 * dy - dx;
  int y = from_y;

  for (int x = from_x; x <= to_x; x++) {
    if (vg_draw_pixel(x, y, color))
      return 1;

    if (D > 0) {
      y += yi;
      D += 2 * (dy - dx);
    }
    else
      D += 2 * dy;
  }

  return OK;
}

int(_vg_draw_line_high)(uint16_t from_x, uint16_t from_y, uint16_t to_x, int16_t to_y, int16_t dx, int16_t dy, int color) {
  int xi = 1;

  if (dx < 0) {
    xi = -1;
    dx = -dx;
  }

  int D = 2 * dx - dy;
  int x = from_x;

  for (int y = from_y; y <= to_y; y++) {
    if (vg_draw_pixel(x, y, color))
      return 1;

    if (D > 0) {
      x += xi;
      D += 2 * (dx - dy);
    }
    else
      D += 2 * dx;
  }

  return OK;
}

int(vg_draw_line)(uint16_t from_x, uint16_t from_y, uint16_t to_x, uint16_t to_y, int color) {
  int16_t dx = to_x - from_x;
  int16_t dy = to_y - from_y;
  if (abs(dy) < abs(dx))
    if (from_x > to_x)
      return _vg_draw_line_low(to_x, to_y, from_x, from_y, -dx, -dy, color);
    else
      return _vg_draw_line_low(from_x, from_y, to_x, to_y, dx, dy, color);
  else if (from_y > to_y)
    return _vg_draw_line_high(to_x, to_y, from_x, from_y, -dx, -dy, color);
  else
    return _vg_draw_line_high(from_x, from_y, to_x, to_y, dx, dy, color);
}

xpm_image_t(vg_load_xpm)(const xpm_map_t map) {
  xpm_image_t info;
  xpm_load(map, XPM_INDEXED, &info);
  return info;
}

int(vg_draw_xpm)(const xpm_image_t xpm_info, uint16_t x, uint16_t y) {
  uint8_t *bytes = xpm_info.bytes;

  for (unsigned long int i = 0; i < (unsigned long) xpm_info.width * xpm_info.height; i++) {
    if (bytes[i] != COLOR_TRANSPARENT && vg_draw_pixel(
                                           x + (i % xpm_info.width),
                                           y + (i / xpm_info.width),
                                           (uint32_t) * (bytes + i)))
      return 1;
  }

  return OK;
}

void getRGB(uint32_t color, uint8_t *r, uint8_t *g, uint8_t *b) {
  *r = (color >> vg_mode_info.RedFieldPosition) & COLOR_SIZE_MASK(vg_mode_info.RedMaskSize);
  *g = (color >> vg_mode_info.GreenFieldPosition) & COLOR_SIZE_MASK(vg_mode_info.GreenMaskSize);
  *b = (color >> vg_mode_info.BlueFieldPosition) & COLOR_SIZE_MASK(vg_mode_info.BlueMaskSize);
}

uint32_t makeRGB(uint8_t r, uint8_t g, uint8_t b) {
  return 0 | ((r & COLOR_SIZE_MASK(vg_mode_info.RedMaskSize)) << vg_mode_info.RedFieldPosition) | ((g & COLOR_SIZE_MASK(vg_mode_info.GreenMaskSize)) << vg_mode_info.GreenFieldPosition) | ((b & COLOR_SIZE_MASK(vg_mode_info.BlueMaskSize)) << vg_mode_info.BlueFieldPosition);
}

void(clear_screen)(uint8_t color) {
  memset(back_buffer, color, bytes_per_pixel * vg_mode_info.XResolution * vg_mode_info.YResolution);
}

void(flip)() {
  memcpy(front_buffer, back_buffer, bytes_per_pixel * vg_mode_info.XResolution * vg_mode_info.YResolution);
}
