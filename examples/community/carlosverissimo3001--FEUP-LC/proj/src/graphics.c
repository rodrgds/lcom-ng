#include <lcom/lcf.h>

#include "graphics.h"

/* Process (virtual) address to which VRAM is mapped */
static uint8_t *video_mem;

/* Stores information related to the graphics mode that is set */
static vbe_mode_info_t vbe_info;

/* Keeps the contents of the current frame -> from pen_pal.c*/
extern xpm_image_t frame;

int(set_graphics_mode)(uint16_t mode) {
  reg86_t reg86;

  /* To avoid unexpected behavior, clear the ununsed registers */
  bzero(&reg86, sizeof reg86);

  reg86.ax = SET_VBE_MODE;
  reg86.bx = mode | LINEAR_BUFFER;
  reg86.intno = BIOS_ACESS_GRAPHICS;

  if (sys_int86(&reg86) != OK) {
    printf("set_vbe_mode(): sys_int86() failed \n");
    return 1;
  }

  /*  If the VBE function is supported, value 0x4f is returned in the AL */
  if (reg86.al != FUNCTION_SUPPORTED) {
    printf("VBE function not supported\n");
    return 1;
  }

  /*  If the VBE function completed successfully, value 0x00 is returned in the AH */
  if (reg86.ah != 0x00) {
    printf("VBE function was not successful\n");
    printf("Cause: ");
    switch (reg86.ah) {
      case FUNCTION_CALL_FAIL:              printf("Function call failed\n"); break;
      case FUNCTION_NOT_SUPPORTED_BY_HW:    printf("Function is not supported in current HW configuration\n"); break;
      case FUNCTION_INV_IN_CURR_VIDEO_MODE: printf("Function is invalid in current video mode"); break;
      default:
        break;
      }
    return 1;
  }

  return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, bool addToBackgroundFrame) {
  uint8_t *ptr = video_mem;
  uint8_t i;

  unsigned int width = vbe_info.XResolution;
  unsigned int bytes_per_pixel = vbe_info.BytesPerScanLine / width;

  ptr += bytes_per_pixel * (y * width + x);

  for (i = 0; i < bytes_per_pixel; i++) {
    *ptr = (color >> 8 * i);
    ptr++;
  }

  if (addToBackgroundFrame)
    add_pixel_to_frame(x, y, color);

  return 0;
}

void (add_pixel_to_frame)(uint16_t x, uint16_t y, uint32_t color){
  uint32_t *frame_bytes = (uint32_t*) frame.bytes;

  *(frame_bytes + x + y * frame.width) = color;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  unsigned int width = vbe_info.XResolution;

  for (uint16_t i = x; i < (x + len); i++) {
    if (i >= width)
      break;
    vg_draw_pixel(i, y, color, false);
  }
  return 0;
}

int(my_vg_draw_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, bool addToBackgroundFrame, bool horizontal) {
  unsigned int width = vbe_info.XResolution;
  unsigned int height = vbe_info.YResolution;

  if (horizontal){
    for (uint16_t i = x; i < (x + len); i++) {
      if (i >= width)
        break;
      vg_draw_pixel(i, y, color, addToBackgroundFrame);
    }
  }
  else{
    for (uint16_t i = y; i < (y + len); i++) {
      if (i >= height)
        break;
      vg_draw_pixel(x, i, color, addToBackgroundFrame);
    }
  }

  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  /* Draw horizontal lines from left to right, top to bottom*/

  unsigned int screen_height = vbe_info.YResolution;

  for (uint16_t i = y; i < (height + y); i++) {
    // if i is greater or equal than the vertical resolution, break
    if (i >= screen_height)
      break;
    // x is x, y is (y + delta), len is width
    vg_draw_hline(x, i, width, color);
  }
  return 0;
}

int(my_vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, bool add_rectangle_to_frame){
/* Draw horizontal lines from left to right, top to bottom*/

  unsigned int screen_height = vbe_info.YResolution;

  for (uint16_t i = y; i < (height + y); i++) {
    // if i is greater or equal than the vertical resolution, break
    if (i >= screen_height)
      break;
    // x is x, y is (y + delta), len is width
    my_vg_draw_line(x, i, width, color, add_rectangle_to_frame, true);
  }
  return 0;
}

int(map_vram)(uint16_t mode) {
  int r;

  /* Allow memory mapping */
  struct minix_mem_range mr;

  mr.mr_base = (phys_bytes) vbe_info.PhysBasePtr;

  unsigned int width = vbe_info.XResolution;
  unsigned int height = vbe_info.YResolution;
  unsigned int bytes_per_pixel = vbe_info.BytesPerScanLine / width;
  unsigned int vram_size = width * height * bytes_per_pixel;

  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  return 0;
}

int vgcard_init(uint16_t mode) {
  bzero(&vbe_info, sizeof(vbe_info));


  if (vbe_get_mode_info(mode, &vbe_info) != OK) {
    printf("Error while getting VBE mode information\n");
    return 1;
  }

  /* Map the graphics mode VRAM into the process' address space */
  if (map_vram(mode) != OK) {
    printf("Error while mapping the VRAM into the process' adress space\n");
    return 1;
  }

  /* set the mode */
  if (set_graphics_mode(mode) != OK) {
    printf("Error setting the interface to the desired graphics mode\n");
    return 1;
  }

  return 0;
}

int(draw_pixmap)(uint16_t x, uint16_t y, xpm_image_t *img, bool addToBackgroundFrame) {
  uint32_t *bytes = (uint32_t*) img->bytes;
  for (uint16_t i = 0; i < img->width; i++) {
    for (uint16_t j = 0; j < img->height; j++) {
      if (*(bytes + j*img->width + i) != TRANSPARENCY)
        vg_draw_pixel((x + i), (y + j), *(bytes + j*img->width + i), addToBackgroundFrame);
    }
  }
  return 0;
}

int (draw_pixmap_fixed_color)(uint16_t x, uint16_t y, xpm_image_t *img, uint32_t color, bool addToBackgroundFrame){
  uint32_t *bytes = (uint32_t*) img->bytes;
  for (uint16_t i = 0; i < img->width; i++) {
    for (uint16_t j = 0; j < img->height; j++) {
      if (*(bytes + j*img->width + i) != TRANSPARENCY)
        vg_draw_pixel((x + i), (y + j), color, addToBackgroundFrame);
    }
  }

  return 0;
}

void(get_screen_dimensions)(int *screen_width, int *screen_height){
  *screen_height = vbe_info.YResolution;
  *screen_width = vbe_info.XResolution;
}


void(draw_unfilled_square)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, bool addToBackgroundFrame){
  /* Draw horizontal top line */
  my_vg_draw_line(x, y, len, color, addToBackgroundFrame, true);

  /* Draw vertical left line */
  my_vg_draw_line(x, y, len, color, addToBackgroundFrame, false);

  /* Draw horizontal bottom line */
  my_vg_draw_line(x, y + len - 1, len,  color, addToBackgroundFrame, true);

  /* Draw vertical right line */
  my_vg_draw_line(x + len - 1, y, len, color, addToBackgroundFrame, false);
}

