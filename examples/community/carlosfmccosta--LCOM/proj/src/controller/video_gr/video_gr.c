/**
 * @file video_gr.c
 * @brief Implementation of video graphics operations.
 *
 * This file contains the implementation of the functions declared in video_gr.h.
 */
#include "video_gr.h"
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/vbe.h>
#include <machine/int86.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

/* Static variables */
static void *video_mem;      /* Address to which VRAM is mapped */
static uint8_t *back_buffer; /* Pointer to the secondary buffer */
static unsigned hres;        /* Frame horizontal resolution */
static unsigned vres;        /* Frame vertical resolution */
static unsigned bytes_per_pixel;
static unsigned bits_per_pixel;
static size_t vram_size; /* Size of VRAM (and back buffer) */
enum color_mode c_mode;

static uint8_t redMaskSize;
static uint8_t redFieldPosition;
static uint8_t greenMaskSize;
static uint8_t greenFieldPosition;
static uint8_t blueMaskSize;
static uint8_t blueFieldPosition;

void *(vg_init) (uint16_t mode) {
  vbe_mode_info_t vmi_p;
  if (vbe_get_mode_info(mode, &vmi_p) != 0) {
    printf("vg_init: vbe_get_mode_info failed for mode 0x%X\n", mode);
    return NULL;
  }

  // Store mode information
  hres = vmi_p.XResolution;
  vres = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel + 7) / 8;
  vram_size = hres * vres * bytes_per_pixel;

  // Store color information
  switch (bytes_per_pixel) {
    case 1:
      c_mode = INDEXED;
      break;
    default:
      c_mode = DIRECT;
      redMaskSize = vmi_p.RedMaskSize;
      redFieldPosition = vmi_p.RedFieldPosition;
      greenMaskSize = vmi_p.GreenMaskSize;
      greenFieldPosition = vmi_p.GreenFieldPosition;
      blueMaskSize = vmi_p.BlueMaskSize;
      blueFieldPosition = vmi_p.BlueFieldPosition;
      break;
  }

  // Map VRAM
  struct minix_mem_range mr;
  mr.mr_base = (phys_bytes) vmi_p.PhysBasePtr;
  mr.mr_limit = mr.mr_base + vram_size;

  int r;
  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    panic("vg_init: sys_privctl (ADD_MEM) failed: %d\n", r);
  }
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED) {
    panic("vg_init: Couldn't map video memory");
  }

  // Allocate Back Buffer
  back_buffer = (uint8_t *) malloc(vram_size);
  if (back_buffer == NULL) {
    vm_unmap_phys(SELF, video_mem, vram_size); // Clean up VRAM mapping
    printf("Error: Failed to allocate back buffer\n");
    return NULL;
  }
  memset(back_buffer, 0, vram_size); // Initialize back buffer (e.g., to black)

  // Set Video Mode using BIOS call
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));
  r86.intno = 0x10;
  r86.ax = 0x4F02;
  r86.bx = (1 << 14) | mode;
  if (sys_int86(&r86) != OK) {
    free(back_buffer);                         // Clean up back buffer
    vm_unmap_phys(SELF, video_mem, vram_size); // Clean up VRAM mapping
    printf("vg_init: sys_int86() failed to set mode 0x%X\n", mode);
    return NULL;
  }
  if (r86.al != 0x4F || r86.ah != 0x00) {
    free(back_buffer);
    vm_unmap_phys(SELF, video_mem, vram_size);
    printf("vg_init: VBE set mode failed (AL=0x%X, AH=0x%X)\n", r86.al, r86.ah);
    return NULL;
  }

  printf("Graphics mode 0x%X set (%ux%u, %u bpp)\n", mode, hres, vres, bits_per_pixel);
  return video_mem; // Return pointer to VRAM (though drawing is to back_buffer)
}

// Draws a pixel on the BACK BUFFER
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (back_buffer == NULL)
    return 1;
  if (x >= hres || y >= vres)
    return 1; // Out of bounds (or return 0 to just ignore)

  uint8_t *pixel_ptr = back_buffer + (y * hres + x) * bytes_per_pixel;
  memcpy(pixel_ptr, &color, bytes_per_pixel);

  return 0;
}

// Draws a horizontal line on the BACK BUFFER
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (back_buffer == NULL)
    return 1;
  if (y >= vres || x >= hres)
    return 1; // Check start point is valid

  // Clamp length to screen bounds
  if (x + len > hres) {
    if (hres <= x)
      return 0; // Started off-screen horizontally
    len = hres - x;
  }

  uint8_t *line_ptr = back_buffer + (y * hres + x) * bytes_per_pixel;
  for (uint16_t i = 0; i < len; ++i) {
    memcpy(line_ptr, &color, bytes_per_pixel);
    line_ptr += bytes_per_pixel;
  }
  return 0;
}

// Draws a filled rectangle on the BACK BUFFER
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (back_buffer == NULL)
    return 1;
  if (x >= hres || y >= vres)
    return 0;

  uint16_t y_end = y + height;
  if (y_end > vres) {
    if (vres <= y)
      return 0;
    height = vres - y;
  }

  for (uint16_t row = y; row < y + height; ++row) {
    vg_draw_hline(x, row, width, color);
  }
  return 0;
}

// Fills the ENTIRE BACK BUFFER with a color
int vg_fill(uint32_t color) {
  if (back_buffer == NULL)
    return 1;

  if (bytes_per_pixel == 1) {
    memset(back_buffer, (uint8_t) color, vram_size);
  }
  else if (bytes_per_pixel == 2 && (color & 0xFF) == ((color >> 8) & 0xFF)) {
    memset(back_buffer, (uint8_t) color, vram_size);
  }
  else {
    // Generic pixel-by-pixel fill
    uint8_t *ptr = back_buffer;
    size_t total_pixels = hres * vres;
    for (size_t i = 0; i < total_pixels; ++i) {
      memcpy(ptr, &color, bytes_per_pixel);
      ptr += bytes_per_pixel;
    }
  }
  return 0;
}

// Copies the BACK BUFFER to VRAM
void vg_copy_buffer() {
  if (video_mem != NULL && back_buffer != NULL) {
    memcpy(video_mem, back_buffer, vram_size);
  }
}

// Draws an XPM on the BACK BUFFER
int draw_xpm(xpm_image_t img, uint16_t x, uint16_t y, bool flip) {
  if (img.bytes == NULL || img.width == 0 || img.height == 0 || back_buffer == NULL)
    return 1;

  for (uint16_t row = 0; row < img.height; ++row) {
    for (uint16_t col = 0; col < img.width; ++col) {
      uint16_t target_x = x + col;
      uint16_t target_y = y + row;

      // Calculate source column
      uint16_t src_col = flip ? (img.width - 1 - col) : col;

      if (target_x < hres && target_y < vres) {
        size_t index = (row * img.width + src_col) * bytes_per_pixel;
        uint32_t color = 0;

        if (index + bytes_per_pixel <= img.size) {
          memcpy(&color, img.bytes + index, bytes_per_pixel);
        }
        else {
          continue;
        }

        if (color == 0) { // Skip black
          continue;
        }

        //Para este projeto, não damos erro se o pixel estiver fora dos limites
        //Visto que certos sprites podem ter pixels fora dos limites.
        //Isto é propositado e devidamente controlado pelo resto do código.
        vg_draw_pixel(target_x, target_y, color);
      }
    }
  }
  return 0;
}

unsigned int get_hres() { return hres; }
unsigned int get_vres() { return vres; }
unsigned int get_bytes_per_pixel() { return bytes_per_pixel; }

uint8_t *get_back_buffer() {
  return back_buffer;
}

size_t get_vram_size() {
  return vram_size;
}
