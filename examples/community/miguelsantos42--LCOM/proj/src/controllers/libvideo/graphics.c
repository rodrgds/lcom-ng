#include "src/include/controllers/video/graphics.h"

int start_videomode(uint16_t mode){
  reg86_t r86;
  
  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02; 
  // modo tem de ser passado no registo bx -> r86.bx
  r86.bx = BIT(14) | mode;

  if(sys_int86(&r86) != 0) return 1;
  return 0;
}

int exit_videomode() {
  reg86_t r86;

  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;
  r86.ah = 0x00;
  r86.al = 0x03;
  
  if( sys_int86(&r86) != 0 ) return 1;
  return 0;
}

int map_memory(uint16_t mode){
  memset(&mode_info, 0, sizeof(mode_info));
  if(vbe_get_mode_info(mode, &mode_info) != 0) return 1;

  unsigned vram_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);

  struct minix_mem_range mr;

  mr.mr_base = mode_info.PhysBasePtr;	
  mr.mr_limit = mr.mr_base + vram_size;  

  if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0) return 1;

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  second_buffer = malloc(vram_size);
  if(second_buffer == NULL) return 1;

  return 0;
}

int color_correction(uint32_t color, uint32_t *new_color){
  if(mode_info.BitsPerPixel == 32){
    *new_color = color;
  } else {
    *new_color = color & (BIT(mode_info.BitsPerPixel) - 1);
  }
  return 0;
}

int draw_pixel(int16_t x, int16_t y, uint32_t color){
  if(x >= mode_info.XResolution || x < 0 || y < 0 || y >= mode_info.YResolution || color == 16) return 0;
  unsigned bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  uint8_t* position = (uint8_t*) second_buffer + (y * mode_info.XResolution + x) * bytesPerPixel;
  memcpy(position, &color, bytesPerPixel);
  return 0;
}

int draw_line(int16_t x, int16_t y, uint16_t width, uint32_t color){
  for(int i = 0; i < width && (x+i) < mode_info.XResolution; i++){
    draw_pixel(x+i,y,color);
  }
  return 0;
}

int draw_rectangle(int16_t x,int16_t y,uint16_t width,uint16_t height, uint32_t color){
  for(int i = 0; i < height && (y+i) < mode_info.YResolution; i++){
    draw_line(x,y+i,width,color);
  }
  return 0;
}

uint32_t calculate_indexed_color(uint8_t no_rectangles, uint32_t first, uint8_t step, uint8_t row, uint8_t col){
  return (first + (row * no_rectangles + col) * step) % (1 << mode_info.BitsPerPixel);
  // index(row, col) = (first + (row * no_rectangles + col) * step) % (1 << BitsPerPixel)
}

uint32_t calculate_direct_color(uint32_t first, uint8_t step, uint8_t row, uint8_t col){
  uint8_t R = ((((1 << mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition)) + col * step) % (1 << mode_info.RedMaskSize);
  uint8_t G = ((((1 << mode_info.GreenMaskSize) - 1) & (first >> mode_info.GreenFieldPosition)) + row * step) % (1 << mode_info.GreenMaskSize);
  uint8_t B = ((((1 << mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition)) + (col + row) * step) % (1 << mode_info.BlueMaskSize);
  
  // (R, G, B) -> 0xFF FF FF

  // R -> 0x00 00 FF -> 0xFF 00 00
  return (R << mode_info.RedFieldPosition) | (G << mode_info.GreenFieldPosition) | (B << mode_info.BlueFieldPosition);
}

int draw_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step){
  
  uint8_t vertical = mode_info.YResolution / no_rectangles;
  uint8_t horizontal = mode_info.XResolution / no_rectangles;

  for(int i = 0; i < no_rectangles; i++){
    for(int j = 0; j < no_rectangles; j++){

      uint32_t color;

      if(mode_info.MemoryModel == 0x06){
        color = calculate_direct_color(first, step, i, j);
      } else {
        color = calculate_indexed_color(no_rectangles, first, step, i, j);
      }

      if(draw_rectangle(j * horizontal, i * vertical, horizontal, vertical, color) != 0) return 1;
    }
  }
  return 0;
}

int draw_xpm(xpm_image_t xpm_image, uint8_t* map, int16_t x, int16_t y){
  if(x >= 1024 || y >= 768 || x+xpm_image.width < 0 || y+xpm_image.height < 0) return 0;
  for(int i = 0; i < xpm_image.height; i++){
    for(int j = 0; j < xpm_image.width; j++){
      if(draw_pixel(x + j,y + i, (uint32_t) *map) != 0) return 1;
      map++;
    }
  }
  return 0;
}

int load_xpm(xpm_map_t xpm, xpm_image_t *img, uint8_t** map){
  *map = xpm_load(xpm, XPM_INDEXED, img);
  if(*map == NULL) return 1;
  return 0;
}

void reset_buffer() {
    if (!second_buffer) return;
    memset(second_buffer, 0, mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8));
}

int change_buffer() {
  if(!second_buffer || !video_mem) return 1;
  memcpy(video_mem, second_buffer, mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8));
  return 0;
}

int free_buffer() {
  if(second_buffer) {
    free(second_buffer);
    second_buffer = NULL;
  }
  return 0;  
}

