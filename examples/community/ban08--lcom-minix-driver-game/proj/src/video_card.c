#include <lcom/lcf.h>
#include "video_card.h"
#include <math.h>

vbe_mode_info_t mode_info; /**< VBE mode information structure */

uint8_t* frame_buffer; /**< Pointer to the video frame buffer */

#include <lcom/lab5.h>
#include <lcom/vbe.h>
#include <lcom/utils.h>

/**
 * @brief Sets the graphic mode using VBE BIOS interrupt (INT 0x10).
 * 
 * @param s_mode The VBE mode to set.
 * @return 0 on success, 1 on failure.
 */
int (set_graphic_mode)(uint16_t s_mode) {
    reg86_t regs;
    int regs_size = sizeof(regs);
    memset(&regs, 0, regs_size);

    regs.intno = 0x10;
    regs.ah = 0x4F; // Function number (VBE)
    regs.al = 0x02; // Set VBE mode
    regs.bx = 1 << 14 | s_mode; // Bit 14 set for linear framebuffer

    if (sys_int86(&regs) == 0) {
        return 0;
    }

    printf("Function set_graphic_mode failed!\n");
    return 1;
}

/**
 * @brief Restores the text mode (80x25 color text).
 * 
 * @return 0 on success, 1 on failure.
 */
int (set_text_mode)() {
    reg86_t reg;
    int regs_size = sizeof(reg);
    memset(&reg, 0, regs_size);

    reg.intno = 0x10;
    reg.ah = 0x00; // Set video mode function
    reg.al = 0x03; // Text mode 80x25 color
    reg.bx = 0x0000;

    if(sys_int86(&reg) == 0) {
        return 0;
    }

    printf("Function set_text_mode failed!\n");
    return 1;
}

/**
 * @brief Maps the physical video memory (frame buffer) to virtual memory.
 *
 * @param mode The VBE mode to get information for.
 * @return 0 on success, 1 on failure.
 */
int (set_frame_buffer)(uint16_t mode){


    size_t info_size = sizeof(mode_info);

    memset(&mode_info, 0, info_size);

    if (vbe_get_mode_info(mode, &mode_info) != 0) {

      return 1;
    }

    uint32_t vertical_res = mode_info.YResolution;

    uint32_t horizontal_res = mode_info.XResolution;

    uint32_t bpp  = (mode_info.BitsPerPixel + 7) / 8;

    uint32_t f_size = horizontal_res * vertical_res * bpp;

    struct minix_mem_range mem_range;

    mem_range.mr_base = mode_info.PhysBasePtr;

    mem_range.mr_limit = mem_range.mr_base + f_size;

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_range) != 0) {
      printf("Error during memory mapping!\n");
      return 1;
    }

    frame_buffer = vm_map_phys(SELF, (void*) mem_range.mr_base, f_size);

    if (frame_buffer != NULL) {

      return 0;
    }

    printf("Error during memory mapping!\n");
    return 1;
}

/**
 * @brief Draws a pixel at the specified coordinates with the given color.
 *
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @param color The color to draw.
 * @return 0 on success, 1 on failure.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if(x >= mode_info.XResolution) {
    return 1;
  }
  if (y >= mode_info.YResolution) {
    return 1;
  }
  
  unsigned bpp = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int idx = (mode_info.XResolution * y + x) * bpp;

  if (memcpy(&frame_buffer[idx], &color, bpp) != NULL) {
    return 0; // success
  }

  return 1;
}

/**
 * @brief Draws a horizontal line starting at (x, y) of given length and color.
 *
 * @param x Starting x-coordinate.
 * @param y The y-coordinate.
 * @param len Length of the line.
 * @param color Color of the line.
 * @return 0 on success, 1 on failure.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned iter = 0 ; iter < len ; ++iter) {
    if (vg_draw_pixel(x + iter, y, color) != 0) {
      return 1;
    }
  }
  return 0;
}


int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned iter = 0 ; iter < len ; ++iter) {
    if (vg_draw_pixel(x, y + iter, color) != 0) {
      return 1;
    }
  }
  return 0;
}

/**
 * @brief Draws a rectangle at (x, y) with specified width, height, and color.
 *
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Color to fill the rectangle.
 * @return 0 on success, 1 on failure.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for(unsigned iter = 0; iter < height ; iter++) {
    if (vg_draw_hline(x, y + iter, width, color) != 0) {
      vg_exit();
      return 1;
    }
  }
  return 0;
}

/**
 * @brief Draws an XPM image at coordinates (x, y).
 *
 * @param xpm The XPM map.
 * @param x The x-coordinate.
 * @param y The y-coordinate.
 * @return 0 on success, 1 on failure.
 */
int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t imagem;
  uint8_t *cores = xpm_load(xpm, XPM_8_8_8_8, &imagem);  

  int alt = imagem.height;
  int larg = imagem.width;

  for (int h_iter = 0 ; h_iter < alt ; ++h_iter) {
    for (int w_iter = 0 ; w_iter < larg ; ++w_iter) {
      if (vg_draw_pixel(x + w_iter, y + h_iter, *cores) != 0) {
        return 1;
      }
      cores++; 
    }
  }
  return 0;
}



/**
 * @brief Constructs a direct color from red, green and blue components.
 *
 * @param red Red component.
 * @param green Green component.
 * @param blue Blue component.
 * @return The combined color value.
 */
uint32_t get_direct_color(uint32_t red, uint32_t green, uint32_t blue) {
  return (red << mode_info.RedFieldPosition) |
         (green << mode_info.GreenFieldPosition) |
         (blue << mode_info.BlueFieldPosition);
}

/**
 * @brief Gets an indexed color based on position and step.
 *
 * @param col Column index.
 * @param row Row index.
 * @param step Step for color increment.
 * @param first First color index.
 * @param n Number of colors per row.
 * @return The calculated color index.
 */
uint32_t get_indexed_color(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
  uint32_t index = row * n + col;

  uint32_t max_color = 1 << mode_info.BitsPerPixel;

  return (first + index * step) % max_color;
}

/**
 * @brief Calculates the red component for a given column.
 *
 * @param col_index Column index.
 * @param step Increment step.
 * @param first_color Base color.
 * @return Red component value.
 */
uint32_t get_red_component(unsigned col_index, uint8_t step, uint32_t first_color) {
  uint32_t base_red = extract_red(first_color);

  uint32_t max_red = 1 << mode_info.RedMaskSize;

  return (base_red + col_index * step) % max_red;
}

/**
 * @brief Calculates the green component for a given row.
 *
 * @param row_index Row index.
 * @param step Increment step.
 * @param first_color Base color.
 * @return Green component value.
 */
uint32_t get_green_component(unsigned row_index, uint8_t step, uint32_t first_color) {
  uint32_t base_green = extract_green(first_color);

  uint32_t max_green = 1 << mode_info.GreenMaskSize;

  return (base_green + row_index * step) % max_green;
}

/**
 * @brief Calculates the blue component for given column and row.
 *
 * @param col_index Column index.
 * @param row_index Row index.
 * @param step Increment step.
 * @param first_color Base color.
 * @return Blue component value.
 */
uint32_t get_blue_component(unsigned col_index, unsigned row_index, uint8_t step, uint32_t first_color) {
  uint32_t base_blue = extract_blue(first_color);
  uint32_t max_blue = 1 << mode_info.BlueMaskSize;
  return (base_blue + (row_index + col_index) * step) % max_blue;
}

/**
 * @brief Extracts the red component from a color.
 *
 * @param color Color value.
 * @return Red component.
*/
uint32_t extract_red(uint32_t color) {
  uint32_t mask = (1 << mode_info.RedMaskSize) - 1;
  return (color >> mode_info.RedFieldPosition) & mask;
}

/**
* @brief Extracts the green component from a color.
* 
* @param color Color value.
* @return Green component.
*/
uint32_t extract_green(uint32_t color) {
  uint32_t mask = (1 << mode_info.GreenMaskSize) - 1;
  return (color >> mode_info.GreenFieldPosition) & mask;
}

/**
* @brief Extracts the blue component from a color.
* 
* @param color Color value.
* @return Blue component.
*/
uint32_t extract_blue(uint32_t color) {
  uint32_t mask = (1 << mode_info.BlueMaskSize) - 1;
  return (color >> mode_info.BlueFieldPosition) & mask;
}

/**
* @brief Clears the entire screen with a specified color.
* @param color The color to fill the screen with.
**/
void clear_screen(uint32_t color) {
    uint32_t* buffer = (uint32_t*) frame_buffer;
    size_t total_pixels = mode_info.XResolution * mode_info.YResolution;

    for (size_t i = 0; i < total_pixels; i++) {
        buffer[i] = color;
    }
}
