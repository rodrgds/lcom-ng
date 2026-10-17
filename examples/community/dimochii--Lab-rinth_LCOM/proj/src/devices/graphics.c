#include "graphics.h"

/** @defgroup graphics graphics
 * @{
 *
 * Functions for using the MINIX graphic card

 */

/**
 * @brief Sets the graphics mode using VBE.
 * 
 * @param mode The graphics mode to set.
 * @return int Returns 0 on success, 1 on failure.
 */
int (graphic_mode)(uint16_t mode) {
  reg86_t r;
  memset(&r, 0, sizeof(r));

  r.intno = 0x10;
  r.ax = 0x4F02;
  r.bx = BIT(14) | mode;
  
  if (sys_int86(&r) != 0) {
    printf("sys_int86 failed for graphic mode\n");
    return 1;
  }

  return 0;
}

/**
 * @brief Initializes the frame buffer for graphics mode.
 * 
 * @param mode The graphics mode to initialize.
 * @return int Returns 0 on success, 1 on failure.
 */
int (initialize_frame_buffer)(uint16_t mode) {
  if (vbe_get_mode_info(mode, &mode_info) != 0) 
    return 1;

  frame.res_x = mode_info.XResolution;
  frame.res_y = mode_info.YResolution;

  frame.bytes_pixel = (mode_info.BitsPerPixel + 7) / 8; 
  frame.size = mode_info.XResolution * mode_info.YResolution * frame.bytes_pixel;
  unsigned int size_total = 2 * frame.size;
  

  int r;
  struct minix_mem_range mr;
  mr.mr_base = mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + size_total;

  if( 0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    printf("sys_privctl failed: %d\n", r);
    return 1;
  }


  frame.buffer1 = vm_map_phys(SELF, (void *)mr.mr_base, size_total);
  if (frame.buffer1 == MAP_FAILED) {
    printf("Mapping video memory failed\n");
    return 1;
  }
  frame.buffer2 = frame.buffer1 + frame.size;

  display_buffer = 1; draw_buffer = frame.buffer2;

  return 0;
}


/**
 * @brief Sets the display start address for double buffering.
 * 
 * @param buffer The buffer to set as the active display.
 */
void (set_display_start)(int buffer) {
    reg86_t r;
    memset(&r, 0, sizeof(r));

    r.intno = 0x10;
    r.ax = 0x4F07;
    r.bx = 0x00;
    r.cx = 0x00;

    if (buffer == 1) {
      r.dx = 0;
    } else {    
      r.dx = frame.res_y;
    }
    
    if (sys_int86(&r) != 0) {
        printf("sys_int86() failed for set display start\n");
    }
}

/**
 * @brief Updates the double buffering mechanism by flipping frames.
 */
void (update_flip_frames)() { 
  if (display_buffer == 1) {
    set_display_start(2);
    display_buffer = 2;
    draw_buffer = frame.buffer1;
  } else {
    set_display_start(1);
    display_buffer = 1;
    draw_buffer = frame.buffer2;
  }

  clear_drawing();
}

/**
 * @brief Clears the drawing buffer.
 */
void (clear_drawing)() {
  memset(draw_buffer, 0, frame.size);
}



/**
 * @brief Draws a pixel of a specified color at the given coordinates.
 * 
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 * @param buffer The buffer to draw the pixel on.
 * @return int Returns 0 on success, 1 if the coordinates are out of bounds.
 */
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* buffer) {
  if (x < 0 || y < 0 || x >= frame.res_x || y >= frame.res_y) return 0;

  unsigned int index = (y * frame.res_x + x) * frame.bytes_pixel;

  if (memcpy(&buffer[index], &color, frame.bytes_pixel) == NULL) 
    return 1;

  return 0;
}

/**
 * @brief Draws the background onto the drawing buffer.
 * 
 * @return int Returns 0 on success, 1 on failure.
 */
int (draw_background)() {
  if (memcpy(draw_buffer, bg_buffer, frame.size) == NULL)
    return 1;
  
  return 0;
}

/**
 * @brief Initializes the background buffers.
 */
void (initialize_buffers)() {
  bg_buffer = malloc(frame.size);
}

/**
 * @brief Frees the allocated background buffers.
 */
void (free_buffers)() {
  free(bg_buffer);
}

