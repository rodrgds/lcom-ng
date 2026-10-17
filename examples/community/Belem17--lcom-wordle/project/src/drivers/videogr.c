#include "graphics.h"
#include <lcom/lcf.h>
#include <lcom/video_gr.h>
#include <lcom/vbe.h>
#include <stdint.h>
#include <stdio.h>
static void *video_mem[3];

static uint16_t h_res;
static uint16_t v_res;
static uint16_t bytes_per_pixel;
vbe_mode_info_t info;
static int ind=0;

int(vg_int)(uint16_t mode) {
  memset(&info, 0, sizeof(info));
  if (vbe_get_mode_info(mode, &info))
    return 1;

  h_res = info.XResolution;
  v_res = info.YResolution;
  bytes_per_pixel = (info.BitsPerPixel + 7) / 8;

  int r;
  struct minix_mem_range mr;                 /* physical memory range */
  unsigned int vram_base = info.PhysBasePtr; /* VRAM’s physical addresss */
  unsigned int vram_size = (unsigned int) info.BytesPerScanLine * info.YResolution;

  mr.mr_base = vram_base;
  mr.mr_limit = mr.mr_base + vram_size *3;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  
  for(int i=0;i<3;i++){
    video_mem[i] = vm_map_phys(SELF, (void *) (mr.mr_base+ i *vram_size), vram_size);
    if (video_mem[i] == MAP_FAILED)
    panic("couldn’t map video memory");
  }  
  return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  uint8_t *begin = (uint8_t *) video_mem[ind]+ bytes_per_pixel * (y * h_res + x);
  if(color==xpm_transparency_color(XPM_8_8_8_8)){
    return 0;
  }
  memcpy(begin, &color, bytes_per_pixel);
  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (x + len > h_res) {
    len = h_res - x;
  }
  for (int i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color))
      return 1;
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height && y + i < v_res; i++) {
    if (vg_draw_hline(x, y + i, width, color))
      return 1;
  }
  return 0;
}

int(vg_enter)(uint16_t mode) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));
  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02;
  r86.bx = (mode | BIT(14));
  if (sys_int86(&r86) != OK)
    return 1;
  return 0;
}

int (vg_f7_withsubfunction80)(){
  reg86_t r86;
  memset(&r86,0,sizeof(r86));
  r86.intno=0x10;
  r86.ah=0x4F;
  r86.al=0x07;
  r86.bl=0x80;
  r86.dx=ind*info.YResolution;
  if (sys_int86(&r86) != OK)
    return 1;
  return 0;
}

void clean_screen(){
  uint32_t color = 0x302c3c;
  memset(video_mem[ind],color,info.XResolution*bytes_per_pixel*info.YResolution);
}

int(vg_draw_xpm)(uint16_t xi, uint16_t yi, uint8_t *map,uint16_t width ,uint16_t height) {
  for (int i = 0; i <width; i++) {
    for (int j = 0; j < height; j++) {
      uint32_t color=*((uint32_t *) &map[(i + j * width)*4]);
      if (vg_draw_pixel(xi+i, yi+j, color))
        return 1;
    }
  }

  return 0;
}
