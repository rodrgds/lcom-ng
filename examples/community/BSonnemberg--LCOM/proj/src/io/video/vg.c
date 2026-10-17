#include "vg.h"
#include "vbe.h"

#include <math.h>

static uint32_t hres;
static uint32_t vres;
static int bytes_per_pixel;

vbe_mode_info_t mode_info;

void *(vg_init)(uint16_t mode) {

  struct minix_mem_range mr;
  unsigned int vram_base;
  unsigned int vram_size;

  // get mode info

  memset(&mode_info, 0, sizeof(mode_info));

  if (vbe_get_mode_info(mode, &mode_info) != 0) {
    printf("failed to get vbe mode info, invalid mode?\n");
    return NULL;
  }

  hres = mode_info.XResolution;
  vres = mode_info.YResolution;
  bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;

  vram_base = mode_info.PhysBasePtr;
  vram_size = hres * vres * bytes_per_pixel;
  vsize = vram_size;

  // map to virtual address space

  mr.mr_base = vram_base;
  mr.mr_limit = vram_base + 2 * vram_size;

  int r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr);
  if (r != OK) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return NULL;
  }

  video_front = (uint8_t*) vm_map_phys(SELF, (void*)mr.mr_base, 2*vram_size);
  video_back = video_front + vram_size;

  if(video_front == MAP_FAILED) {
    panic("couldn't map video memory");
    return NULL;
  }

  // set graphics mode
  if (vbe_set_mode(mode) != 0) {
    return NULL;
  }
  
  return video_front;
}

int vg_set_pixel(uint16_t x, uint16_t y, uint32_t color) {

  if (video_back == NULL) {
    return 1;
  }
  if (x >= hres || y >= vres) {
    return 1;
  }
  if (color > 63) {
    // invalid color/ transparent
    return 1;
  }

  uint8_t *ptr = video_back + (x + y * hres) * bytes_per_pixel;

  if (memset(ptr, color, bytes_per_pixel) == NULL) {
    return 2;
  }

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++) {
    vg_set_pixel(x+i, y, color);
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height; i++) {
    vg_draw_hline(x, y+i, width, color);
  }
  return 0;
}

int vg_draw_sprite(sprite *sp, uint16_t x, uint16_t y) {
  uint8_t *colors = (uint8_t*) sp->map;
  for (int i = 0; i < sp->height; i++) {
    for (int j = 0; j < sp->width; j++) {
      vg_set_pixel(x+j, y+i, *colors);
      colors++;
    }
  }
  return 0;
}

int vg_draw_sprite_rotated(sprite *sp, uint16_t x, uint16_t y, double angle, uint8_t padding) {
  uint8_t *colors = (uint8_t*) sp->map;

  double sin_a = sin(angle);
  double cos_a = cos(angle);

  uint16_t pivot_x = sp->width / 2;
  uint16_t pivot_y = 0;

  for (int i = 0; i < sp->height; i++) {
      for (int j = 0; j < sp->width; j++) {
            
          int dx = j - pivot_x;
          int dy = i - pivot_y;

          // rotate
          int rotated_x = (int)(dx * cos_a - dy * sin_a);
          int rotated_y = (int)(dx * sin_a + dy * cos_a);

            // Translate to the position of the ball (pivot point)
          int screen_x = x + rotated_x - (20 + padding) * sin_a;
          int screen_y = y + rotated_y + (20 + padding) * cos_a;

          if (screen_x >= 0 && screen_y >= 0) {
              vg_set_pixel(screen_x, screen_y, *colors);
          }

          colors++;
        }
    }
  return 0;
}

int vg_flip_buffer() {
  // this is somehow faster than using
  // vbe's built in function :/
  return memcpy(video_front, video_back, vsize) != NULL;
}

uint32_t vg_get_hres() {
  return hres;
}

uint32_t vg_get_vres() {
  return vres;
}
