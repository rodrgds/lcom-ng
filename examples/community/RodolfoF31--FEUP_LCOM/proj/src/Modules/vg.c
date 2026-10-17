#include "vg.h"

static vbe_mode_info_t modeInfo;
static void *video_mem;
static mmap_t mem_map;
static void *video_buffer;
static unsigned int vram_size;

uint8_t get_red_field_position(void) {
  return modeInfo.RedFieldPosition;
}

uint8_t get_green_field_position(void) {
  return modeInfo.GreenFieldPosition;
}

uint8_t get_blue_field_position(void) {
  return modeInfo.BlueFieldPosition;
}

uint16_t get_red_mask_size(void) {
  return modeInfo.RedMaskSize;
}

uint16_t get_green_mask_size(void) {
  return modeInfo.GreenMaskSize;
}

uint16_t get_blue_mask_size(void) {
  return modeInfo.BlueMaskSize;
}

uint16_t get_horizontal_resolution(void) {
  return modeInfo.XResolution;
}

uint16_t get_vertical_resolution(void) {
  return modeInfo.YResolution;
}

unsigned get_bits_per_pixel(void) {
  return modeInfo.BitsPerPixel;
}

int free_map_memory(void) {
  lm_free(&mem_map);
  return 0;
}

uint8_t *(read_pixmap) (xpm_row_t * pixmap, uint16_t *width, uint16_t *height) {
  if (width == NULL || height == NULL) {
    printf("null pointer\n");
    return NULL;
  }

  enum xpm_image_type type = XPM_INDEXED;
  xpm_image_t img;
  uint8_t *sprite = xpm_load((xpm_map_t) pixmap, type, &img);

  if (sprite == NULL) {
    printf("null pointer\n");
    return NULL;
  }

  *width = img.width;
  *height = img.height;

  return sprite;
}

int(draw_sprite)(uint8_t *sprite, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {

  for (uint16_t i = 0; i < width; i++)
    for (uint16_t j = 0; j < height; j++)
      if (draw_pixel(x + i, y + j, sprite[j * width + i]) != 0)
        return 1;

  return 0;
}

int(draw_element)(xpm_row_t *pixmap, uint16_t x, uint16_t y) {
  uint16_t width = 0, height = 0;
  uint8_t *sprite = read_pixmap(pixmap, &width, &height);
  if (sprite == NULL)
    return 1;

  if (draw_sprite(sprite, x, y, width, height))
    return 1;

  return 0;
}

int draw_element_scaled(xpm_row_t *pixmap, uint16_t x, uint16_t y, float scale) {
  uint16_t src_w = 0, src_h = 0;
  uint8_t *sprite = read_pixmap(pixmap, &src_w, &src_h);
  if (sprite == NULL)
    return 1;

  uint16_t dst_w = src_w * scale;
  uint16_t dst_h = src_h * scale;

  for (uint16_t j = 0; j < dst_h; j++) {
    for (uint16_t i = 0; i < dst_w; i++) {

      uint16_t src_x = i / scale;
      uint16_t src_y = j / scale;
      uint8_t color = sprite[src_y * src_w + src_x];

      draw_pixel(x + i, y + j, color);
    }
  }
  return 0;
}

void *(vg_init) (uint16_t mode) {
  struct minix_mem_range mr;
  unsigned int vram_base;
  int r;

  if (vbe_get_mode_info(mode, &modeInfo) != 0) {
    printf("CANNOT READ VBE MODE INFO\n");
    return NULL;
  }

  vram_base = modeInfo.PhysBasePtr;
  vram_size = get_horizontal_resolution() * get_vertical_resolution() * (get_bits_per_pixel() / 8);

  if (get_bits_per_pixel() % 8 != 0) {
    vram_size += 1;
  }

  lm_alloc(vram_size, &mem_map);

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  }

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED) {
    panic("couldn't map video memory");
  }

  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86));

  reg86.ax = 0x4F02;
  reg86.intno = 0x10;
  reg86.bx = 1 << 14 | mode;
  if (sys_int86(&reg86) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return NULL;
  }

  video_buffer = malloc(vram_size);
  if (video_buffer == NULL) {
    printf("Failed to allocate video buffer\n");
    return NULL;
  }
  // Optionally clear the buffer (black screen)
  memset(video_buffer, 0, vram_size);

  return video_mem;
}

int draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
  uint32_t pos;
  uint8_t number_bytes;

  if (x > get_horizontal_resolution() || y > get_vertical_resolution()) {
    return 1;
  }

  switch (get_bits_per_pixel()) {
    case 8:
      number_bytes = 1;
      if (get_bits_per_pixel() % 8 != 0) {
        number_bytes = 2;
      }
      pos = get_horizontal_resolution() * y * number_bytes + x * number_bytes;
      memcpy((uint8_t *) video_buffer + pos, &color, number_bytes);
      break;

    case 24:
      number_bytes = 3;
      if (get_bits_per_pixel() % 8 != 0) {
        number_bytes = 4;
      }
      pos = get_horizontal_resolution() * y * number_bytes + x * number_bytes;
      memcpy((uint8_t *) video_buffer + pos, &color, number_bytes);
      break;
  }


  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++) {
    draw_pixel(x + i, y, color);
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height; i++) {
    vg_draw_hline(x, y + i, width, color);
  }
  return 0;
}

void vg_draw_content() {
  memcpy(video_mem, video_buffer, vram_size);
}

int clearScreen() {
  vg_draw_rectangle(0, 0, get_horizontal_resolution(), get_vertical_resolution(), 0x000000);
  return 0;
}
