#include <lcom/lcf.h>

#include "graphics.h"
#include "vbe.h"

#include <math.h>

vbe_mode_info_t* vmi_p;
char* video_mem;
int indexed_color_mode;
int frame_counter;

int (map_vram)(uint16_t mode) {
  int r;
  struct minix_mem_range mr;
  
  vmi_p = (vbe_mode_info_t*) malloc(sizeof(vbe_mode_info_t));
  if (vbe_get_mode_info(mode, vmi_p) != 0)
    return 1;
  if (mode == 0x105)
    indexed_color_mode = 1;
  else
    indexed_color_mode = 0;

  unsigned int vram_base = vmi_p->PhysBasePtr;
  unsigned int vram_size = vmi_p->XResolution * vmi_p->YResolution * ceil(vmi_p->BitsPerPixel / (double) 8); /* VRAM’s size, but you can use the frame-buffer size, instead */

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED)
    panic("couldn’t map video memory");
  return 0;
}

int (initialize_graphics_mode)(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.ah = AH;
  r.al = VBE_FUNCTION;
  r.bx = BIT(14)|mode;
  r.intno = INT_NO;
  if (sys_int86(&r) != OK)
    return 1;
  return 0;
}

int (graphics_init)(uint16_t mode) {
  if (map_vram(mode) != 0)
    return 1;
  if (initialize_graphics_mode(mode) != 0)
    return 1;
  return 0;
}

int (verify_screen_limits)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  if (x >= vmi_p->XResolution) {
    vg_exit();
    printf("X is out of range\n");
    return 1;
  }
  if (y >= vmi_p->YResolution) {
    vg_exit();
    printf("Y is out of range\n");
    return 1;
  }
  if (x + width >= vmi_p->XResolution) {
    vg_exit();
    printf("Width is out of range\n");
    return 1;
  }
  if (y + height >= vmi_p->YResolution) {
    vg_exit();
    printf("Height is out of range\n");
    return 1;
  }
  return 0;
}

void (draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  size_t num_bytes = ceil(vmi_p->BitsPerPixel / (double) 8);
  char* address = (char*) video_mem;
  address += (y*vmi_p->XResolution+x)*num_bytes;
  memcpy(address, &color, num_bytes);
}

void (draw_hline)(uint16_t x, uint16_t y, uint16_t width, uint32_t color) {
  for (int i = x; i < x + width; i++)
    draw_pixel(i, y, color);
}

void (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = y; i < y + height; i++)
    draw_hline(x, i, width, color);
}

uint8_t* (initialize_pixmap)(xpm_map_t xpm, xpm_image_t* img_info) {
  uint8_t* pixmap = xpm_load(xpm, XPM_INDEXED, img_info);
  return pixmap;
}

void (draw_xpm)(uint8_t* pixmap, xpm_image_t img_info, uint16_t x, uint16_t y) {
  for (int i = y; i < y + img_info.height; i++) {
    for (int j = x; j < x + img_info.width; j++) {
      draw_pixel(j, i, *pixmap);
      pixmap++;
    }
  }
}

void (move_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint16_t* x, uint16_t* y, int16_t speed) {
  if (speed > 0) {
    if ((xi < xf && *x < xf) || (xi > xf && *x > xf) || (yi < yf && *y < yf) || (yi > yf && *y > yf)) {
      draw_rectangle(0, 0, vmi_p->XResolution, vmi_p->YResolution, BACKGROUND);
      //draw_xpm(*x, *y);
    }
    if (xi < xf && *x < xf)
      *x += speed;
    if (xi > xf && *x > xf)
      *x -= speed;
    if (yi < yf && *y < yf)
      *y += speed;
    if (yi > yf && *y > yf)
      *y -= speed;
  }

  else {
    if (frame_counter == abs(speed)) {
      frame_counter = 0;
      draw_rectangle(0, 0, vmi_p->XResolution, vmi_p->YResolution, BACKGROUND);
      //draw_xpm(*x, *y);

      if (xi < xf && *x < xf)
        *x += 1;
      if (xi > xf && *x > xf)
        *x -= 1;
      if (yi < yf && *y < yf)
        *y += 1;
      if (yi > yf && *y > yf)
        *y -= 1;
    }
    frame_counter++;
  }
}
