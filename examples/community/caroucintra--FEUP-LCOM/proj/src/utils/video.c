#include <lcom/lcf.h>
#include <math.h>
#include "video.h"

static unsigned h_res;
static unsigned v_res;
static unsigned bitsPerPixel;
static unsigned bytesPerPixel;
static uint8_t *video_mem;
static uint8_t *double_buffer;
unsigned int vram_size;
struct minix_mem_range mr;

int(set_graphics_mode)(uint16_t mode){
    reg86_t r86;

    memset(&r86, 0, sizeof(r86));
    r86.intno = 0x10; 
    r86.ah = 0x4F;
    r86.al = 0x02;
    r86.bx = mode | BIT(14);

    if(sys_int86(&r86) == EFAULT) 
    {
        printf("\tset_VBE_mode(): sys_int86() failed \n");
        return 1;
    }
    return 0;
}

int(map_VRAM)(uint16_t mode)
{

    struct minix_mem_range mr;
    unsigned int vram_base;  /* VRAM's physical addresss */
    int r;				    

    /* Use VBE function 0x01 to initialize vram_base and vram_size */
    vbe_mode_info_t vbe_info;
    if(vbe_get_mode_info(mode, &vbe_info)){
        printf("vbe_get_mode_info call failed int map_VRAM.\n");
        return 1;
    }

    vram_base = vbe_info.PhysBasePtr;
    h_res = vbe_info.XResolution;
    v_res = vbe_info.YResolution;
    bitsPerPixel = vbe_info.BitsPerPixel;
    bytesPerPixel = ceil(bitsPerPixel / 8.0);
    vram_size = h_res * v_res * bytesPerPixel * 2;

    /* Allow memory mapping */

    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK)
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED)
    {
        panic("couldn't map video memory");
        return 1;
    }

    return 0;
}

int (init_vg) (uint16_t mode) {
  if (map_VRAM(mode))
    return 1;
  if (set_graphics_mode(mode))
    return 1;
  double_buffer = (uint8_t*)malloc(vram_size/2);
  double_buffer = video_mem + vram_size/2;
  return 0;
}

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {

  if (x >= h_res || y >= v_res || x < 0 || y < 0){
    return 0;
  }

  unsigned pos = bytesPerPixel * (h_res * y + x);
  uint8_t* buffer = double_buffer + pos;

  if (color != 0x000001) {
    for(unsigned i = 0; i < bytesPerPixel; i++){
      *buffer = color;
      color >>= 8;
      buffer++;
    }
  }

  return 0;
}

void change_buffer(){
  memcpy(video_mem, double_buffer, vram_size/2);
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned i = 0; i < len; i++) {
    if (x + i >= h_res)
      return 1;
    draw_pixel(x + i, y, color);
  }

  return 0;
}

int(draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (unsigned i = 0; i < height; i++) {
    if (y + i >= v_res)
      return 1;
    if (vg_draw_hline(x, y + i, width, color))
      return 1;
  }

  return 0;
}

int (draw_pixmap)(uint8_t *map, xpm_image_t img, uint16_t x, uint16_t y) {
  for (size_t i = 0; i < img.height; i++) {
    if ((y + i) >= v_res)
      break;

    for (size_t j = 0; j < img.width; j++) {
      if ((x + j) >= h_res)
        break;

      draw_pixel((x + j), (y + i), *(map + j + i * img.width));
    }
  }

  return 0;
}

int(reset_terminal)() {
  if (draw_rectangle(0, 0, h_res, v_res, 0x000001))
    return 1;
  return 0;
}

unsigned get_hres() {
  return h_res;
}
unsigned get_vres() {
  return v_res;
}

unsigned get_bitsperpixel() {
  return bitsPerPixel;
}
unsigned get_bytesperpixel() {
  return bytesPerPixel;
}
