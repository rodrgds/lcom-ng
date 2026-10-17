#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

int (graphics_init)(uint16_t mode);

/**
 * @brief Maps the vram according to a given mode.
 * @param mode is the mode.
 * 
 */
int (map_vram)(uint16_t mode);

/**
 * @brief Initializes graphics mode.
 * 
 */
int (initialize_graphics_mode)(uint16_t mode);

/**
 * @brief Checks if a rectangle exceed the screen limits of a given mode.
 * @param mode is the mode.
 * @param x is the inicial horizontal position of the rectangle.
 * @param y is the inicial vertical position of the rectangle.
 * @param width is the with of the rectangle.
 * @param height is the height of the rectangle.
 * 
 */
int (verify_screen_limits)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Draws a pixel given its position and color.
 * @param x is the horizontal position.
 * @param y is the vertical position.
 * @param color is the color of the pixel.
 * 
 */
void (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line.
 * @param x is the horizontal position.
 * @param y is the vertical position.
 * @param len is the lenght of the line.
 * @param color is the color of the line.
 */
void (draw_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle.
 * @param x is the inicial horizontal position of the rectangle.
 * @param y is the inicial vertical position of the rectangle.
 * @param width is the with of the rectangle.
 * @param height is the height of the rectangle.
 * @param color is the color of the rectangle.
 * 
 */
void (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

uint8_t* (initialize_pixmap)(xpm_map_t xpm, xpm_image_t* img_info);

void (draw_xpm)(uint8_t* pixmap, xpm_image_t img_info, uint16_t x, uint16_t y);

void (move_xpm)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint16_t* x, uint16_t* y, int16_t speed);

#endif
