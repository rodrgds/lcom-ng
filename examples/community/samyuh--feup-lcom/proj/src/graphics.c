#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include <math.h>

void *video_mem;		    /* Process (virtual) address to which VRAM is mapped */
void *double_buffer;
vbe_mode_info_t vbe;
static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static uint8_t RedMaskSize;		/* size of direct color red mask in bits */
static uint8_t GreenMaskSize;	/* size of direct color green mask in bits */
static uint8_t BlueMaskSize;
static uint8_t RedPos;
static uint8_t GreenPos;
static uint8_t BluePos;
static uint8_t BytePixel;

int (vbe_mode_info)(uint16_t mode, vbe_mode_info_t *vbe){
  reg86_t r;
  memset(&r, 0, sizeof(reg86_t));
  mmap_t map;
  lm_alloc(sizeof(vbe_mode_info_t), &map);

  r.ax = 0x4F01;
  r.es = PB2BASE(map.phys);
  r.di = PB2OFF(map.phys);
  r.cx = mode;
  r.intno = 0x10;


  if (sys_int86(&r) != OK) {
    lm_free(&map);
    return 1;
  }

  *vbe = *(vbe_mode_info_t *) map.virt;

  lm_free(&map);

  return 0;
}


int vg_init_function(uint16_t mode){
  struct minix_mem_range mr;

  //Map the lower 1MB
  mmap_t map;
  map.phys = 0;
  map.size = 1024*1024;
  lm_alloc(1024*1024, &map);

  vbe_mode_info(mode, &vbe);

  h_res = vbe.XResolution;
  v_res = vbe.YResolution;

  bits_per_pixel = vbe.BitsPerPixel;

  RedMaskSize = vbe.RedMaskSize;
  RedPos = vbe.RedFieldPosition;

  GreenMaskSize = vbe.GreenMaskSize;
  GreenPos = vbe.GreenFieldPosition;

  BlueMaskSize = vbe.BlueMaskSize;
  BluePos = vbe.BlueFieldPosition;
  BytePixel = vbe.BitsPerPixel/8;

  unsigned int vram_base;
  unsigned int vram_size;
  vram_base = vbe.PhysBasePtr;
  vram_size = vbe.XResolution * vbe.YResolution * vbe.BitsPerPixel/8;

  //Memory map;
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  int r;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
    printf("sys_privctl (ADD_MEM) failed: %d\n", r);
    return 1;
  }

  //Map the videomem and double buffer
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED){
    printf("couldn't map video memory");
    return 1;
  }
  double_buffer = (char *) malloc(h_res * v_res *  bits_per_pixel/8);

  //Initialize in graphic mode
  reg86_t reg;
  memset(&reg, 0, sizeof(reg));
  reg.intno = 0x10;
  reg.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
  reg.bx = 1<<14|mode; // set bit 14: linear framebuffer
  if(sys_int86(&reg) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }

  return 0;
}

void (draw_pixel)(uint16_t x, uint16_t y, const void *color) {
  uint32_t pos = (y * vbe.XResolution + x)* BytePixel;
  uint8_t * pixel = ((uint8_t *) double_buffer + pos);

  char *clr = (char *)color;
  char *pxl = (char *)pixel;

  for (size_t i=0; i< BytePixel; i++)
    pxl[i] = clr[i];
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (size_t i = 0; i < len; i++) {
    if ((x+i) > vbe.XResolution || (y > vbe.YResolution)) break;
    draw_pixel(x+i, y, &color);
  }
  return 0;
}
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for(int j = 0; j < height; j++)
    if(vg_draw_hline(x, y+j, width, color) != OK);
  return 0;
}

int (draw_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step){
  uint16_t width = h_res/no_rectangles;
  uint16_t height = v_res/no_rectangles;
  uint32_t color;
  if(mode == 0x105){
    for(size_t col = 0; col < no_rectangles; col++){
      for(size_t row = 0; row < no_rectangles; row++){

        color = (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel);

        draw_rectangle(col*width, row*height, width, height, color);
      }
    }
  }

  if(mode == 0x115){
    uint8_t firstR = first >> RedPos;
    uint8_t firstG = first >> GreenPos;
    uint8_t firstB = first >> BluePos;
    for(size_t col = 0; col < no_rectangles; col++){
      for(size_t row = 0; row < no_rectangles; row++){
        uint8_t red = (firstR + col * step) % (1 << RedMaskSize);
        uint8_t green = (firstG + row * step) % (1 << GreenMaskSize);
        uint8_t blue = (firstB + (row + col) * step) % (1 << BlueMaskSize);

        color = (red << RedPos) + (green << GreenPos) + (blue << BluePos);

        draw_rectangle(col*width, row*height, width, height, color);
      }
    }
  }
  return 0;
}

int (draw_xpm)(uint16_t x, uint16_t y, xpm_image_t img) {
  int pixel_num = 0;
  for(int j = 0; j < img.height; j++){
    for (int i=0; i < img.width; i++){
      if(((uint16_t *)(img.bytes))[pixel_num] != xpm_transparency_color(img.type))
          draw_pixel(x+i,y+j,&((uint16_t *)(img.bytes))[pixel_num]);
      pixel_num++;
    }
  }
  return 0;
}
int (draw_xpm_resize)(uint16_t x, uint16_t y, xpm_image_t img,  double teta, double size) {
  int pixel_num = 0;
  teta = 3.14159265359 * teta / 180;
  for(int j = 0; j < img.height; j++){
    for (int i=0; i < img.width; i++){
      if(((uint16_t *)(img.bytes))[pixel_num] != xpm_transparency_color(img.type))
        draw_pixel(x+size*(i*cos(teta)-j*sin(teta)),y + size*(i*sin(teta)+j*cos(teta)),&((uint16_t *)(img.bytes))[pixel_num]);
      pixel_num++;
    }
  }

  return 0;
}

int (draw_xpm_rotated)(uint16_t x, uint16_t y, xpm_image_t img, double teta) {
  int pixel_num = 0;
  teta = 3.14159265359 * teta / 180;
  for(int j = 0; j < img.height; j++){
    for (int i=0; i < img.width; i++){
      if(((uint16_t *)(img.bytes))[pixel_num] != xpm_transparency_color(img.type))
        draw_pixel(x+i*cos(teta)-j*sin(teta),i*sin(teta)+y+j*cos(teta),&((uint16_t *)(img.bytes))[pixel_num]);
      pixel_num++;
    }
  }
  return 0;
}

int (double_buffering)(){
  memcpy(video_mem, double_buffer, h_res * v_res *  bits_per_pixel/8);
  return 0;
}

int (freeBuffering)(){
  free(double_buffer);
  return 0;
}

void (draw_pixel_mem)(uint16_t x, uint16_t y, const void *color) {
  uint32_t pos = (y * vbe.XResolution + x)* BytePixel;
  uint8_t * pixel = ((uint8_t *) video_mem + pos);

  char *clr = (char *)color;
  char *pxl = (char *)pixel;

  for (size_t i=0; i< BytePixel; i++)
    pxl[i] = clr[i];
}

int (draw_xpm_mem)(uint16_t x, uint16_t y, xpm_image_t img) {
  int pixel_num = 0;
  for(int j = 0; j < img.height; j++){
    for (int i=0; i < img.width; i++){
      if(((uint16_t *)(img.bytes))[pixel_num] != xpm_transparency_color(img.type))
          draw_pixel_mem(x+i,y+j,&((uint16_t *)(img.bytes))[pixel_num]);
      pixel_num++;
    }
  }
  return 0;
}
