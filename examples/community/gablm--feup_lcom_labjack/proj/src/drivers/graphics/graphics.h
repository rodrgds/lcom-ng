#ifndef _LCOM_DRIVER_GRAPHICS_H_
#define _LCOM_DRIVER_GRAPHICS_H_

#include <lcom/lcf.h>
#include "iVBE.h"

/**
 * @defgroup graphics Graphics
 * @ingroup drivers
 * @{
 *
 * @brief Functions and data structures related to graphics handling.
 */

/**
 * @brief Enum representing the color mode of the graphics.
 */
typedef enum {
	indexed, /**< @brief Indexed color mode */
	direct,  /**< @brief Direct color mode */
	unknown  /**< @brief Unknown color mode */
} colormode_t;

/**
 * @brief Struct containing information about the graphics mode.
 */
typedef struct {
	uint8_t *frame_buffer[3]; /**< @brief Array of pointers to the frame buffers */
	bool needs_redraw;        /**< @brief Flag indicating if a redraw is needed */
	int selectedNum;          /**< @brief Selected number for the buffer */

	unsigned x_res;           /**< @brief Horizontal resolution */
	unsigned y_res;           /**< @brief Vertical resolution */
	unsigned bits_per_pixel;  /**< @brief Number of bits per pixel */
	unsigned bytes_per_pixel; /**< @brief Number of bytes per pixel */

	unsigned red_mask_size;   /**< @brief Size of the red color mask */
	unsigned red_field_pos;   /**< @brief Position of the red field */
	unsigned green_mask_size; /**< @brief Size of the green color mask */
	unsigned green_field_pos; /**< @brief Position of the green field */
	unsigned blue_mask_size;  /**< @brief Size of the blue color mask */
	unsigned blue_field_pos;  /**< @brief Position of the blue field */

	bool direct_color;        /**< @brief Flag indicating if direct color mode is used */
} gph_t;


/**
 * @brief Initializes the video graphics mode.
 * @return 0 if successful, non-zero otherwise
 */
int	vg_init_mode();

/**
 * @brief Enters the specified graphic mode.
 * @param mode The mode to enter
 * @return 0 if successful, non-zero otherwise
 */
int	vg_enter_graphic_mode(uint16_t mode);

/**
 * @brief Maps the memory for the specified mode.
 * @param mode The mode to map the memory for
 * @return 0 if successful, non-zero otherwise
 */
int	vg_map_memory(uint16_t mode);

/**
 * @brief Draws a rectangle on the screen.
 * @param x The x-coordinate of the top-left corner
 * @param y The y-coordinate of the top-left corner
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param color The color of the rectangle
 * @return 0 if successful, non-zero otherwise
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, 
					uint16_t height, uint32_t color);

/**
 * @brief Draws a pixel on the screen.
 * @param x The x-coordinate of the pixel
 * @param y The y-coordinate of the pixel
 * @param color The color of the pixel
 * @return 0 if successful, non-zero otherwise
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line on the screen.
 * @param x The starting x-coordinate
 * @param y The y-coordinate of the line
 * @param len The length of the line
 * @param color The color of the line
 * @return 0 if successful, non-zero otherwise
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a vertical line on the screen.
 * @param x The starting x-coordinate
 * @param y The y-coordinate of the line
 * @param len The length of the line
 * @param color The color of the line
 * @return 0 if successful, non-zero otherwise
 */
int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a border around a rectangle on the screen.
 * @param x The x-coordinate of the top-left corner
 * @param y The y-coordinate of the top-left corner
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param color The color of the border
 * @param thickness The thickness of the border
 * @return 0 if successful, non-zero otherwise
 */
int vg_draw_border(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t thickness);

/**
 * @brief Gets the graphics information.
 * @return A gph_t struct containing the graphics information
 */
gph_t vg_get_info();

/**
 * @brief Flips the buffers to update the screen.
 * @return 0 if successful, non-zero otherwise
 */
int (vg_flip)();

/**
 * @brief Gets the width of the screen.
 * @return The width of the screen in pixels
 */
unsigned vg_get_width();

/**
 * @brief Gets the height of the screen.
 * @return The height of the screen in pixels
 */
unsigned vg_get_height();

/**
 * @brief Sets the flag indicating that a redraw is needed.
 */
void vg_set_redraw();

/**
 * @brief Checks if a redraw is needed.
 * @return 1 if a redraw is needed, 0 otherwise
 */
int vg_has_redraw();

/** @} */

#endif
