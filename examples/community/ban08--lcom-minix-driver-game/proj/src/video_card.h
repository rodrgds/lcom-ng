#ifndef VIDEO_CARD_H
#define VIDEO_CARD_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <lcom/lab5.h>

#include <lcom/lab5.h>
#include <lcom/vbe.h>
#include <lcom/utils.h>


// Video mode info structure (VBE mode info)
extern vbe_mode_info_t mode_info;

// Pointer to the mapped frame buffer
extern uint8_t* frame_buffer;

/**
 * @brief Sets the video mode to the given VBE graphics mode.
 * 
 * @param s_mode The VBE mode number
 * @return 0 on success, non-zero otherwise
 */
int set_graphic_mode(uint16_t s_mode);

/**
 * @brief Sets the video mode to text mode (usually mode 03h).
 * 
 * @return 0 on success, non-zero otherwise
 */
int set_text_mode(void);

/**
 * @brief Maps the frame buffer for the given VBE mode.
 * 
 * @param mode The VBE mode to map frame buffer for
 * @return 0 on success, non-zero otherwise
 */
int set_frame_buffer(uint16_t mode);

/**
 * @brief Draws a pixel at (x, y) with the specified color.
 * 
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Color value (format depends on mode)
 * @return 0 on success, non-zero otherwise
 */
int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line starting at (x, y) with length `len` and color.
 * 
 * @param x Starting X coordinate
 * @param y Y coordinate
 * @param len Length of the line in pixels
 * @param color Color value
 * @return 0 on success, non-zero otherwise
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle at (x, y) with the specified width, height, and color.
 * 
 * @param x X coordinate
 * @param y Y coordinate
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color value
 * @return 0 on success, non-zero otherwise
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);


/**
 * @brief Prints an XPM image at position (x, y).
 * 
 * @param xpm XPM map to print
 * @param x X coordinate
 * @param y Y coordinate
 * @return 0 on success, non-zero otherwise
 */
int print_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);



/**
 * @brief Computes the direct color value from red, green, blue components.
 * 
 * @param red Red component
 * @param green Green component
 * @param blue Blue component
 * @return Combined color value
 */
uint32_t get_direct_color(uint32_t red, uint32_t green, uint32_t blue);

/**
 * @brief Computes an indexed color based on grid position and color step.
 * 
 * @param col Column index
 * @param row Row index
 * @param step Color step size
 * @param first First color component value
 * @param n Number of colors
 * @return Indexed color value
 */
uint32_t get_indexed_color(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);

/**
 * @brief Extracts red component based on column index and step.
 */
uint32_t get_red_component(unsigned col_index, uint8_t step, uint32_t first_color);

/**
 * @brief Extracts green component based on row index and step.
 */
uint32_t get_green_component(unsigned row_index, uint8_t step, uint32_t first_color);

/**
 * @brief Extracts blue component based on column and row index and step.
 */
uint32_t get_blue_component(unsigned col_index, unsigned row_index, uint8_t step, uint32_t first_color);

/**
 * @brief Extracts red component from a combined color value.
 */
uint32_t extract_red(uint32_t color);

/**
 * @brief Extracts green component from a combined color value.
 */
uint32_t extract_green(uint32_t color);

/**
 * @brief Extracts blue component from a combined color value.
 */
uint32_t extract_blue(uint32_t color);

/**
 * @brief Clears the screen with the specified color.
 * 
 * @param color Color to fill the screen with
 */
void clear_screen(uint32_t color);

#endif // VIDEO_CARD_H
