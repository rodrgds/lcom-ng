/*
 * @file graphics.c
 * @brief Low-level VBE graphics module with linear framebuffer and double buffering.
 *
 * This module handles VBE mode initialization, VRAM mapping, drawing to a
 * software back buffer, and copying that buffer to video memory.
 *
 * All other modules should use this API instead of accessing VRAM directly.
 */

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"

static uint8_t *video_mem = NULL;
static uint8_t *back_buffer = NULL;

static vbe_mode_info_t mode_info;

static uint16_t h_res = 0;
static uint16_t v_res = 0;

static uint8_t bits_per_pixel = 0;
static uint8_t bytes_per_pixel = 0;

static uint16_t bytes_per_scan_line = 0;
static size_t frame_size = 0;

static int graphics_initialized = 0;

static int graphics_set_mode(uint16_t mode) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  /*
   * VBE function 0x4F02 sets the video mode.
   * BIT(14) requests the linear framebuffer version of the mode.
   */
  r86.intno = 0x10;
  r86.ax = 0x4F02;
  r86.bx = mode | BIT(14);

  if (sys_int86(&r86) != OK) {
    printf("graphics_set_mode: sys_int86 failed\n");
    return 1;
  }

  /*
   * VBE returns AL = 0x4F if the function is supported.
   * AH = 0x00 means the function completed successfully.
   */
  if (r86.al != 0x4F || r86.ah != 0x00) {
    printf("graphics_set_mode: VBE call failed, AL=0x%02X AH=0x%02X\n", r86.al, r86.ah);
    return 1;
  }

  return 0;
}

static int graphics_validate_mode_info(void) {
  if (mode_info.XResolution == 0 || mode_info.YResolution == 0) {
    printf("graphics_validate_mode_info: invalid resolution\n");
    return 1;
  }

  if (mode_info.BitsPerPixel == 0) {
    printf("graphics_validate_mode_info: invalid bits per pixel\n");
    return 1;
  }

  if (mode_info.PhysBasePtr == 0) {
    printf("graphics_validate_mode_info: invalid physical base pointer\n");
    return 1;
  }

  /*
   * ModeAttributes BIT(0): mode is supported.
   * ModeAttributes BIT(7): linear framebuffer is available.
   */
  if (!(mode_info.ModeAttributes & BIT(0))) {
    printf("graphics_validate_mode_info: mode not supported\n");
    return 1;
  }

  if (!(mode_info.ModeAttributes & BIT(7))) {
    printf("graphics_validate_mode_info: linear framebuffer not supported\n");
    return 1;
  }

  return 0;
}

static int graphics_map_vram(void) {
  struct minix_mem_range mr;

  mr.mr_base = mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + frame_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK) {
    printf("graphics_map_vram: sys_privctl failed\n");
    return 1;
  }

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, frame_size);

  if (video_mem == MAP_FAILED || video_mem == NULL) {
    printf("graphics_map_vram: vm_map_phys failed\n");
    video_mem = NULL;
    return 1;
  }

  return 0;
}

static void graphics_reset_state(void) {
  video_mem = NULL;
  back_buffer = NULL;

  memset(&mode_info, 0, sizeof(mode_info));

  h_res = 0;
  v_res = 0;

  bits_per_pixel = 0;
  bytes_per_pixel = 0;

  bytes_per_scan_line = 0;
  frame_size = 0;

  graphics_initialized = 0;
}

/*
 * @brief Initializes the selected VBE graphics mode and allocates the back buffer.
 *
 * @param mode VBE mode number, for example 0x105.
 * @return 0 on success, non-zero otherwise.
 */
int graphics_init(uint16_t mode) {
  if (graphics_initialized) {
    return 0;
  }

  graphics_reset_state();

  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    printf("graphics_init: vbe_get_mode_info failed\n");
    return 1;
  }

  if (graphics_validate_mode_info() != 0) {
    return 1;
  }

  h_res = mode_info.XResolution;
  v_res = mode_info.YResolution;

  bits_per_pixel = mode_info.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel + 7) / 8;

  bytes_per_scan_line = mode_info.BytesPerScanLine;

  if (bytes_per_scan_line == 0) {
    bytes_per_scan_line = h_res * bytes_per_pixel;
  }

  frame_size = (size_t) bytes_per_scan_line * v_res;

  if (graphics_map_vram() != 0) {
    graphics_reset_state();
    return 1;
  }

  back_buffer = malloc(frame_size);

  if (back_buffer == NULL) {
    printf("graphics_init: could not allocate back buffer\n");
    graphics_reset_state();
    return 1;
  }

  memset(back_buffer, 0, frame_size);

  if (graphics_set_mode(mode) != 0) {
    free(back_buffer);
    graphics_reset_state();
    return 1;
  }

  graphics_initialized = 1;

  return 0;
}

/*
 * @brief Frees graphics resources and returns to text mode.
 *
 * @return 0 on success, non-zero otherwise.
 */
int graphics_exit(void) {
  if (back_buffer != NULL) {
    free(back_buffer);
    back_buffer = NULL;
  }

  graphics_initialized = 0;

  if (vg_exit() != 0) {
    printf("graphics_exit: vg_exit failed\n");
    return 1;
  }

  video_mem = NULL;

  return 0;
}

/*
 * @brief Clears the whole back buffer with the given color.
 *
 * @param color Color value to write.
 */
void graphics_clear_screen(uint32_t color) {
  if (back_buffer == NULL) {
    return;
  }

  /*
   * In 8 bpp modes each pixel has exactly 1 byte.
   * Therefore memset is safe and much faster.
   */
  if (bytes_per_pixel == 1) {
    memset(back_buffer, (uint8_t) color, frame_size);
    return;
  }

  for (uint16_t y = 0; y < v_res; y++) {
    uint8_t *row = back_buffer + (size_t) y * bytes_per_scan_line;

    for (uint16_t x = 0; x < h_res; x++) {
      memcpy(row + (size_t) x * bytes_per_pixel, &color, bytes_per_pixel);
    }
  }
}

/*
 * @brief Copies a full-screen software buffer into the back buffer.
 *
 * The source buffer must have graphics_get_frame_size() bytes and the same
 * scanline layout as the active VBE mode.
 *
 * @param buffer Source frame buffer to copy.
 */
void graphics_copy_buffer_to_back(const uint8_t *buffer) {
  if (back_buffer == NULL || buffer == NULL) {
    return;
  }

  memcpy(back_buffer, buffer, frame_size);
}

/*
 * @brief Copies a full-screen software buffer into the back buffer with vertical wrap.
 *
 * @param buffer Source frame buffer to copy.
 * @param y_offset Source row used as the first row on screen.
 */
void graphics_copy_buffer_to_back_scrolled(const uint8_t *buffer, uint16_t y_offset) {
  if (back_buffer == NULL || buffer == NULL || v_res == 0) {
    return;
  }

  y_offset %= v_res;

  if (y_offset == 0) {
    memcpy(back_buffer, buffer, frame_size);
    return;
  }

  size_t row_size = bytes_per_scan_line;
  size_t first_rows = v_res - y_offset;
  size_t first_size = first_rows * row_size;
  size_t second_size = (size_t) y_offset * row_size;

  memcpy(back_buffer, buffer + (size_t) y_offset * row_size, first_size);
  memcpy(back_buffer + first_size, buffer, second_size);
}

/*
 * @brief Copies the back buffer to the physical video memory.
 */
void graphics_swap_buffers(void) {
  if (video_mem == NULL || back_buffer == NULL) {
    return;
  }

  memcpy(video_mem, back_buffer, frame_size);
}

/*
 * @brief Draws one pixel into the back buffer.
 *
 * @param x Pixel x coordinate.
 * @param y Pixel y coordinate.
 * @param color Color value.
 * @return 0 on success, non-zero otherwise.
 */
int graphics_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (back_buffer == NULL) return 1;
  if (x >= h_res || y >= v_res) return 1;

  size_t index = (size_t) y * bytes_per_scan_line + (size_t) x * bytes_per_pixel;

  if (bytes_per_pixel == 1) {
    back_buffer[index] = (uint8_t) color;
  }
  else {
    memcpy(back_buffer + index, &color, bytes_per_pixel);
  }
  return 0;
}

int graphics_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (back_buffer == NULL) return 1;
  if (width == 0 || height == 0) return 0;
  if (x >= h_res || y >= v_res) return 1;

  uint32_t x_end = (uint32_t) x + width;
  uint32_t y_end = (uint32_t) y + height;

  if (x_end > h_res) x_end = h_res;
  if (y_end > v_res) y_end = v_res;

  uint16_t draw_width = (uint16_t) (x_end - x);
  uint16_t draw_height = (uint16_t) (y_end - y);

  if (draw_width == 0 || draw_height == 0) return 0;

  size_t row_bytes = (size_t) draw_width * bytes_per_pixel;
  uint8_t *first_row = back_buffer +
                       (size_t) y * bytes_per_scan_line +
                       (size_t) x * bytes_per_pixel;

  if (bytes_per_pixel == 1) {
    uint8_t c = (uint8_t) color;

    for (uint16_t row = 0; row < draw_height; row++) {
      uint8_t *dst = first_row + (size_t) row * bytes_per_scan_line;
      memset(dst, c, draw_width);
    }

    return 0;
  }

  for (uint16_t col = 0; col < draw_width; col++) {
    memcpy(first_row + (size_t) col * bytes_per_pixel,
           &color,
           bytes_per_pixel);
  }

  for (uint16_t row = 1; row < draw_height; row++) {
    uint8_t *dst = first_row + (size_t) row * bytes_per_scan_line;
    memcpy(dst, first_row, row_bytes);
  }

  return 0;
}

int graphics_draw_rectangle_clipped(int x, int y, int width, int height, uint32_t color) {
  if (back_buffer == NULL) return 1;
  if (width <= 0 || height <= 0) return 0;

  int screen_width = h_res;
  int screen_height = v_res;

  if (x >= screen_width || y >= screen_height) return 0;
  if (x + width <= 0 || y + height <= 0) return 0;

  if (x < 0) {
    width += x;
    x = 0;
  }

  if (y < 0) {
    height += y;
    y = 0;
  }

  if (x + width > screen_width) {
    width = screen_width - x;
  }

  if (y + height > screen_height) {
    height = screen_height - y;
  }

  if (width <= 0 || height <= 0) return 0;

  return graphics_draw_rectangle((uint16_t) x,
                                 (uint16_t) y,
                                 (uint16_t) width,
                                 (uint16_t) height,
                                 color);
}

uint16_t graphics_get_width(void) {
  return h_res;
}

uint16_t graphics_get_height(void) {
  return v_res;
}

uint8_t graphics_get_bytes_per_pixel(void) {
  return bytes_per_pixel;
}

uint16_t graphics_get_bytes_per_scanline(void) {
  return bytes_per_scan_line;
}

size_t graphics_get_frame_size(void) {
  return frame_size;
}
