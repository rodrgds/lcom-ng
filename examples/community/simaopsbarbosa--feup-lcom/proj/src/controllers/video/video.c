#include "video.h"
#include <lcom/lcf.h>
#include <math.h>
#include "font8x8_basic.h"

int enable_graphic_mode(uint16_t res) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));
  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02;
  r86.bx = res | BIT(14);
  if (sys_int86(&r86) != 0) {
    printf("[ERROR] sys_int86() failed \n");
    return 1;
  }
  return 0;
}

// read 4.2 - Mapping Video RAM in a Process's Address Space
int get_frame_buffer(uint16_t mode, uint8_t** frame_buffer) {
  memset(&mode_info, 0, sizeof(mode_info)); // clear the structure
  if (vbe_get_mode_info(mode, &mode_info)) {
    return 1;
  };

  struct minix_mem_range phys_addr;
  phys_addr.mr_base = mode_info.PhysBasePtr;
  uint32_t frame_size =
      mode_info.XResolution * mode_info.YResolution * (mode_info.BitsPerPixel + 7) >> 3;
  phys_addr.mr_limit = phys_addr.mr_base + frame_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &phys_addr)) {
    printf("[ERROR] sys_privctl() failed \n");
    return 1;
  }

  *frame_buffer = vm_map_phys(SELF, (void *)phys_addr.mr_base, frame_size);
  if (frame_buffer == NULL) {
    printf("[ERROR] failed to allocate virtual memory : frame buffer is empty \n");
    return 1;
  }

  return 0;
}

int video_draw_pixel(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer) {
  if (x >= mode_info.XResolution) {
    printf("[ERROR] video_draw_pixel() failed : x is out of bounds \n");
    return 1;
  }
  if (y >= mode_info.YResolution) {
    printf("[ERROR] video_draw_pixel() failed : y is out of bounds \n");
    return 1;
  }

  uint16_t bytes_per_pixel = (mode_info.BitsPerPixel + 7) >> 3;
  uint32_t index = (mode_info.XResolution * y + x) * bytes_per_pixel;

  if (memcpy(&frame_buffer[index], &color, bytes_per_pixel) == NULL) {
    printf("[ERROR] video_draw_pixel() failed : memcpy() null \n");
    return 1;
  };

  return 0;
}

int(video_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t* frame_buffer) {
  if (x >= mode_info.XResolution) {
    printf("[ERROR] video_draw_hline() failed : x is out of bounds \n");
    return 1;
  }
  if (y >= mode_info.YResolution) {
    printf("[ERROR] video_draw_hline() failed : y is out of bounds \n");
    return 1;
  }
  if (len == 0) {
    printf("[ERROR] video_draw_hline() failed : length is zero \n");
    return 1;
  }

  for (uint16_t i = x; i < x + len; i++) {
    if (video_draw_pixel(i, y, color, frame_buffer)) {
      printf("[ERROR] video_draw_hline() failed : video_draw_pixel() failed \n");
      return 1;
    }
  }

  return 0;
}

int(video_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t* frame_buffer) {
  if (x >= mode_info.XResolution) {
    printf("[ERROR] video_draw_rectangle() failed : x is out of bounds \n");
    return 1;
  }
  if (y >= mode_info.YResolution) {
    printf("[ERROR] video_draw_rectangle() failed : y is out of bounds \n");
    return 1;
  }
  if (width == 0 || height == 0) {
    printf("[ERROR] video_draw_rectangle() failed : width or height is zero \n");
    return 1;
  }

  for (uint16_t i = y; i < y + height; i++) {
    if (video_draw_hline(x, i, width, color, frame_buffer)) {
      printf("[ERROR] video_draw_rectangle() failed : video_draw_hline() failed \n");
      return 1;
    }
  }

  return 0;
}

int video_draw_xpm(struct Sprite sprite, uint16_t x, uint16_t y, uint8_t* frame_buffer) {
  uint32_t current_color;

  for (int h = 0 ; h < sprite.img.height ; h++) {
    for (int w = 0 ; w < sprite.img.width ; w++) {
      current_color = sprite.data[w + h * sprite.img.width];
      if (current_color == TRANSPARENT_COLOR) continue;
      if (video_draw_pixel(x + w, y + h, current_color, frame_buffer) != 0) {
        printf("[ERROR] video_draw_pixel() failed \n");
        return 1;
      }
    }
  }

  return 0;
}

int video_draw_char(uint16_t x, uint16_t y, char c, uint32_t color, uint8_t* frame_buffer, uint8_t scale) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (font8x8_basic[(uint8_t)c][row] & (1 << (7 - col))) {
        for (uint8_t dy = 0; dy < scale; dy++) {
          for (uint8_t dx = 0; dx < scale; dx++) {
            video_draw_pixel(x + col * scale + dx, y + row * scale + dy, color, frame_buffer);
          }
        }
      }
    }
  }
  return 0;
}

int video_draw_text(uint16_t x, uint16_t y, uint32_t color, char* text, uint8_t* frame_buffer, uint8_t scale) {
  for (int i = 0; text[i] != '\0'; i++) {
    video_draw_char(x + i * 8 * scale, y, text[i], color, frame_buffer, scale);
  }
  return 0;
}

int video_draw_centered_text(uint16_t y, uint32_t color, char* text, uint8_t* frame_buffer, uint8_t scale) {
  uint16_t text_width = strlen(text) * 8 * scale;
  uint16_t x = (mode_info.XResolution - text_width) / 2;
  return video_draw_text(x, y, color, text, frame_buffer, scale);
}
