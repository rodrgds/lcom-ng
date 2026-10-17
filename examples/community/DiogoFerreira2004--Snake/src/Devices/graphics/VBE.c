#include <lcom/lcf.h>

#include "VBE.h"
#include <lcom/lab5.h>
#include <minix/sysutil.h>
#include <string.h>

struct minix_mem_range mr;
static void *video_mem;

extern uint8_t scancode;
vbe_mode_info_t vmi;

void(vg_clear)() {
  uint32_t vram_size = vmi.XResolution * vmi.YResolution * (vmi.BitsPerPixel / 8);
  memset(video_mem, 0, vram_size);
}

void *(vg_init) (uint16_t mode) {
  int r;
  vbe_get_mode_info(mode, &vmi);
  uint32_t vram_size = vmi.XResolution * vmi.YResolution * (vmi.BitsPerPixel / 8);
  uint32_t vram_base = vmi.PhysBasePtr;
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  if (vbe_set_mode(mode) != 0)
    return NULL;

  return video_mem;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color) != 0) {
      return 1; // Return error if drawing pixel fails
    }
  }
  return 0; // Return success
}

int(vbe_set_mode)(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.intno = INT_NUMBER;
  r.ah = SET_VBE_MODE_AH;
  r.al = SET_VBE_MODE_AL;
  r.bx = mode | BIT(14);

  if (sys_int86(&r) != OK) {
    printf("vbe_set_mode: sys_int86() failed \n");
    return 1;
  }

  if (r.ax != 0x004F) {
    printf("vbe_set_mode: vbe_set_mode failed \n");
    return 1;
  }

  return 0;
}


int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= vmi.XResolution || y >= vmi.YResolution) {
    return 1;
  }
  uint32_t offset = (y * vmi.XResolution + x) * ((vmi.BitsPerPixel + 7) / 8);
  memcpy((uint8_t *) video_mem + offset, &color, (vmi.BitsPerPixel + 7) / 8);

  return 0;
}


int(our_vg_draw_rectangle_implementation)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (unsigned i = 0; i < height; i++)
    if (vg_draw_hline(x, y + i, width, color) != 0) {
      vg_exit();
      return 1;
    }
  return 0;
}


int(draw_xpm_image)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  enum xpm_image_type type = XPM_INDEXED;
  xpm_image_t img;
    
  uint8_t *sprite = xpm_load(xpm, type, &img);
  uint8_t *sprite_start = sprite;
  
  if (sprite == NULL) {
    printf("Error loading XPM: sprite is NULL\n");
    return 1;
  }
  
  
  if (x + img.width > vmi.XResolution || y + img.height > vmi.YResolution) {
    printf("Warning: XPM would be drawn outside screen bounds!\n");
    printf("Screen resolution: %dx%d\n", vmi.XResolution, vmi.YResolution);
  }
  
  for (int height = 0; height < img.height; height++) {
    for (int width = 0; width < img.width; width++) {
      if (vg_draw_pixel(x + width, y + height, *sprite) != 0) {
        printf("Error drawing pixel at (%d, %d)\n", x + width, y + height);
        free(sprite_start);
        return 1;
      }
      sprite++;
    }
  }
  
  free(sprite_start);
  return 0;
}
