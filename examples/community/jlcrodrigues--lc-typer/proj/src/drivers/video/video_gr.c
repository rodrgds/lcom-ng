#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "video.h"

static char *video_mem; /* Process (virtual) address to which VRAM is mapped */
static char *buff; /* Used for double buffering */
static vbe_mode_info_t mode_info;
static mmap_t mem_map;

static unsigned h_res;          /* Horizontal resolution in pixels */
static unsigned v_res;          /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned bytes_per_pixel; /* Number of VRAM bytes per pixel */
static unsigned int vram_size; /* Size of a frame in memory */
static const uint32_t bg_color = BG_COLOR;

int(map_memory)(vbe_mode_info_t mode_info) {
  struct minix_mem_range mr;
  int r;

  /* Use VBE function 0x01 to initialize vram_base and vram_size */
  unsigned int vram_base = mode_info.PhysBasePtr;
  vram_size = bytes_per_pixel * h_res * v_res;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + 2 * vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, 2 * vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  //buff = (char *) (video_mem + vram_size);
  buff = video_mem;
  buff += vram_size;
  memset(video_mem, 0, 2 * vram_size);

  return 0;
}

int (vbe_get_info_from_mode)(uint16_t mode){
    reg86_t r86;
    memset(&r86, 0, sizeof(reg86_t));
    memset(&mode_info,0, sizeof(vbe_mode_info_t));  /* clear the info of the mode*/

    vbe_mode_info_t *mode_info_address = lm_alloc(sizeof(vbe_mode_info_t),&mem_map);
    r86.intno = 0x10; /* BIOS video services */
    r86.ah = 0x4F;    /* Set Video Mode function */
    r86.al = 0x01;    /* set video mode  */
    r86.cx = mode | BIT(14);
    r86.es = PB2BASE(mem_map.phys);
    r86.di = PB2OFF(mem_map.phys);
    if(sys_int86(&r86)!= OK || r86.ah != 0X00){
        printf("\tvg_init(): sys_int86() failed \n");
        if (!lm_free(&mem_map)){
            printf("\tvg_init(): lm_free failed \n");
        }
        return 1;
    }
    memcpy((void*)&mode_info,(void*)mode_info_address,mem_map.size);
    return 0;
}

void *(set_mode) (uint16_t mode) {
  reg86_t r86;
  /* Specify the appropriate register values */
  memset(&r86, 0, sizeof(r86)); /* zero the structure */

  r86.intno = 0x10; /* BIOS video services */
  r86.ah = 0x4F;    /* Set Video Mode function */
  r86.al = 0x02;    /* set video mode  */
  r86.bx = mode | BIT(14);

  /* Make the BIOS call */

  if (sys_int86(&r86) != OK) {
    printf("\tvg_init(): sys_int86() failed \n");
    video_mem = NULL;
  }
  return video_mem;
}

void *(vg_init) (uint16_t mode) {
  if (vbe_get_info_from_mode(mode))
    return NULL;

  h_res = mode_info.XResolution;
  v_res = mode_info.YResolution;
  bits_per_pixel = mode_info.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel + 8 - 1) / 8;
  if (map_memory(mode_info))
    return NULL;

  return set_mode(mode);
}

int(draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= h_res || x < 0 || y >= v_res || y < 0) {
    return 1;
  }
  unsigned int pos = (x + y * h_res) * bytes_per_pixel;
  memcpy(buff + pos, &color, bytes_per_pixel);
  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (uint16_t i = 0; i < len; i++) {
    if (draw_pixel(x + i, y, color))
      return 1;
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width,
                       uint16_t height, uint32_t color) {
  for (uint16_t i = 0; i < height; i++) {
    if (vg_draw_hline(x, y + i, width, color))
      return 1;
  }
  return 0;
}

int(draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {
  uint16_t width = h_res / no_rectangles;
  uint16_t height = v_res / no_rectangles;

  uint32_t color = 0;

  for (uint8_t row = 0; row < no_rectangles; row++) {
    for (uint8_t col = 0; col < no_rectangles; col++) {
      if (bytes_per_pixel == 1) {
        color = (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel);
      }
      else {
        uint32_t red = ((RED(first) + col * step) % (1 << mode_info.RedMaskSize)) << 16;
        uint32_t green = ((GREEN(first) + row * step) % (1 << mode_info.GreenMaskSize)) << 8;
        uint32_t blue = (BLUE(first) + (col + row) * step) % (1 << mode_info.BlueMaskSize);
        color = red + green + blue;
      }
      if (vg_draw_rectangle(width * col, height * row, width, height, color))
        return 1;
    }
  }
  return 0;
}

int(draw_sprite)(Sprite sprite, uint16_t x, uint16_t y) {
  if (sprite.pix_map == NULL)
    return 1;

  for (uint8_t i = 0; i < sprite.img.height; i++) {
    for (uint8_t j = 0; j < sprite.img.width; j++) {
      int pos = (j + i * sprite.img.width) * bytes_per_pixel;
      uint32_t color = sprite.pix_map[pos];
      color |= sprite.pix_map[pos + 1] << 8;
      color |= sprite.pix_map[pos + 2] << 16; 
      if (color != 0xff00ff) //transparent
        draw_pixel(x + j, y + i, color);
    }
  }
  return 0;
}

int(draw_letter)(Sprite sprite, uint16_t x, uint16_t y, uint32_t color) {
  if (sprite.pix_map == NULL)
    return 1;

  for (uint8_t i = 0; i < sprite.img.height; i++) {
    for (uint8_t j = 0; j < sprite.img.width; j++) {
      int pos = (j + i * sprite.img.width) * bytes_per_pixel;
      uint32_t c = sprite.pix_map[pos];
      c |= sprite.pix_map[pos + 1] << 8;
      c |= sprite.pix_map[pos + 2] << 16; 
      if (c != 0xff00ff) //transparent
        draw_pixel(x + j, y + i, color);
    }
  }
  return 0;
}

void (clear_screen)() {
  vg_draw_rectangle(0, 0, h_res, v_res, 0);
}

int (vg_refresh)() {
  if (vbe_set_display_start(video_mem < buff ? v_res : 0))
    return 1;
  char* tmp = video_mem;
  video_mem = buff;
  buff = tmp;
  for (unsigned int i = 0; i < vram_size / bytes_per_pixel; i++) {
    memcpy(buff + bytes_per_pixel * i, &bg_color, bytes_per_pixel);
  }
  //memset(buff, 0, vram_size);
  return 0;
}

int vbe_set_display_start(uint16_t start) {
  reg86_t r86;
  /* Specify the appropriate register values */
  memset(&r86, 0, sizeof(r86)); /* zero the structure7 */

  r86.intno = 0x10; /* BIOS video services */
  r86.ah = 0x4F;    /* VBE call */
  r86.al = 0x07;    /* set display start  */
  r86.bx = 0x0000;
  r86.dx = start;

  /* Make the BIOS call */

  if (sys_int86(&r86) != OK) {
    printf("\tvg_init(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int16_t (video_get_v_res)(void) {
  return (int16_t) v_res;
}

int16_t (video_get_h_res)(void) {
  return (int16_t) h_res;
}
