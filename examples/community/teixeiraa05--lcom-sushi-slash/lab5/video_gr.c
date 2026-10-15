#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <string.h>

#include "vesa-bios.h"
#include "video_gr.h"

static char *video_mem;
static vbe_mode_info_t vmi;
static unsigned bytes_per_pixel;

static char *vram_ptr0;
static char *vram_ptr1;
static unsigned page_size = 0;
static uint8_t active_page = 1;

int(set_video_mode)(uint16_t mode) {

  reg86_t reg;
  memset(&reg, 0, sizeof(reg));

  reg.intno = VBE_INT;
  reg.ah = AH_VBE;
  reg.al = AL_SET_MODE;

  reg.bx = CHECK_BIT_SET | mode;

  if (sys_int86(&reg) != OK) {
    printf("sys_int86() failed \n");
    return 1;
  }

  return 0;
}

void *(graphics_init)(uint16_t mode) {

  memset(&vmi, 0, sizeof(vmi));
  if (vbe_get_mode_info(mode, &vmi))
    return NULL;

  bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8;

  int r;
  struct minix_mem_range mr;

  /* Need to allocate memory for the video mode */
  unsigned int vram_base = vmi.PhysBasePtr;
  unsigned int vram_size = vmi.XResolution * vmi.YResolution * bytes_per_pixel;

  mr.mr_base = (phys_bytes)vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  /* Allow memory mapping */
  if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return NULL;
  }

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED) {
    panic("couldn't map video memory");
    return NULL;
  }

  if (set_video_mode(mode))
    return NULL;

  return video_mem;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x < 0 || y < 0)
    return 1;
  if (x >= vmi.XResolution || y >= vmi.YResolution)
    return 1;

  // ignore chroma key green pixels (used in place of transparency)
  if (bytes_per_pixel == 3 && (color & 0x00FFFFFF) == CHROMA_KEY_GREEN_888)
    return 0;
  if (bytes_per_pixel == 2 && (color & 0x0000FFFF) == CHROMA_KEY_GREEN_565)
    return 0;

  // Handle mode 0x110 where the color is represented in 15 bits
  if (vmi.BitsPerPixel == 15)
    color &= 0x7FFF;

  // calculate the offset in the video memory for the pixel at (x, y)
  uint32_t pixel_offset = (y * vmi.XResolution + x) * bytes_per_pixel;
  char *pixel_addr = video_mem + pixel_offset;

  // Copy the color value to the video memory
  if (memcpy(pixel_addr, &color, bytes_per_pixel) == NULL)
    return 1;

  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  // Check if the starting point is valid and if the line fits within the screen
  // boundaries
  if (x < 0 || y < 0)
    return 1;
  if (x + len > vmi.XResolution || y >= vmi.YResolution)
    return 1;

  // Draw a horizontal line by drawing individual pixels
  for (uint16_t i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color))
      return 1;
  }

  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                       uint32_t color) {
  // Check if the starting point is valid and if the rectangle fits within the
  // screen boundaries
  if (x < 0 || y < 0)
    return 1;
  if (x + width > vmi.XResolution || y + height > vmi.YResolution)
    return 1;

  // Draw a rectangle by drawing horizontal lines
  for (uint16_t j = 0; j < height; j++) {
    if (vg_draw_hline(x, y + j, width, color))
      return 1;
  }

  return 0;
}

int(vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y,
                 enum xpm_image_type type) {
  // Check if the starting point is valid and if the rectangle fits within the
  // screen boundaries
  if (x < 0 || y < 0)
    return 1;

  xpm_image_t img;

  uint8_t *img_bytes;
  if ((img_bytes = xpm_load(xpm, type, &img)) == NULL)
    return 1;
  if (x + img.width > vmi.XResolution || y + img.height > vmi.YResolution)
    return 1;

  /*
  for (uint16_t j = 0; j < img.height; j++) {
      for (uint16_t i = 0; i < img.width; i++) {
          if(type == XPM_INDEXED) {
              uint8_t color = img_bytes[j * img.width + i];
              if (vg_draw_pixel(x + i, y + j, color)) return 1;
          }
          else if(type == XPM_1_5_5_5 || type == XPM_5_6_5) {
              uint16_t color = ((uint16_t*)img_bytes)[j * img.width + i];
              if (vg_draw_pixel(x + i, y + j, color)) return 1;
          }
          else {
              uint32_t color = ((uint32_t*)img_bytes)[j * img.width + i];
              if (vg_draw_pixel(x + i, y + j, color)) return 1;
          }
      }
  }
  */

  for (uint16_t j = 0; j < img.height; j++) {
    for (uint16_t i = 0; i < img.width; i++) {
      uint8_t *color = img_bytes + ((j * img.width + i) * bytes_per_pixel);
      if (vg_draw_pixel(x + i, y + j, *(uint32_t *)color))
        return 1;
    }
  }

  return 0;
}

static int video_set_display_start(uint16_t x, uint16_t y) {
  reg86_t reg;
  memset(&reg, 0, sizeof(reg));

  reg.intno = VBE_INT;
  reg.ah = AH_VBE;
  reg.al = VBE_SET_DISPLAY_START;
  reg.bx = 0x00;
  reg.cx = x;
  reg.dx = y;

  if (sys_int86(&reg) != OK) {
    printf("[video_gr] sys_int86 (VBE Set Display Start) failed\n");
    return 1;
  }
  return 0;
}

int video_init_page_flipping(uint16_t mode) {
  memset(&vmi, 0, sizeof(vmi));
  if (vbe_get_mode_info(mode, &vmi) != OK) {
    printf("[video_gr] vbe_get_mode_info failed\n");
    return 1;
  }

  bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8;

  page_size = vmi.XResolution * vmi.YResolution * bytes_per_pixel;
  unsigned vram_size = page_size * 2;

  int r;
  struct minix_mem_range mr;
  mr.mr_base = (phys_bytes)vmi.PhysBasePtr;
  mr.mr_limit = mr.mr_base + vram_size;

  if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK) {
    printf("[video_gr] sys_privctl (ADD_MEM) failed: %d\n", r);
    return 1;
  }

  vram_ptr0 = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if (vram_ptr0 == MAP_FAILED) {
    printf("[video_gr] vm_map_phys failed to map VRAM memory\n");
    return 1;
  }

  vram_ptr1 = vram_ptr0 + page_size;
  active_page = 1;
  video_mem = vram_ptr1;

  if (set_video_mode(mode) != 0) {
    printf("[video_gr] set_video_mode failed\n");
    return 1;
  }

  if (video_set_display_start(0, 0) != 0) {
    printf("[video_gr] failed to configure initial display page\n");
    return 1;
  }

  memset(vram_ptr0, 0, vram_size);

  return 0;
}

int video_clear_page(void) {
  if (video_mem == NULL) {
    return 1;
  }
  memset(video_mem, 0, page_size);
  return 0;
}

int video_swap_pages(void) {
  if (video_mem == NULL) {
    return 1;
  }

  uint16_t y_offset = active_page * vmi.YResolution;
  if (video_set_display_start(0, y_offset) != 0) {
    return 1;
  }

  active_page = 1 - active_page;
  video_mem = (active_page == 0) ? vram_ptr0 : vram_ptr1;
  return 0;
}

int video_cleanup_page_flipping(void) {
  video_mem = NULL;
  vram_ptr0 = NULL;
  vram_ptr1 = NULL;
  page_size = 0;
  active_page = 1;
  memset(&vmi, 0, sizeof(vmi));
  bytes_per_pixel = 0;
  return 0;
}

int video_draw_background(const uint8_t *background_pixmap) {
  if (video_mem == NULL || background_pixmap == NULL) {
    return 1;
  }

  if (bytes_per_pixel == 3) {
    memcpy(video_mem, background_pixmap, page_size);
  } else if (bytes_per_pixel == 4) {
    for (uint32_t i = 0; i < vmi.XResolution * vmi.YResolution; i++) {
      uint32_t xpm_offset = i * 3;
      uint32_t vram_offset = i * 4;

      uint32_t color = background_pixmap[xpm_offset] |
                       (background_pixmap[xpm_offset + 1] << 8) |
                       (background_pixmap[xpm_offset + 2] << 16);

      memcpy(video_mem + vram_offset, &color, 4);
    }
  } else {
    return 1;
  }
  return 0;
}

int video_draw_sprite(const uint8_t *pixmap, uint16_t width, uint16_t height,
                      uint16_t x, uint16_t y) {
  if (video_mem == NULL || pixmap == NULL)
    return 1;
  if (x >= vmi.XResolution || y >= vmi.YResolution)
    return 0;

  uint16_t draw_width =
      (x + width > vmi.XResolution) ? (vmi.XResolution - x) : width;
  uint16_t draw_height =
      (y + height > vmi.YResolution) ? (vmi.YResolution - y) : height;

  for (uint16_t row = 0; row < draw_height; row++) {
    uint8_t *dst = (uint8_t *)video_mem +
                   ((y + row) * vmi.XResolution + x) * bytes_per_pixel;
    const uint8_t *src = pixmap + (row * width) * 3;

    for (uint16_t col = 0; col < draw_width; col++) {
      uint32_t color = src[0] | (src[1] << 8) | (src[2] << 16);

      if ((color & 0x00FFFFFF) != CHROMA_KEY_GREEN_888) {
        if (bytes_per_pixel == 3) {
          dst[0] = src[0];
          dst[1] = src[1];
          dst[2] = src[2];
        } else {
          *(uint32_t *)dst = color;
        }
      }
      dst += bytes_per_pixel;
      src += 3;
    }
  }
  return 0;
}

int video_draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                    uint32_t color) {
  if (video_mem == NULL)
    return 1;
  if (x >= vmi.XResolution || y >= vmi.YResolution)
    return 0;

  /* clip to screen bounds */
  if (x + width > vmi.XResolution)
    width = vmi.XResolution - x;
  if (y + height > vmi.YResolution)
    height = vmi.YResolution - y;

  for (uint16_t row = 0; row < height; row++) {
    uint8_t *dst = (uint8_t *)video_mem +
                   ((y + row) * vmi.XResolution + x) * bytes_per_pixel;
    for (uint16_t col = 0; col < width; col++) {
      memcpy(dst, &color, bytes_per_pixel);
      dst += bytes_per_pixel;
    }
  }
  return 0;
}
