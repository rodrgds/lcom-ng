#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>
/** @defgroup graphics graphics
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

/**
 * @brief Gets information about VBE mode
 * @param mode VBE mode
 * @param vbe information about the mode is stored here
 * @return 0 if no errors ocurred, 1 otherwise
 */
int (vbe_mode_info)(uint16_t mode, vbe_mode_info_t *vbe);

/**
 * @brief Initalizes the VBE
 * @param mode VBE mode initialized
 * @return 0 if no errors ocurred, 1 otherwise
 */
int (vg_init_function)(uint16_t mode);

/**
 * @brief Draws a pixel
 * @param x Position in the x axis
 * @param y Position in the y axis
 * @param color color of the pixel
 */
void (draw_pixel)(uint16_t x, uint16_t y, const void *color);

/**
 * @brief Draws an horizontal line
 * @param x Position in the x axis
 * @param y Position in the y axis
 * @param len horizontal line length
 * @param color Horizonal line pixel color
 * @return 0 if no errors ocurred, 1 otherwise
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle
 * @param x Position in the x axis
 * @param y Position in the y axis
 * @param width Rectangle width
 * @param height Rectangle height
 * @param color Rectangle pixel color
 * @return 0 if no errors ocurred, 1 otherwise
 */
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a patter with a certain number of rectangles
 * @param mode VBE mode
 * @param no_rectangles Number of rectangles to be drawn
 * @param first First rectangle to be drawn
 * @param step Step of rectangles to be drawn
 * @return 0 if no errors ocurred, 1 otherwise
 */
int (draw_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief Draws an xpm
 * @param x Position in the x axis
 * @param y Position in the x axis
 * @param img image of the xpm
 * @return 0 if no errors ocurred, 1 otherwise
 */
int (draw_xpm)(uint16_t x, uint16_t y, xpm_image_t img);

/**
 * @brief Draws an xpm with a determinated rotation and size
 * @param x Position in the x axis
 * @param y Position in the x axis
 * @param img image of the xpm
 * @param teta rotation angle of the xpm in degrees
 * @param size size multipier of xpm (should be lower than 1 in order to work properly)
 * @return 0 if no errors ocurred, 1 otherwise
 */
int (draw_xpm_resize)(uint16_t x, uint16_t y, xpm_image_t img, double teta, double size);

/**
 * @brief Draws an xpm with a determinated rotation
 * @param x Position in the x axis
 * @param y Position in the x axis
 * @param img image of the xpm
 * @param teta rotation angle of the xpm in degrees
 * @return 0 if no errors ocurred, 1 otherwise
 */
int (draw_xpm_rotated)(uint16_t x, uint16_t y, xpm_image_t img, double teta);

/**
 * @brief Enables double buffering
 * @return 0 if no errors ocurred, 1 otherwise
 */
int (double_buffering)();

/**
 * @brief Frees the Bufffer memory allocation
 * @return 0 if no errors ocurred, 1 otherwise
 */
int (freeBuffering)();

/**
 * @brief Draws a pixel in the video memory
 * @param x Position in the x axis
 * @param y Position in the x axis
 * @param color color of the xpm
 * @return 0 if no errors ocurred, 1 otherwise
 */
void (draw_pixel_mem)(uint16_t x, uint16_t y, const void *color);

/**
 * @brief Draws an xpm in the video memory
 * @param x Position in the x axis
 * @param y Position in the x axis
 * @param img image of the xpm
 * @return 0 if no errors ocurred, 1 otherwise
 */
int (draw_xpm_mem)(uint16_t x, uint16_t y, xpm_image_t img);
