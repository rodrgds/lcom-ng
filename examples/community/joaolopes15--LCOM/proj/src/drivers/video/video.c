#include "video.h"
#include <lcom/lcf.h>

void *video_mem[3];
int currentDrawBuffer = 0;
int currentDisplayBuffer = 1;
int readyBuffer = 2;

// function that sets video mode using bios calls
int(set_mode)(uint16_t mode) {
  reg86_t reg86;

  memset(&reg86, 0, sizeof(reg86)); // initialize all fields to zero

  reg86.intno = 0x10;
  reg86.ah = 0x4F;
  reg86.al = 0x02;
  reg86.bx = mode | BIT(14);

  if (sys_int86(&reg86) != 0) {
    return 1;
  }

  return 0;
}

// function that flips the buffers using bios calls, which displays the ready buffer and then rotates to the next buffers for the triple buffer implementation
int(flip)() {
  reg86_t reg86;

  memset(&reg86, 0, sizeof(reg86)); // initialize all fields to zero

  reg86.intno = 0x10;
  reg86.ah = 0x4F;
  reg86.al = 0x07;
  reg86.dx = readyBuffer * vmi_p.YResolution;

  if (sys_int86(&reg86) != 0) {
    return 1;
  }

  int temp = currentDisplayBuffer;
  currentDisplayBuffer = readyBuffer;
  readyBuffer = currentDrawBuffer;
  currentDrawBuffer = temp;

  return 0;
}

// initializes minix in video mode with triple buffering. it first gets the mode info, then maps the physical memory to virtual memory for virtual ram access, sets up the triple buffers, sets the video mode and clears all the buffers.
void *(vg_init) (uint16_t mode) {
  struct minix_mem_range mr;
  unsigned int vram_base;
  unsigned int vram_size;
  int r;

  if (vbe_get_mode_info(mode, &vmi_p) != 0) {
    return NULL;
  }

  vram_base = vmi_p.PhysBasePtr;
  vram_size = vmi_p.XResolution * vmi_p.YResolution * ((vmi_p.BitsPerPixel + 7) / 8);

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + 3 * vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    return NULL;
  }

  video_mem[0] = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  video_mem[1] = vm_map_phys(SELF, (void *) (mr.mr_base + vram_size), vram_size);
  video_mem[2] = vm_map_phys(SELF, (void *) (mr.mr_base + 2 * vram_size), vram_size);

  if (video_mem[0] == MAP_FAILED || video_mem[1] == MAP_FAILED || video_mem[2] == MAP_FAILED) {
    return NULL;
  }

  if (set_mode(mode) != 0) {
    return NULL;
  }

  size_t buffer_size = vram_size;
  memset(video_mem[0], 0, buffer_size);
  memset(video_mem[1], 0, buffer_size);
  memset(video_mem[2], 0, buffer_size);

  return video_mem[currentDrawBuffer];
}

// function that draws a pixel on a certain position on the screen
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= vmi_p.XResolution || y >= vmi_p.YResolution) {
    return 1;
  }

  unsigned int index = (y * vmi_p.XResolution + x);

  if (vmi_p.BitsPerPixel == 8) {
    uint8_t *video_ptr = (uint8_t *) video_mem[currentDrawBuffer];
    video_ptr[index] = (uint8_t) (color & 0xFF);
  }
  else if (vmi_p.BitsPerPixel == 15 || vmi_p.BitsPerPixel == 16) {
    uint16_t *video_ptr = (uint16_t *) video_mem[currentDrawBuffer];
    video_ptr[index] = (uint16_t) (color & 0xFFFF);
  }
  else if (vmi_p.BitsPerPixel == 24) {
    uint8_t *video_ptr = (uint8_t *) video_mem[currentDrawBuffer];
    index *= 3;
    video_ptr[index] = color & 0xFF;
    video_ptr[index + 1] = (color >> 8) & 0xFF;
    video_ptr[index + 2] = (color >> 16) & 0xFF;
  }
  else if (vmi_p.BitsPerPixel == 32) {
    uint32_t *video_ptr = (uint32_t *) video_mem[currentDrawBuffer];
    video_ptr[index] = color;
  }

  return 0;
}

// draws a line on the screen using the draw_pixel function
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color) != 0) {
      return 1;
    }
  }

  return 0;
}

// draws a rectangle on the screen using the draw_hline function
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (unsigned i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color) != 0) {
      return 1;
    }
  }

  return 0;
}

// draws a XPM image on the screen into memory, by processing each pixel. transparent pixels are skipped
int(draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t img;
  uint8_t *map = xpm_load(xpm, XPM_8_8_8_8, &img);

  if (map == NULL) {
    return 1;
  }

  for (unsigned i = 0; i < img.height; i++) {
    for (unsigned j = 0; j < img.width; j++) {
      unsigned pixel_index = (i * img.width + j) * 4;

      uint8_t red = map[pixel_index];
      uint8_t green = map[pixel_index + 1];
      uint8_t blue = map[pixel_index + 2];
      uint8_t alpha = map[pixel_index + 3];

      uint32_t color = (alpha << 24) | (blue << 16) | (green << 8) | red;

      if (color == xpm_transparency_color(XPM_8_8_8_8)) {
        continue;
      }

      if (vg_draw_pixel(x + j, y + i, color) != 0) {
        free(map);
        return 1;
      }
    }
  }

  free(map);
  return 0;
}

// clears the screen by filling up the video memory with zeros
int(clear_screen)() {
  if (memset(video_mem[currentDrawBuffer], 0, vmi_p.XResolution * vmi_p.YResolution * ((vmi_p.BitsPerPixel + 7) / 8)) == NULL) {
    return 1;
  }

  return 0;
}
