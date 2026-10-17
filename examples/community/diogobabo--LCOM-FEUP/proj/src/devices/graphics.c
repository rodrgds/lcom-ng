#include "graphics.h"
#include <math.h>

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static char *buffer;		/* buffer (virtual) address to which a buffer is mapped */
static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned int bytes_per_pixel; /* Number of VRAM bytes per pixel */
static unsigned int bits_per_pixel; /* Number of VRAM bits per pixel */
vbe_mode_info_t info;

int (video_set_graphics)(uint16_t mode){
  vbe_get_mode_info(mode,&info);

  h_res = info.XResolution;
  v_res = info.YResolution;

  struct minix_mem_range mr;
  unsigned int vram_base;  /* VRAM's physical addresss */
  unsigned int vram_size;  /* VRAM's size, but you can use the frame-buffer size, instead */
  int r;				    

  // igualar às valores da struct
  vram_base = info.PhysBasePtr;
  bits_per_pixel = info.BitsPerPixel;
  bytes_per_pixel = ceil(info.BitsPerPixel / 8.0);
  vram_size = h_res * v_res * bytes_per_pixel;


  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size; 

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
   panic("sys_privctl (ADD_MEM) failed: %d\n", r);
  }

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED) {
   panic("couldn't map video memory");
  }
  buffer = malloc(vram_size);

  if(buffer == MAP_FAILED) {
   panic("couldn't map buffer memory");
  }

  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.ah = 0x4F;
  r86.al = 0x02;
  r86.bx = mode | VBEBIT;
  r86.intno = 0x10;

  if(sys_int86(&r86) != OK) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }

  return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if(x >= h_res || y >= v_res || x < 0 || y < 0) {
    return 1;
  }
  uint8_t* pix;
  pix = (uint8_t* ) buffer + (bytes_per_pixel * h_res * y + x * bytes_per_pixel);
  memcpy(pix,&color,bytes_per_pixel);
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for(unsigned int i=0; i < len; i++)
    {
      if(vg_draw_pixel(x+i, y, color) != 0) {return 1;}
    }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  for(unsigned int i=0; i < height; i++)
    {
      if(vg_draw_hline(x,y+ i, width, color) != 0){return 1;}
    }
  return 0;
}

int(vg_draw_matrix)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  uint32_t color;
  unsigned width = h_res / no_rectangles; // dimensoes retangulos
  unsigned height = v_res / no_rectangles;

  for(int i = 0; i < no_rectangles; i++) {
    for(int j = 0; j < no_rectangles; j++) {
      if(mode != INDEXED_COLORS_MODE) {

        uint8_t red_f =  (first >> info.RedFieldPosition) & (BIT(info.RedMaskSize) - 1); // R(f)
        uint8_t green_f = (first >> info.GreenFieldPosition) & (BIT(info.GreenMaskSize) - 1); // G(f)
        uint8_t blue_f = (first >> info.BlueFieldPosition) & (BIT(info.BlueMaskSize) - 1); // B(f)

        uint8_t red = (red_f + j * step) % (1 << info.RedMaskSize);
        uint8_t green = (green_f + i * step) % (1 << info.GreenMaskSize);
        uint8_t blue = (blue_f + (j + i) * step) % (1 << info.BlueMaskSize);

        color = (red << info.RedFieldPosition) | (green << info.GreenFieldPosition) | (blue << info.BlueFieldPosition);
      }
      else {
        color = (first + (i * no_rectangles + j) * step) % BIT(bits_per_pixel);
      }
      uint16_t x = j * width;
      uint16_t y = i * height;
      if(vg_draw_rectangle(x,y,width,height,color) != 0) {return 1;}
    }
  }
  return 0;
}

int (draw_pix_map)(uint16_t x, uint16_t y, uint8_t *map, xpm_image_t img) {
  for(unsigned int i = 0; i < img.width; i++) {
    for(unsigned int j = 0; j < img.height; j++) {
      uint32_t color;
      uint8_t * pos = map +  j*img.width * bytes_per_pixel + i*bytes_per_pixel;
      memcpy(&color, pos, bytes_per_pixel);
     if (color != xpm_transparency_color(img.type)) {
       vg_draw_pixel(x + i, y + j, color);
     }
    }
  }
  return 0;
}

int (draw_letter_map)(uint16_t x, uint16_t y, uint8_t *map, xpm_image_t img, unsigned int startX, unsigned int startY) {
  for(unsigned int i = startX; i < 37 + startX; i++) {
    for(unsigned int j = startY; j < 40 + startY; j++) {
      uint32_t color;
      uint8_t * pos = map +  j*img.width * bytes_per_pixel + i*bytes_per_pixel;
      memcpy(&color, pos, bytes_per_pixel);
     if (color != xpm_transparency_color(img.type)) {
       vg_draw_pixel(x + i, y + j, color);
     }
    }
  }
  return 0;
}


int (clear_pix_map)(uint16_t x, uint16_t y, xpm_image_t img) {
  vg_draw_rectangle(x,y,img.width,img.height,0);
  return 0;
}

void switchBuffer(){
  memcpy(video_mem,buffer,h_res*v_res*bytes_per_pixel);
}

void cleanBG(unsigned int x, unsigned int y, int width, int height, xpm_image_t img, uint8_t* map) {
  if (width <= 0 || height <= 0 || x >= img.width || y >= img.height || x >= h_res || y >= v_res) return;
  unsigned int columns = width, rows = height;
  if (columns > img.width - x) columns = img.width - x;
  if (columns > h_res - x) columns = h_res - x;
  if (rows > img.height - y) rows = img.height - y;
  if (rows > v_res - y) rows = v_res - y;
  for(unsigned int i = x; i < columns + x; i++) {
    for(unsigned int j = y; j < rows + y; j++) {
      uint32_t color = 0;
      uint8_t * pos = map + j*img.width * bytes_per_pixel + i*bytes_per_pixel;
      memcpy(&color, pos, bytes_per_pixel);
     if (color != xpm_transparency_color(img.type)) {
       vg_draw_pixel(i,j, color);
     } 
    }
  }
}

void exit_graphics() {
  vg_exit();
  free(buffer);
}
