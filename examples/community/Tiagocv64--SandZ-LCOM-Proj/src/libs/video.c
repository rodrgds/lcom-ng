#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>
#include "video.h"

// static unsigned h_res;
// static unsigned v_res;
// static unsigned bits_pix;
// static char *v_mem;
// static char *aux_v_mem;

static uint8_t RedMaskSize;
static uint8_t RedFieldPosition;
static uint8_t GreenMaskSize;
static uint8_t GreenFieldPosition;
static uint8_t BlueMaskSize;
static uint8_t BlueFieldPosition;

int getVerResolution(){
  return (int)v_res;
}

int getHorResolution(){
  return (int)h_res;
}

u_char* getGraphicsBuffer(){
  return aux_v_mem;
}

uint32_t convert_phys_to_virtual(uint32_t p){
  return (uint32_t) (((p & 0xFFFF0000) >> 12) + (p & 0x0000FFFF));
}

void flip_buff(){
  memcpy(v_mem, aux_v_mem, h_res * v_res * ((bits_pix+7) / 8));
}

int vg_change_mode(uint16_t mode){
  struct reg86u reg86;

  memset(&reg86, 0, sizeof(reg86));	/* zero the structure */

  reg86.u.b.intno = INT;
  reg86.u.b.ah = 0x4F;
  reg86.u.b.al = SET_VBE_MODE;
  reg86.u.w.bx = mode | BIT(14);

  if( sys_int86(&reg86))
    return 1;

  return 0;
}

void *(vg_init)(uint16_t mode){

  lm_init(true);

  vbe_mode_info_t vmi_p;
  if (vbe_get_mode_info(mode, &vmi_p))
    return NULL;

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bits_pix = vmi_p.BitsPerPixel;
  RedMaskSize = vmi_p.RedMaskSize;
  RedFieldPosition = vmi_p.RedFieldPosition;
  GreenMaskSize = vmi_p.GreenMaskSize;
  GreenFieldPosition = vmi_p.GreenFieldPosition;
  BlueMaskSize = vmi_p.BlueMaskSize;
  BlueFieldPosition = vmi_p.BlueFieldPosition;

  int r;
  struct minix_mem_range mr; /* physical memory range */
  unsigned int vram_base = vmi_p.PhysBasePtr;  /* VRAM’s physical addresss */
  unsigned int vram_size = h_res * v_res * ((bits_pix+7) / 8);  /* VRAM’s size, but you can use
                               the frame-buffer size, instead */
  void *video_mem;         /* frame-buffer VM address */
  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED)
    panic("couldn’t map video memory");

  aux_v_mem = (u_char*) malloc(vram_size);
  if (aux_v_mem == NULL)
    return NULL;

  if (vg_change_mode(mode))
    return NULL;

  return v_mem = video_mem;
}

int vg_color_pixel(uint16_t x, uint16_t y, uint32_t color){

  if ((x>=h_res)||(y>=v_res))
    return 1;

  uint32_t bytes_pix = (bits_pix+7)/8;

  memcpy((aux_v_mem+((y*h_res+x)*bytes_pix)), &color, bytes_pix);

  if (bits_pix == 15)
    *(aux_v_mem + (y * h_res * bytes_pix) + (x * bytes_pix)+1) = (uint8_t) (color>>8) & ~0x80;

  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len , uint32_t color){

  for (int i = 0; i < len; i++){
    if(vg_color_pixel(x + i, y, color))
      return 1;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){

  for(int i=0; i< height;i++)
    vg_draw_hline(x, y + i, width, color);

  return 0;
}

void vg_draw_sprite(uint16_t* m_sprite, uint16_t h_sprite, uint16_t w_sprite, uint16_t y, uint16_t x){
  for (uint16_t i = 0; i < h_sprite; i++){
    for (uint16_t j = 0; j < w_sprite; j++){
      if (y + i < v_res &&  x + j < h_res)
        vg_color_pixel(x + j, y + i, *(m_sprite + i * w_sprite + j));
    }
  }
}

void vg_draw_text_sprite(char* m_sprite, uint16_t h_sprite, uint16_t w_sprite,
  uint16_t h_partial, uint16_t w_partial, uint16_t begin_y, uint16_t begin_x, uint16_t y, uint16_t x, uint32_t color, int text_size){
    if ((begin_y) * w_sprite + ( begin_x) < 0 || ((begin_y ) * w_sprite + ( begin_x)) > w_sprite*h_sprite){
      return;
    }

    for (uint16_t i = 0; i < h_partial; i++){
      for (uint16_t j = 0; j < w_partial; j++){
        if (y + i < v_res &&  x + j < h_res)
          if (*(m_sprite + (begin_y + i) * w_sprite + (j + begin_x)) != 0){
            for (int t = 0; t < text_size; t++){
              for (int t2 = 0; t2 < text_size; t2++){
                vg_color_pixel(x + text_size*j + t, y + text_size*i + t2, color);
              }
            }
          }
      }
    }
}

void vg_clear(){

  for(int i=0; i<h_res*v_res*bits_pix/8; i++)
    *(aux_v_mem + i) = 0;

}

uint8_t RGB_first(uint8_t maskSize, int8_t Position, uint32_t color) {

  uint32_t r_color = color >> Position;
  for (int i = maskSize; i < 32; i++)
    r_color -= (1 << i) & r_color;

  return r_color;
}

uint32_t d_color(uint16_t i, uint16_t j, uint32_t first, uint8_t step){
  uint8_t R_first = RGB_first(RedMaskSize, RedFieldPosition, first);
  uint8_t G_first = RGB_first(GreenMaskSize, GreenFieldPosition, first);
  uint8_t B_first = RGB_first(BlueMaskSize, BlueFieldPosition, first);

  uint32_t Red = (R_first + i * step) % (1 << RedMaskSize);
  uint32_t Green = (G_first + j * step) % (1 << GreenMaskSize);
  uint32_t Blue = (B_first + (i + j) * step) % (1 << BlueMaskSize);

  uint32_t color = (Red << RedFieldPosition) | (Green << GreenFieldPosition) | (Blue << BlueFieldPosition);

  return color;

}
