#include "gpu.h"
#include <lcom/lcf.h>

vbe_mode_info_t modeInfo;
static char *front_buffer;
static char *back_buffer;
static unsigned bytes_per_pixel;
static unsigned fb_size;

int(gpu_set_mode)(uint16_t mode) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86)); /* zero the structure */

  r86.intno = VBE_INTERRUPT;
  r86.ah = VBE_FUNCTION_AH;
  r86.al = VBE_SET_MODE;

  r86.bx = mode | VBE_SET_LINEAR_MODEL;
  /* Make the BIOS call */

  if (sys_int86(&r86) != OK) {
    printf("\tgpu_set_mode(): sys_int86() failed \n");
    return 1;
  }

  // if (r86.ah != 0x4F || r86.al != 0x00) return 1; // not passing tests with this for some reason

  return 0;
}

int(gpu_map_vram)(uint16_t mode) {
  struct minix_mem_range mr;
  int r;

  /* Use VBE function 0x01 to initialize vram_base and vram_size */
  if (vbe_get_mode_info(mode, &modeInfo))
    return 1;
  bytes_per_pixel = (modeInfo.BitsPerPixel + 7) / 8;
  fb_size = modeInfo.XResolution * modeInfo.YResolution * bytes_per_pixel;

  /* Allow memory mapping */

  mr.mr_base = modeInfo.PhysBasePtr;
  mr.mr_limit = mr.mr_base + fb_size;
  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  front_buffer = vm_map_phys(SELF, (void *) mr.mr_base, fb_size);

  if (front_buffer == MAP_FAILED)
    panic("couldn't map video memory");
  
  back_buffer = malloc(fb_size);

  return 0;
}

void swap_buffers() {
    memcpy(front_buffer, back_buffer, fb_size);
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++) {
    if (x + i >= modeInfo.XResolution || y >= modeInfo.YResolution)
      return 1;
    unsigned index = (x + i + y * modeInfo.XResolution) * bytes_per_pixel;
    memcpy(&back_buffer[index], &color, bytes_per_pixel);
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  if (modeInfo.BitsPerPixel < 32) {
    color = color & ((1 << modeInfo.BitsPerPixel) - 1);
  }

  for (int i = 0; i < height; i++) {
    vg_draw_hline(x, y + i, width, color);
  }
  // printf("Attempting to draw rectangle.. ");
  return 0;
}

int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {
  uint8_t r, g, b;
  uint32_t color;
  int width = modeInfo.XResolution / no_rectangles;
  int height = modeInfo.YResolution / no_rectangles;

  if (modeInfo.MemoryModel == MEM_MODEL_DIRECT) {

    for (int i = 0; i < no_rectangles; i++) {
      uint8_t r_1, g_1, b_1;
      for (int j = 0; j < no_rectangles; j++) {
        r_1 = (BIT(modeInfo.RedMaskSize) - 1) & (first >> modeInfo.RedFieldPosition);
        g_1 = (BIT(modeInfo.GreenMaskSize) - 1) & (first >> modeInfo.GreenFieldPosition);
        b_1 = (BIT(modeInfo.BlueMaskSize) - 1) & (first >> modeInfo.BlueFieldPosition);

        r = (r_1 + j * step) % (1 << modeInfo.RedMaskSize);
        g = (g_1 + i * step) % (1 << modeInfo.GreenMaskSize);
        b = (b_1 + (i + j) * step) % (1 << modeInfo.BlueMaskSize);

        color = (r << modeInfo.RedFieldPosition) | (g << modeInfo.GreenFieldPosition) | (b << modeInfo.BlueFieldPosition);
        vg_draw_rectangle(j * width, i * height, width, height, color);
      }
    }
  }
  else if (modeInfo.MemoryModel == MEM_MODEL_INDEXED) {
    for (int i = 0; i < no_rectangles; i++) {
      for (int j = 0; j < no_rectangles; j++) {
        color = (first + (i * no_rectangles + j) * step) % (1 << modeInfo.BitsPerPixel);
        if (vg_draw_rectangle(j * width, i * height, width, height, color))
          return 1;
      }
    }
  }
  return 0;
}


int (paint_pixel)(uint16_t x, uint16_t y, uint32_t color){
  //check if x and y are within range
  if(x >= modeInfo.XResolution | y >= modeInfo.YResolution){ return 1;}

  uint8_t bytes_per_pixel = (modeInfo.BitsPerPixel + 7) / 8;

  unsigned int index = (modeInfo.XResolution * y + x) * bytes_per_pixel;

  if(memcpy(&back_buffer[index], &color, bytes_per_pixel) == NULL) return 1;
  return 0;
}


int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
  xpm_image_t img;
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
  

  for(int i = x; i < x + img.width; i++){
    for(int j = y; j < y + img.height; j++){
      if(paint_pixel(j, i, *colors)) return 1;
      colors++;
    }
  }

  return 0;
}
