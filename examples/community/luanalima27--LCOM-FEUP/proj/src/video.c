#include "video.h"
#include <inttypes.h>

static void *video_mem;
vbe_mode_info_t vmi_p;
char *buff;

// initialize the video card
int(vg_initi)(uint16_t mode) {
  // memory range
  struct minix_mem_range mem_range;
  // vram base
  unsigned int vram_base;
  // size of vram
  unsigned int vram_size;
  int r;

  // return vbe mode information
  vbe_get_mode_info(mode, &vmi_p);

  // get vram Physical base
  vram_base = vmi_p.PhysBasePtr;

  // get vram size (max X resolution multiplied by max Y resolution multiplied by the amount of pixels)
  vram_size = vmi_p.XResolution * vmi_p.YResolution * ((vmi_p.BitsPerPixel + 7) / 8);

  // allocate the vram size to create a buffer
  buff = (char *) malloc(vram_size);

  // set memory range base to vram physical base
  mem_range.mr_base = (phys_bytes) vram_base;

  // set memory limit to memory range base added with vram size
  mem_range.mr_limit = mem_range.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_range))) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  }

  video_mem = vm_map_phys(SELF, (void *) mem_range.mr_base, vram_size);

  reg86_t r86;
  memset(&r86, 0, sizeof(r86)); /* zero the structure */

  r86.ah = 0x4F;
  r86.al = 0x02;
  r86.bx = 1 << 14 | mode;
  r86.intno = 0x10;

  if (sys_int86(&r86) != OK) {
    printf("vg_init: sys_int86() failed to set graphics mode\n");
    return 1;
  }

  if (video_mem == MAP_FAILED) {
    panic("couldn't map video memory");
  }

  return 0;
}

// draw pixel
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint8_t *color) {

  // if x is bigger than max x, stop
  if (x > vmi_p.XResolution) {
    printf("The paramater x is too big!");
    return 1;
  }

  // if y is bigger than max y, stop
  if (y > vmi_p.YResolution) {
    printf("The paramater y is too big!");
    return 1;
  }

  // copy mem with restriction of the max pixels with the buffer, color given, per pixel
  memcpy(buff + vmi_p.XResolution * y * ((vmi_p.BitsPerPixel + 7) / 8) + x * ((vmi_p.BitsPerPixel + 7) / 8), color, ((vmi_p.BitsPerPixel + 7) / 8));

  // return 0 if successful
  return 0;
}


//draw horizontal pixel
int (vg_draw_line)(uint16_t x, uint16_t y, uint16_t len, uint8_t* color) {

  //until widht ends
  for(int i = 0; i < len; i++) {
    //draw pixel
    if(vg_draw_pixel(x+i,y, color) != 0) {
      return 1;
    }
  }

  //return 0 if successful
  return 0;
}

//draw rectangle
int (vg_draw_rect)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t* color) {
  //until height ends
  for(int i = 0; i < height; i++) {
    //draw line
    if(vg_draw_line(x, y+i, width, color) != 0) {
      return 1;
    }
  }

  //return 0 if successful
  return 0;
}


// double buffer (write video to memory)
void(double_buffer)() {
  memcpy(video_mem, buff, vmi_p.XResolution * vmi_p.YResolution * ((vmi_p.BitsPerPixel + 7) / 8));
}

int(draw_xpm)(uint8_t *used_colors, xpm_image_t img, uint16_t x, uint16_t y) {

  if (used_colors == NULL) {
    return 1;
  }

  for (int height = 0; height < img.height; height++) {
    for (int width = 0; width < img.width; width++) {
      if (xpm_transparency_color(XPM_8_8_8) != 0) {
        if (vg_draw_pixel(x + width, y + height, used_colors) != 0) {
          return 1;
        }
        used_colors += 3;
      }
    }
  }

  return 0;
}


int (draw_sprite)(xpm_image_t img, uint8_t* sprite, uint16_t x, uint16_t y) {

for(int height = 0; height < img.height; height++){
    for(int width = 0; width < img.width; width++){

      if (vg_draw_pixel(x + width,y + height,sprite) != 0) {
        return 1;
      }
      sprite++;
    }
  }

  return 0;
}

