#include "gpu.h"
#include <lcom/lcf.h>
#include <math.h>

int(set_graphics_mode)(uint16_t mode) {
  // dada pelo stor
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86));

  reg86.intno = 0x10;
  reg86.ax = VBE_CALL_FUNCTION_02;
  reg86.bx = mode | LINEAR_FRAME_BUFFER;

  if (sys_int86(&reg86) != 0) {
    printf("Error in GPU - Could not set graphics mode");
    return 1;
  }
  return 0;
}

int(build_frame_buffer)(uint16_t mode) {

  memset(&vbe_mode_info, 0, sizeof(vbe_mode_info_t)); // fills vbe_mode_info with 0s

  if (vbe_get_mode_info(mode, &vbe_mode_info) != 0) { // fills vbe_mode_info with the info of the mode
    printf("Error in GPU - Get mode info failed!");
    return 1;
  }

  x_res = vbe_mode_info.XResolution;
  y_res = vbe_mode_info.YResolution;
  bytes_per_pixel = (ceil(vbe_mode_info.BitsPerPixel / 8));

  frame_size = x_res * y_res * bytes_per_pixel; // calculates the size of the frame buffer in bytes. ceil() is used to round up the result of the division

  struct minix_mem_range mem_range;
  mem_range.mr_base = (phys_bytes) vbe_mode_info.PhysBasePtr;               // sets the base of the memory range to the physical address of the frame buffer
  mem_range.mr_limit = mem_range.mr_base + x_res * y_res * bytes_per_pixel; // sets the limit of the memory range to the size of the frame buffer

  int r;
  if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_range))) // gives the process permission to access the memory range
    panic("Error in GPU - sys_privctl (ADD_MEM) failed: %d\n", r);

  frame_memory = vm_map_phys(SELF, (void *) mem_range.mr_base, frame_size); // maps the physical memory range to the virtual memory range of the process
  frame_buffer = malloc(frame_size);

  if (frame_buffer == MAP_FAILED) // checks if the mapping was successful
    panic("Error in GPU - Could not map video memory");

  return 0;
}

void(clear_buffer)() {
  memset(frame_buffer, 0, frame_size);
}

void(swap_buffer)() {
  memcpy((void *) frame_memory, frame_buffer, frame_size);
}

int(draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  // printf("color: %d", color);
  if (x >= x_res || y >= y_res || x < 0 || y < 0) { // checks if the coordinates are valid
    printf("Error in GPU - Pixel outside bounds");
    return 1;
  }

  // unsigned int bytes_per_pixel = ((vbe_mode_info.BitsPerPixel+7)/8); //calculates the number of bytes per pixel

  uint32_t index = (y * x_res + x) * bytes_per_pixel;                  // calculates the index of the pixel in the frame buffer

  if (memcpy(&frame_buffer[index], &color, bytes_per_pixel) != NULL) { // copies the color to the frame buffer
    return 0;
  }
  printf("Error in GPU - could not do memcpy() in draw_pixel()");
  return 1;
}

int(draw_hor_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++) {
    if (draw_pixel(x + i, y, color) != 0) {
      printf("Error in GPU - could not do draw_pixel() in draw_hor_line()");
      return 1;
    }
  }
  return 0;
}

int(draw_rect)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (draw_pixel(x + j, y + i, color) != 0) {
        printf("Error in GPU - could not do draw_pixel()");
        return 1;
      }
    }
  }
  return 0;
}

//=========================== SPRITE FUNCTION(S) =======================================================================
int(draw_xpm)(xpm_image_t img, uint16_t x, uint16_t y, int flipped) {
  for (int i = 0; i < img.height; i++) {
    if (!flipped) {
      for (int j = 0; j < img.width; j++) {
        if (((uint32_t *) img.bytes)[i * img.width + j] == TRANSPARENCY_COLOR_8_8_8_8)      // checks if the pixel is transparent
          continue;                                                                         // skips the pixel if it is transparent
        else {
          if (draw_pixel(x + j, y + i, ((uint32_t *) img.bytes)[i * img.width + j]) != 0) { // draws the pixel
            printf("Error in GPU: could not draw_pixel() in draw_xpm()");
            return 1;
          }
        }
      }
    }
    else if (flipped) {
      for (int j = img.width -1; j >= 0; j--) {
        if (((uint32_t *) img.bytes)[i * img.width + j] == TRANSPARENCY_COLOR_8_8_8_8)      // checks if the pixel is transparent
          continue;                                                                         // skips the pixel if it is transparent
        else {
          if (draw_pixel(x + (img.width -1 - j), y + i, ((uint32_t *) img.bytes)[i * img.width + j]) != 0) { // draws the pixel
            printf("Error in GPU: could not draw_pixel() in draw_xpm()");
            return 1;
          }
        }
      }
    }
  }

  return 0;
}
