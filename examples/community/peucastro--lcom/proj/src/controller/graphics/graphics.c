#include <lcom/lcf.h>

#include "controller/graphics/graphics.h"

uint32_t(extract_red)(uint32_t color) {
  /* compute a bitmask with RedMaskSize bits set to 1.
   * BIT(n) is assumed to return 2^n, so BIT(n) - 1 gives a mask of n bits */
  uint32_t mask = BIT(vbe_get_mode().RedMaskSize) - 1;

  /* shift the input color to the right by the red field position.
   * then apply the mask to isolate the red component */
  return (color >> vbe_get_mode().RedFieldPosition) & mask;
}

uint32_t(extract_green)(uint32_t color) {
  // create a mask with GreenMaskSize bits set to 1
  uint32_t mask = BIT(vbe_get_mode().GreenMaskSize) - 1;

  // extract the green component by shifting and masking
  return (color >> vbe_get_mode().GreenFieldPosition) & mask;
}

uint32_t(extract_blue)(uint32_t color) {
  // create a mask with BlueMaskSize bits set to 1
  uint32_t mask = BIT(vbe_get_mode().BlueMaskSize) - 1;

  // extract the blue component by shifting and masking
  return (color >> vbe_get_mode().BlueFieldPosition) & mask;
}

int(graphics_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  /* check if the given pixel coordinates (x, y) are outside the screen boundaries
   * note: coordinates are valid if 0 <= x < h_res and 0 <= y < v_res */
  if (x >= vbe_get_h_res() || y >= vbe_get_v_res()) {
    fprintf(stderr, "graphics_draw_pixel: invalid coordinate.");
    return 1;
  }

  /* calculate the memory address of the pixel to be drawn
   each row has h_res pixels, so we move y rows down by doing (y * h_res).
   we then move x pixels right within that row.
   then we multiply by bytes_per_pixel to account for the color depth */
  uint8_t *pixel = vbe_get_video_mem() + (y * vbe_get_h_res() + x) * (vbe_get_bytes_per_pixel());

  // write the given color value to the calculated pixel location in memory
  if (memcpy(pixel, &color, vbe_get_bytes_per_pixel()) == NULL) {
    perror("graphics_draw_pixel: failed to draw pixel.");
    return 1;
  }

  return 0;
}

int(graphics_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  // check if the starting point or the line length goes beyond the screen width
  if (x >= vbe_get_h_res() || y >= vbe_get_v_res() || x + len > vbe_get_h_res()) {
    fprintf(stderr, "graphics_draw_hline: invalid coordinate.");
    return 1;
  }

  /* draw the horizontal line pixel by pixel.
   * we iterate from the start position x up to (x + len), keeping the same y */
  for (uint16_t col = x; col < x + len; col++) {
    // draw each pixel along the horizontal direction
    if (graphics_draw_pixel(col, y, color) != 0) {
      fprintf(stderr, "graphics_draw_hline: failed to draw pixel.");
      return 1;
    }
  }

  return 0;
}

int(graphics_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  /* check if the rectangle’s starting point or size would cause it to exceed screen bounds.
   * ensures both bottom and right edges of the rectangle stay within the screen */
  if (x >= vbe_get_h_res() || y >= vbe_get_v_res() || x + width > vbe_get_h_res() || y + height > vbe_get_v_res()) {
    fprintf(stderr, "graphics_draw_rectangle: invalid dimensions.");
    return 1;
  }

  /* draw the rectangle row by row using horizontal lines.
   * for each vertical position from y to (y + height - 1), draw a full horizontal line */
  for (uint16_t row = y; row < y + height; row++) {
    // draw a horizontal line of 'width' pixels starting at (x, row)
    if (graphics_draw_hline(x, row, width, color) != 0) {
      fprintf(stderr, "graphics_draw_rectangle: failed to draw line.");
      return 1;
    }
  }

  return 0;
}

int(graphics_draw_matrix)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  // calculate the width and height of each rectangle in the grid
  uint16_t width = vbe_get_h_res() / no_rectangles;
  uint16_t height = vbe_get_v_res() / no_rectangles;
  uint32_t color, r, g, b;

  // extract R, G, B components from the base color in case of direct color mode
  uint32_t r_first = extract_red(first);
  uint32_t g_first = extract_green(first);
  uint32_t b_first = extract_blue(first);

  // iterate over all grid positions
  for (uint8_t row = 0; row < no_rectangles; row++) {
    for (uint8_t col = 0; col < no_rectangles; col++) {
      // check if mode is indexed
      if (mode == VBE_MODE_1024x768_8_8_8) {
        // compute the color index for indexed mode
        color = (first + (row * no_rectangles + col) * step) % (1 << vbe_get_mode().BitsPerPixel);
      }
      else {
        // compute RGB components for direct color mode
        r = (r_first + col * step) % (1 << vbe_get_mode().RedMaskSize);
        g = (g_first + row * step) % (1 << vbe_get_mode().GreenMaskSize);
        b = (b_first + (col + row) * step) % (1 << vbe_get_mode().BlueMaskSize);

        // build the final color by shifting components to their proper positions
        color = (r << vbe_get_mode().RedFieldPosition) |
                (g << vbe_get_mode().GreenFieldPosition) |
                (b << vbe_get_mode().BlueFieldPosition);
      }

      // draw the filled rectangle at the appropriate screen location
      if (graphics_draw_rectangle(width * col, height * row, width, height, color) != 0) {
        fprintf(stderr, "graphics_draw_matrix: failed to draw rectangle.");
        return 1;
      }
    }
  }

  return 0;
}

int(graphics_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* check if the given pixel coordinates (x, y) are outside the screen boundaries
   * note: coordinates are valid if 0 <= x < h_res and 0 <= y < v_res */
  if (x >= vbe_get_h_res() || y >= vbe_get_v_res()) {
    fprintf(stderr, "graphics_draw_xpm: invalid coordinates.");
    return 1;
  }

  // declare an xpm_image_t struct to hold metadata (e.g. type, width, height) of the loaded xpm
  xpm_image_t img;

  /* convert the input XPM into a pixel map using the lcf-provided xpm_load function.
   * we use XPM_INDEXED assuming the color mode is indexed for mode 0x105 */
  uint8_t *map = xpm_load(xpm, XPM_INDEXED, &img);

  // check if the image loading failed
  if (map == NULL) {
    fprintf(stderr, "graphics_draw_xpm: xpm map address is null.");
    return 1;
  }

  /* check if the full image would go beyond the screen limits when drawn from (x, y).
   * prevents drawing outside of VRAM which would cause memory errors */
  if (x + img.width > vbe_get_h_res() || y + img.height > vbe_get_v_res()) {
    fprintf(stderr, "graphics_draw_xpm: invalid dimensions.");
    return 1;
  }

  // aux variable to store the color of each pixel
  uint32_t color;

  /* iterate over every pixel in the loaded image.
   * each row is drawn top-down from the starting y coordinate, and each column left-right from x */
  for (uint16_t row = 0; row < img.height; row++) {
    for (uint16_t col = 0; col < img.width; col++) {
      // compute the address of the pixel color in the map (row-major order)
      color = *(map + (row * img.width + col));

      // draw the pixel at the corresponding position on screen
      if (graphics_draw_pixel(x + col, y + row, color) != 0) {
        fprintf(stderr, "graphics_draw_xpm: failed to draw pixel.");
        return 1;
      }
    }
  }

  return 0;
}

int(graphics_clear_screen)(void) {
  // clears out the video mem
  if (memset(vbe_get_video_mem(), 0, vbe_get_h_res() * vbe_get_v_res() * vbe_get_bytes_per_pixel()) == NULL) {
    perror("graphics_clear_screen: failed to clear vram frame.");
    return 1;
  }

  return 0;
}
