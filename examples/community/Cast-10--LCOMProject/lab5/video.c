#include <lcom/lcf.h>
#include <string.h>
#include <stdlib.h>

#include "video.h"
#include "vbe.h"
#include "keyboard.h"
#include "i8042.h"

static char *video_mem;          /* VRAM base address */

static vbe_mode_info_t vmi;
static unsigned bytes_per_pixel;

static unsigned int frame_size;  /* size of one full screen in bytes */
static unsigned int vram_size;   /* total mapped VRAM size */

static uint8_t front_page = 0;
static uint8_t back_page = 1;


static char *video_get_back_buffer() {
  return video_mem + back_page * frame_size; 

}


int(set_vram_mapping)(uint16_t mode) {
  struct minix_mem_range mr;
  unsigned int vram_base;

  if (vbe_get_mode_info(mode, &vmi) != 0) {
    return 1;
  }

  if (vmi.NumberOfImagePages < 1) {
    return 1;
  }

  bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8;

  vram_base = vmi.PhysBasePtr;

  frame_size = vmi.BytesPerScanLine * vmi.YResolution;
  vram_size = frame_size * 2;

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK) {
    return 1;
  }

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED) {
    return 1;
  }

  front_page = 0;
  back_page = 1;

  return 0;
}

static int video_set_display_start(uint16_t x, uint16_t y) {
  reg86_t r86;

  memset(&r86, 0, sizeof(r86));

  r86.intno = BIOS_VIDEO_INT;

  r86.ah = VBE_FUNCTION_AH;
  r86.al = VBE_SET_DISPLAY_START_AL;

  r86.bh = 0x00;
  r86.bl = VBE_SET_DISPLAY_START_VERTICAL;

  r86.cx = x;
  r86.dx = y;

  if (sys_int86(&r86) != OK) {
    return 1;
  }

  if (r86.al != VBE_SUPPORTED || r86.ah != VBE_SUCCESS) {
    return 1;
  }

  return 0;
}


int vbe_set_mode(uint16_t mode) {
  reg86_t r86;

  memset(&r86, 0, sizeof(r86));

  r86.intno = BIOS_VIDEO_INT;

  r86.ah = VBE_FUNCTION_AH;
  r86.al = VBE_SET_MODE_AL;

  r86.bx = mode | LINEAR_FRAMEBUFFER;

  if (sys_int86(&r86) != OK) {
    return 1;
  }

  if (r86.al != VBE_SUPPORTED || r86.ah != VBE_SUCCESS) {
    return 1;
  }

  return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (video_mem == NULL) {
    return 1;
  }

  if (x >= vmi.XResolution || y >= vmi.YResolution) {
    return 1;
  }

  char *buffer = video_get_back_buffer();

  unsigned int offset = y * vmi.BytesPerScanLine + x * bytes_per_pixel;

  memcpy(&buffer[offset], &color, bytes_per_pixel);

  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (video_mem == NULL) {
    return 1;
  }

  if (y >= vmi.YResolution) {
    return 1;
  }

  if ((uint32_t) x + len > vmi.XResolution) {
    return 1;
  }

  char *buffer = video_get_back_buffer();

  char *dst = buffer +
              y * vmi.BytesPerScanLine +
              x * bytes_per_pixel;

  if (bytes_per_pixel == 1) {
    memset(dst, (uint8_t) color, len);
    return 0;
  }

  for (uint16_t i = 0; i < len; i++) {
    memcpy(dst + i * bytes_per_pixel, &color, bytes_per_pixel);
  }

  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y,
                       uint16_t width, uint16_t height,
                       uint32_t color) {
  if (video_mem == NULL) {
    return 1;
  }

  if (width == 0 || height == 0) {
    return 0;
  }

  if ((uint32_t) x + width > vmi.XResolution) {
    return 1;
  }

  if ((uint32_t) y + height > vmi.YResolution) {
    return 1;
  }

  char *buffer = video_get_back_buffer();

  if (bytes_per_pixel == 1) {
    for (uint16_t row = 0; row < height; row++) {
      char *dst = buffer +
                  (y + row) * vmi.BytesPerScanLine +
                  x;

      memset(dst, (uint8_t) color, width);
    }

    return 0;
  }

  unsigned int row_bytes = width * bytes_per_pixel;

  char *first_row = buffer +
                    y * vmi.BytesPerScanLine +
                    x * bytes_per_pixel;

  for (uint16_t col = 0; col < width; col++) {
    memcpy(first_row + col * bytes_per_pixel, &color, bytes_per_pixel);
  }

  for (uint16_t row = 1; row < height; row++) {
    char *dst = buffer +
                (y + row) * vmi.BytesPerScanLine +
                x * bytes_per_pixel;

    memcpy(dst, first_row, row_bytes);
  }

  return 0;
}

static uint32_t rgb_to_mode_color(uint8_t r, uint8_t g, uint8_t b) {
  uint32_t color = 0;

  color |= ((uint32_t)(r >> (8 - vmi.RedMaskSize))) << vmi.RedFieldPosition;
  color |= ((uint32_t)(g >> (8 - vmi.GreenMaskSize))) << vmi.GreenFieldPosition;
  color |= ((uint32_t)(b >> (8 - vmi.BlueMaskSize))) << vmi.BlueFieldPosition;

  return color;
}

int(vg_draw_pixmap)(uint8_t *pixmap,
                    uint16_t width,
                    uint16_t height,
                    int x,
                    int y,
                    uint32_t transparent_color) {
  if (pixmap == NULL || video_mem == NULL) {
    return 1;
  }

  char *buffer = video_get_back_buffer();

  for (uint16_t row = 0; row < height; row++) {
    int screen_y = y + row;

    if (screen_y < 0 || screen_y >= vmi.YResolution) {
      continue;
    }

    for (uint16_t col = 0; col < width; col++) {
      int screen_x = x + col;

      if (screen_x < 0 || screen_x >= vmi.XResolution) {
        continue;
      }

      /*
       * XPM_8_8_8 = 3 bytes por pixel:
       * R G B
       */
      size_t src_index = ((size_t)row * width + col) * 3;

      uint8_t b = pixmap[src_index];
      uint8_t g = pixmap[src_index + 1];
      uint8_t r = pixmap[src_index + 2];

      uint32_t rgb_color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;

      if (rgb_color == transparent_color) {
        continue;
      }

      uint32_t color = rgb_to_mode_color(r, g, b);

      unsigned int offset =
          screen_y * vmi.BytesPerScanLine + screen_x * bytes_per_pixel;

      memcpy(&buffer[offset], &color, bytes_per_pixel);
    }
  }

  return 0;
}

int(vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t img;

  uint8_t *pixmap = xpm_load(xpm, XPM_INDEXED, &img);

  if (pixmap == NULL) {
    return 1;
  }

  int result = vg_draw_pixmap(pixmap,img.width,img.height,x,y,0xFFFFF);

  free(pixmap);

  return result;
}

int video_swap_buffers() {
  if (video_set_display_start(0, back_page * vmi.YResolution) != 0) {
    return 1;
  }

  uint8_t temp = front_page;
  front_page = back_page;
  back_page = temp;

  return 0;
}

int video_clear_back_buffer(const uint8_t *background) {
    char *buffer = video_get_back_buffer();

    if (buffer == NULL) {
        return 1;
    }

    if (background != NULL) {
        memcpy(buffer, background, frame_size);
        return 0;
    }

    memset(buffer, 0, frame_size);
    return 0;
}

int video_init_page_flipping(uint16_t mode) {
  if (set_vram_mapping(mode) != 0) {
    return 1;
  }

  if (vbe_set_mode(mode) != 0) {
    return 1;
  }

  return 0;
}



