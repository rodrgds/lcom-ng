#include "video_gr_ext.h"

#include <lcom/vbe.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t *video_mem;
static uint8_t *back_buffer;
static lcom_vbe_mode_info_t vmi;
static unsigned int h_res;
static unsigned int v_res;
static unsigned int bytes_per_pixel;
static unsigned int scanline_bytes;
static size_t frame_buffer_size;

static int parse_xpm_header(const char *line, int *width, int *height, int *colors, int *cpp) {
  return sscanf(line, "%d %d %d %d", width, height, colors, cpp) == 4;
}

static uint32_t parse_hex_color(const char *s, bool *transparent) {
  *transparent = false;
  while (*s != '\0' && isspace((unsigned char)*s)) s++;
  if (strncmp(s, "None", 4) == 0) {
    *transparent = true;
    return 0;
  }
  if (*s != '#') return 0;
  unsigned r = 0;
  unsigned g = 0;
  unsigned b = 0;
  if (sscanf(s + 1, "%02x%02x%02x", &r, &g, &b) != 3) return 0;
  return 0xFF000000u | (b & 0xFFu) | ((g & 0xFFu) << 8) | ((r & 0xFFu) << 16);
}

static void write_color(uint8_t *dst, uint32_t color) {
  dst[0] = (uint8_t)(color & 0xFFu);
  if (bytes_per_pixel > 1) dst[1] = (uint8_t)((color >> 8) & 0xFFu);
  if (bytes_per_pixel > 2) dst[2] = (uint8_t)((color >> 16) & 0xFFu);
  if (bytes_per_pixel > 3) dst[3] = (uint8_t)((color >> 24) & 0xFFu);
}

static bool sprite_pixel_transparent(const vg_sprite_t *sprite, const uint8_t *px) {
  if (sprite->bytes_per_pixel >= 4) return px[3] == 0;
  uint32_t color = 0;
  memcpy(&color, px, sprite->bytes_per_pixel);
  return color == 0;
}

static void copy_sprite_pixel(const vg_sprite_t *sprite, const uint8_t *src, uint8_t *dst) {
  uint8_t count = bytes_per_pixel < sprite->bytes_per_pixel ? (uint8_t)bytes_per_pixel : sprite->bytes_per_pixel;
  memcpy(dst, src, count);
  for (uint8_t i = count; i < bytes_per_pixel; i++) dst[i] = 0;
}

void *vg_init(uint16_t mode) {
  if (lcom_vbe_get_mode_info(mode, &vmi) != LCOM_OK) return NULL;
  if (lcom_vbe_set_mode(mode) != LCOM_OK) return NULL;
  if (lcom_phys_map(vmi.framebuffer_phys, vmi.framebuffer_size, (void **)&video_mem) != LCOM_OK) {
    video_mem = NULL;
    return NULL;
  }
  h_res = vmi.width;
  v_res = vmi.height;
  bytes_per_pixel = vmi.bytes_per_pixel;
  scanline_bytes = vmi.pitch;
  frame_buffer_size = (size_t)vmi.framebuffer_size;
  return video_mem;
}

int vg_exit(void) {
  if (video_mem != NULL) {
    lcom_phys_unmap(video_mem, frame_buffer_size);
    video_mem = NULL;
  }
  return 0;
}

unsigned int get_h_res(void) { return h_res; }
unsigned int get_v_res(void) { return v_res; }
unsigned int get_bytes_per_pixel(void) { return bytes_per_pixel; }
char *get_video_mem(void) { return (char *)video_mem; }
char *get_back_buffer(void) { return (char *)back_buffer; }
size_t vg_get_frame_buffer_size(void) { return frame_buffer_size; }

int vg_prepare_buffer(void) {
  if (video_mem == NULL || frame_buffer_size == 0) return 1;
  if (back_buffer != NULL) return 0;
  back_buffer = calloc(frame_buffer_size, 1);
  return back_buffer == NULL ? 1 : 0;
}

void vg_dispose_buffer(void) {
  free(back_buffer);
  back_buffer = NULL;
}

int vg_clear_buffer(uint32_t color) {
  if (back_buffer == NULL) return 1;
  return vg_fill_rect_to_buffer(0, 0, (uint16_t)h_res, (uint16_t)v_res, color);
}

int vg_fill_rect_to_buffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (back_buffer == NULL) return 1;
  if (x >= h_res || y >= v_res || width == 0 || height == 0) return 0;
  if ((uint32_t)x + width > h_res) width = (uint16_t)(h_res - x);
  if ((uint32_t)y + height > v_res) height = (uint16_t)(v_res - y);

  uint8_t pixel[4] = {0, 0, 0, 0};
  write_color(pixel, color);
  for (uint16_t row = 0; row < height; row++) {
    uint8_t *dst = back_buffer + (size_t)(y + row) * scanline_bytes + (size_t)x * bytes_per_pixel;
    for (uint16_t col = 0; col < width; col++) {
      memcpy(dst, pixel, bytes_per_pixel);
      dst += bytes_per_pixel;
    }
  }
  return 0;
}

int vg_present_buffer(void) {
  if (video_mem == NULL || back_buffer == NULL) return 1;
  memcpy(video_mem, back_buffer, frame_buffer_size);
  return lcom_vbe_present() == LCOM_OK ? 0 : 1;
}

int vg_load_xpm_sprite(vg_sprite_t *sprite, xpm_map_t xpm) {
  if (sprite == NULL || xpm == NULL) return 1;
  int width = 0;
  int height = 0;
  int colors = 0;
  int cpp = 0;
  if (!parse_xpm_header(xpm[0], &width, &height, &colors, &cpp) ||
      width <= 0 || height <= 0 || colors <= 0 || cpp <= 0 || cpp > 2) {
    return 1;
  }

  uint32_t *table = calloc(65536u, sizeof(uint32_t));
  bool *transparent = calloc(65536u, sizeof(bool));
  if (table == NULL || transparent == NULL) {
    free(table);
    free(transparent);
    return 1;
  }

  for (int i = 0; i < colors; i++) {
    const char *line = xpm[1 + i];
    uint16_t key = cpp == 1 ? (uint8_t)line[0] : (uint16_t)(((uint8_t)line[0] << 8) | (uint8_t)line[1]);
    const char *color = strstr(line + cpp, " c ");
    if (color == NULL) color = strstr(line + cpp, "\tc ");
    if (color == NULL) continue;
    bool is_transparent = false;
    table[key] = parse_hex_color(color + 3, &is_transparent);
    transparent[key] = is_transparent;
  }

  uint8_t *pixels = calloc((size_t)width * height * 4u, 1);
  if (pixels == NULL) {
    free(table);
    free(transparent);
    return 1;
  }

  for (int row = 0; row < height; row++) {
    const char *line = xpm[1 + colors + row];
    for (int col = 0; col < width; col++) {
      const char *p = line + col * cpp;
      uint16_t key = cpp == 1 ? (uint8_t)p[0] : (uint16_t)(((uint8_t)p[0] << 8) | (uint8_t)p[1]);
      uint8_t *dst = pixels + ((size_t)row * width + col) * 4u;
      if (transparent[key]) {
        memset(dst, 0, 4);
      } else {
        uint32_t color = table[key];
        memcpy(dst, &color, 4);
      }
    }
  }

  free(table);
  free(transparent);
  sprite->pixels = pixels;
  sprite->width = (uint16_t)width;
  sprite->height = (uint16_t)height;
  sprite->bytes_per_pixel = 4;
  return 0;
}

void vg_destroy_sprite(vg_sprite_t *sprite) {
  if (sprite == NULL) return;
  free(sprite->pixels);
  sprite->pixels = NULL;
  sprite->width = 0;
  sprite->height = 0;
  sprite->bytes_per_pixel = 0;
}

int vg_capture_buffer_to_sprite(vg_sprite_t *sprite) {
  return vg_capture_region_to_sprite(sprite, 0, 0);
}

int vg_capture_region_to_sprite(vg_sprite_t *sprite, uint16_t x, uint16_t y) {
  if (back_buffer == NULL || sprite == NULL || sprite->pixels == NULL) return 1;
  if ((uint32_t)x + sprite->width > h_res || (uint32_t)y + sprite->height > v_res) return 1;

  for (uint16_t row = 0; row < sprite->height; row++) {
    const uint8_t *src = back_buffer + (size_t)(y + row) * scanline_bytes + (size_t)x * bytes_per_pixel;
    uint8_t *dst = sprite->pixels + (size_t)row * sprite->width * sprite->bytes_per_pixel;
    for (uint16_t col = 0; col < sprite->width; col++) {
      memcpy(dst, src, bytes_per_pixel < sprite->bytes_per_pixel ? bytes_per_pixel : sprite->bytes_per_pixel);
      if (sprite->bytes_per_pixel >= 4) dst[3] = 0xFF;
      src += bytes_per_pixel;
      dst += sprite->bytes_per_pixel;
    }
  }
  return 0;
}

int vg_draw_sprite_region_scaled_to_buffer(const vg_sprite_t *sprite, uint16_t src_x, uint16_t src_y,
                                           uint16_t width, uint16_t height, int16_t dst_x,
                                           int16_t dst_y, uint16_t scale) {
  if (back_buffer == NULL || sprite == NULL || sprite->pixels == NULL || scale == 0) return 1;
  if ((uint32_t)src_x + width > sprite->width || (uint32_t)src_y + height > sprite->height) return 1;

  for (uint16_t source_row = 0; source_row < height; source_row++) {
    for (uint16_t source_col = 0; source_col < width; source_col++) {
      const uint8_t *source_pixel = sprite->pixels +
                                    ((size_t)(src_y + source_row) * sprite->width + src_x + source_col) *
                                        sprite->bytes_per_pixel;
      if (sprite_pixel_transparent(sprite, source_pixel)) continue;

      for (uint16_t scale_row = 0; scale_row < scale; scale_row++) {
        int32_t screen_y = dst_y + (int32_t)source_row * scale + scale_row;
        if (screen_y < 0 || screen_y >= (int32_t)v_res) continue;
        for (uint16_t scale_col = 0; scale_col < scale; scale_col++) {
          int32_t screen_x = dst_x + (int32_t)source_col * scale + scale_col;
          if (screen_x < 0 || screen_x >= (int32_t)h_res) continue;
          uint8_t *destination = back_buffer + (size_t)screen_y * scanline_bytes +
                                 (size_t)screen_x * bytes_per_pixel;
          copy_sprite_pixel(sprite, source_pixel, destination);
        }
      }
    }
  }
  return 0;
}

int vg_draw_sprite_frame_scaled_to_buffer(const vg_sprite_t *sprite_sheet, uint16_t frame_width,
                                          uint16_t frame_height, uint16_t column, uint16_t row,
                                          int16_t dst_x, int16_t dst_y, uint16_t scale) {
  if (sprite_sheet == NULL) return 1;
  return vg_draw_sprite_region_scaled_to_buffer(sprite_sheet,
                                                (uint16_t)(column * frame_width),
                                                (uint16_t)(row * frame_height),
                                                frame_width,
                                                frame_height,
                                                dst_x,
                                                dst_y,
                                                scale);
}

int vg_opaque_sprite_to_buffer(const vg_sprite_t *sprite, int16_t dst_x, int16_t dst_y) {
  if (back_buffer == NULL || sprite == NULL || sprite->pixels == NULL) return 1;
  if (dst_x >= (int16_t)h_res || dst_y >= (int16_t)v_res) return 0;
  if (dst_x <= -(int16_t)sprite->width || dst_y <= -(int16_t)sprite->height) return 0;

  uint16_t src_x = 0;
  uint16_t src_y = 0;
  uint16_t copy_width = sprite->width;
  uint16_t copy_height = sprite->height;

  if (dst_x < 0) {
    src_x = (uint16_t)(-dst_x);
    copy_width = (uint16_t)(copy_width - src_x);
    dst_x = 0;
  }
  if (dst_y < 0) {
    src_y = (uint16_t)(-dst_y);
    copy_height = (uint16_t)(copy_height - src_y);
    dst_y = 0;
  }
  if ((uint32_t)dst_x + copy_width > h_res) copy_width = (uint16_t)(h_res - dst_x);
  if ((uint32_t)dst_y + copy_height > v_res) copy_height = (uint16_t)(v_res - dst_y);

  for (uint16_t row = 0; row < copy_height; row++) {
    const uint8_t *source = sprite->pixels + ((size_t)(src_y + row) * sprite->width + src_x) *
                                                 sprite->bytes_per_pixel;
    uint8_t *destination = back_buffer + (size_t)(dst_y + row) * scanline_bytes +
                           (size_t)dst_x * bytes_per_pixel;
    for (uint16_t col = 0; col < copy_width; col++) {
      copy_sprite_pixel(sprite, source, destination);
      source += sprite->bytes_per_pixel;
      destination += bytes_per_pixel;
    }
  }
  return 0;
}
