#include <lcom/lcf.h>
#include <lcom/video_gr.h>

#include <stdint.h>

#include "bios.h"
#include "vbe_func.h"

static void *video_mem;
static void *video_buffer;

static vbe_mode_info_t vmi_p;
static unsigned int vram_size;

static uint16_t hres; // XResolution
static uint16_t vres; // YResolution
static uint8_t clrdepth; // color depth (no. of bits per pixel)

void *(vg_init)(uint16_t mode) {
  /* initially, get mode information */
  memset(&vmi_p, 0, sizeof(vbe_mode_info_t));
  if (vbe_get_mode_info(mode, &vmi_p) != OK)
    return NULL;

  hres = vmi_p.XResolution;
  vres = vmi_p.YResolution;
  clrdepth = vmi_p.BitsPerPixel;

  if (map_memory(mode))
    return NULL;

  reg86_t r86;
  memset(&r86, 0, sizeof(r86)); // zero the structure

  /* specify the appropriate register values */
  r86.intno = BIOS_INT; 
  r86.ah = VBE_FUNC_CALL; 
  r86.al = VBE_SET_MODE; 
  r86.bx = mode | VBE_LINEAR_BUF; 

  if (sys_int86(&r86) != OK) {
    printf("set_vbe_mode: sys_int86() failed\n");
    return NULL;
  }

  return video_mem;
}

int (map_memory)(uint16_t mode) {
  int r;
  struct minix_mem_range mr;
  mr.mr_base = vmi_p.PhysBasePtr;
  vram_size = hres * vres * bytes_per_pixel();
  mr.mr_limit = mr.mr_base + vram_size;

  if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return 1;
  }
  
  /* map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED) {
    panic("couldn't map video memory\n");
    return 1;
  }

  video_buffer = malloc(vram_size);

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++) {
    if (vg_generate_pixel(x + i, y, color) != OK)
      return 1;
  }

  return 0;
}
  
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height; i++)
    if (vg_draw_hline(x, y + i, width, color) != OK)
      return 1;

  return 0;
}

uint16_t (get_hres)() {
  return hres;
}

uint16_t (get_vres)() {
  return vres;
}

int (bytes_per_pixel)() {
  int bits = (int)clrdepth;
  int bytes = (bits / 8) + ((bits % 8) ? 1 : 0);
  return bytes;
}

int (vg_generate_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  uint8_t *ptr;
  ptr = (uint8_t *)video_buffer + (y * hres + x) * bytes_per_pixel();
  for (int i = 0; i < bytes_per_pixel(); i++) {
    *(ptr + i) = color;
    color = color >> 8;
  }

  return 0;
}

void (vg_clear_screen)() {
  memset(video_buffer, 0, vram_size);
}

void (vg_draw_content)() {
  memcpy(video_mem, video_buffer, vram_size);
}

uint8_t *(vg_read_pixmap)(xpm_row_t *pixmap, uint16_t *width, uint16_t *height) {
  if (width == NULL || height == NULL) {
    printf("null pointer\n");
    return NULL;
  }

  enum xpm_image_type type = XPM_INDEXED;
  xpm_image_t img;
  uint8_t *sprite = xpm_load((xpm_map_t)pixmap, type, &img);

  if (sprite == NULL) {
    printf("null pointer\n");
    return NULL;
  }

  *width = img.width;
  *height = img.height;

  return sprite;
}

int (vg_draw_sprite)(uint8_t *sprite, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  if (sprite == NULL) {
    printf("null pointer\n");
    return 1;
  }

  for (uint16_t i = 0; i < width; i++)
    for (uint16_t j = 0; j < height; j++)
      if (vg_generate_pixel(x + i, y + j, sprite[j * width + i]) != OK)
        return 1;

  return 0;
}

int (vg_draw_element)(xpm_row_t *pixmap, uint16_t x, uint16_t y) {
  uint16_t width = 0, height = 0;
  uint8_t *sprite = vg_read_pixmap(pixmap, &width, &height);
  if (sprite == NULL)
    return 1;
    
  if (vg_draw_sprite(sprite, x, y, width, height))
    return 1;
  
  return 0;
}
