#include <lcom/lcf.h>
#include "graphics.h"
#include <math.h>

/**
 * @brief Changes minix to graphics mode
 * @param submode submode to be set
 * @return 0 if successful, 1 otherwise
*/
int (graphics_set_mode)(uint16_t submode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86)); 
    reg86.intno = 0x10;                   
    reg86.ah = 0x4F;                 
    reg86.al = 0x02;                  
    reg86.bx = submode | BIT(14);     
    if (sys_int86(&reg86)) {   
        printf("Set graphic mode failed\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Sets the frame buffer
 * @param mode graphics mode
 * @param frame_buffer pointer to the frame buffer
 * @return 0 if successful, 1 otherwise
*/
int (graphics_set_frame_buffer)(uint16_t mode, uint8_t** frame_buffer) {
  memset(&mode_info, 0, sizeof(mode_info));
  if(vbe_get_mode_info(mode, &mode_info)) return 1;

  unsigned frame_size = (mode_info.XResolution * mode_info.YResolution * mode_info.BitsPerPixel) / 8;   // cálculo do tamanho do frame buffer, em bytes
  
  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = mode_info.PhysBasePtr; // início físico do buffer
  physic_addresses.mr_limit = physic_addresses.mr_base + frame_size; // fim físico do buffer
  
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)) {
    printf("Physical memory allocation error\n");
    return 1;
  }

  *frame_buffer = vm_map_phys(SELF, (void*) physic_addresses.mr_base, frame_size);
  if (frame_buffer == NULL) {
    printf("Virtual memory allocation error\n");
    return 1;
  }

  return 0;
}

/**
 * @brief Draws a pixel
 * @param x x coordinate
 * @param y y coordinate
 * @param color color to be set
 * @param frame_buffer pointer to the frame buffer
 * @return 0 if successful, 1 otherwise
*/
int (graphics_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer) {
  if(x >= mode_info.XResolution || y >= mode_info.YResolution) return 1;
  
  unsigned BytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;

  unsigned index = (mode_info.XResolution * y + x) * BytesPerPixel;

  memcpy(&frame_buffer[index], &color, BytesPerPixel);

  return 0;
}

/**
 * @brief Draws a line
 * @param x x coordinate
 * @param y y coordinate
 * @param len length of the line
 * @param color color to be set
 * @param frame_buffer pointer to the frame buffer
 * @return 0 if successful, 1 otherwise
*/
int (graphics_draw_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t* frame_buffer) {
  for (unsigned i = 0 ; i < len ; i++)   
    if (graphics_draw_pixel(x+i, y, color, frame_buffer)) return 1;
  return 0;
}

/**
 * @brief Draws a rectangle
 * @param x x coordinate
 * @param y y coordinate
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @param color color to be set
 * @param frame_buffer pointer to the frame buffer
 * @return 0 if successful, 1 otherwise
*/
int (graphics_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t* frame_buffer) {
  for(unsigned i = 0; i < height ; i++)
    if (graphics_draw_line(x, y+i, width, color, frame_buffer)) return 1;
  return 0;
}
