#include "graphics.h"
#include "vbe.h"
#include <lcom/lcf.h>

static uint8_t *video_mem;       // base pointer to the full mapped VRAM (2 pages)
static vbe_mode_info_t vmi;      // VBE mode info
static unsigned bytes_per_pixel; // bytes per pixel
static unsigned page_size;       // bytes in one page = XRes * YRes * bpp
static uint8_t draw_page = 0;    // page currently being drawn into (0 or 1)

int (set_graphic_mode)(uint16_t mode) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.intno = BIOS_VIDEO_SERV;
  r86.ah    = VBE_CALL;
  r86.al    = VBE_SET_MODE;
  r86.bx    = mode | VBE_LINEAR_FB;

  if (sys_int86(&r86) != OK) return 1;
  return 0;
}

int (video_exit)(void) {
  return vg_exit();
}

int (map_vram)(uint16_t mode) {
  int r;
  struct minix_mem_range mr;

  if (vbe_get_mode_info(mode, &vmi) != 0) return 1;

  bytes_per_pixel = (vmi.BitsPerPixel + 7) / 8;
  page_size = vmi.XResolution * vmi.YResolution * bytes_per_pixel;

  // Map two pages of VRAM for page flipping
  unsigned int vram_base = vmi.PhysBasePtr;
  unsigned int vram_size = page_size * 2;

  mr.mr_base  = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) return 1;

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED) return 1;

  // Start drawing into page 0 (displayed by default)
  draw_page = 0;
  return 0;
}

int (flip_page)(void) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.intno = BIOS_VIDEO_SERV;
  r86.ah    = VBE_CALL;
  r86.al    = VBE_SET_START;
  r86.bh    = 0;
  r86.bl    = VBE_START_VSYNC;
  r86.cx    = 0;
  r86.dx    = (uint16_t)(draw_page * vmi.YResolution);

  if (sys_int86(&r86) != OK) return 1;

  draw_page = (draw_page + 1) % 2;
  return 0;
}

uint8_t *(video_get_draw_buffer)(void) {
  return video_mem + draw_page * page_size;
}

uint16_t (get_h_res)(void)            { return vmi.XResolution; }
uint16_t (get_v_res)(void)            { return vmi.YResolution; }
unsigned (get_bytes_per_pixel)(void)  { return bytes_per_pixel; }
unsigned (get_bits_per_pixel)(void)   { return vmi.BitsPerPixel; }
unsigned (get_bytes_per_scanline)(void) { return vmi.BytesPerScanLine; }

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= vmi.XResolution || y >= vmi.YResolution) return 1;

  uint8_t *buf = video_get_draw_buffer();
  unsigned int idx = (y * vmi.BytesPerScanLine) + (x * bytes_per_pixel);

  memcpy(&buf[idx], &color, bytes_per_pixel);
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (uint16_t i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color) != 0) return 1;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color) != 0) return 1;
  }
  return 0;
}

// load XPMs once at startup, not inside the game loop
int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t img;
  uint8_t *map = xpm_load(xpm, XPM_INDEXED, &img);
  if (map == NULL) return 1;

  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      uint8_t color = map[i * img.width + j];
      vg_draw_pixel((uint16_t)(x + j), (uint16_t)(y + i), color);
    }
  }
  return 0;
}
