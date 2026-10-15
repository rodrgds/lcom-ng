#include <lcom/lcf.h>
#include "graphics.h"
#include <math.h>

uint8_t* img_buffer = NULL;
unsigned int bytes_per_pixel;
unsigned int framebuffer_size;

int (set_graphic_mode)(uint16_t submode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86)); 
                                
    reg86.ax = 0x4F02;             
    reg86.bx = submode | BIT(14); 
    reg86.intno = 0x10;  
    
    if (sys_int86(&reg86) != 0) {     
        printf("Failed to set graphic mode\n");
        return 1;
    }
    return 0;
}

int (set_text_mode)() {
    reg86_t reg86;                       
    memset(&reg86, 0, sizeof(reg86));  
                   
    reg86.ax = 0x0003;          
    reg86.bx = 0x0000;  
    reg86.intno = 0x10;
    
    if(sys_int86(&reg86) != 0) {        
        printf("Set text mode failed\n");
        return 1;
    }
    return 0;
}

int (set_frame_buffer)(uint16_t mode){
  memset(&mode_info, 0, sizeof(vbe_mode_info_t));
  if(vbe_get_mode_info(mode, &mode_info) != 0) {
    printf("Failed to retrieve VBE mode info\n");
    return 1;
  }

  bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
  framebuffer_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = mode_info.PhysBasePtr;
  physic_addresses.mr_limit = physic_addresses.mr_base + mode_info.LinBytesPerScanLine * mode_info.YResolution;
  
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses) != 0) {
    printf("Failed to grant physical memory access\n");
    return 1;
  }

  size_t fb_map_size = mode_info.LinBytesPerScanLine * mode_info.YResolution;
  frame_buffer = vm_map_phys(SELF, (void*) physic_addresses.mr_base, fb_map_size);
  if (frame_buffer == NULL) {
    printf("Failed to virtually allocate memory\n");
    return 1;
  }
  return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if(x >= mode_info.XResolution || y >= mode_info.YResolution) return 1;
  unsigned int index = (mode_info.XResolution * y + x) * bytes_per_pixel;
  if (memcpy(&img_buffer[index], &color, bytes_per_pixel) == NULL) return 1;
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (uint16_t dx = 0 ; dx < len ; dx++)   
    if (vg_draw_pixel(x+dx, y, color) != 0) return 1;
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for(uint16_t i = 0; i < height ; i++)
    if (vg_draw_hline(x, y+i, width, color) != 0) return 1;
  return 0;
}

int normalize_color(uint32_t color, uint32_t *new_color) {
  if (mode_info.BitsPerPixel == 32) {
    *new_color = color;
  } else {
    *new_color = color & (BIT(mode_info.BitsPerPixel) - 1);
  }
  return 0;
}

uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B) {
  return (R << mode_info.RedFieldPosition) | (G << mode_info.GreenFieldPosition) | (B << mode_info.BlueFieldPosition);
}

uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
  return (first + (row * n + col) * step) % (1 << mode_info.BitsPerPixel);
}

uint32_t (Red)(unsigned j, uint8_t step, uint32_t first) {
  return (R(first) + j * step) % (1 << mode_info.RedMaskSize);
}

uint32_t (Green)(unsigned i, uint8_t step, uint32_t first) {
  return (G(first) + i * step) % (1 << mode_info.GreenMaskSize);
}

uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first) {
  return (B(first) + (i + j) * step) % (1 << mode_info.BlueMaskSize);
}

uint32_t (R)(uint32_t first){
  return ((1 << mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition);
}

uint32_t (G)(uint32_t first){
  return ((1 << mode_info.GreenMaskSize) - 1) & (first >> mode_info.GreenFieldPosition);
}

uint32_t (B)(uint32_t first){
  return ((1 << mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition);
}

void allocateImgBuffer(){
  img_buffer = (uint8_t *)malloc(framebuffer_size); 
}

void copyBufferToVideo(){
  memcpy(frame_buffer, img_buffer, framebuffer_size); 
}

void clearImgBuffer(){
  memset(img_buffer, 0, framebuffer_size); 
}

void freeImgBuffer(){
  free(img_buffer); 
}

uint8_t *get_buffer() {
  return img_buffer;
}

