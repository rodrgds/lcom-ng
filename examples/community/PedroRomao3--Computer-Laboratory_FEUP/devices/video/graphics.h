/**
 * @file graphics.h
 * @brief ...
 * @author musta
 * @date 16/04/2023
 */

#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include <math.h>
#include "graphics_constance.h"
#include "settings.h"
#include "helper.h"

vbe_mode_info_t mode_info;
uint8_t* frame_buffer;      // ponho ele na memoria
uint8_t* frame_buffer_copy; // atualizo ele
uint32_t total_frame_buffer_size;
unsigned int pixel_byte_count;

/**
 @brief Changes the graphics mode to the specified mode.
This function changes the graphics mode to the specified mode by initializing the memory, creating a buffer copy, and switching the VBE mode.
 @param graphics_mode The desired graphics mode to switch to.
 @return 0 indicating successful mode change, non-zero value in case of an error.
*/
int (change_graphics_mode)(uint16_t mode);

/**
 @brief Draws a pixel with a specified color in the screen buffer at the given position.
 @param x The horizontal position of the pixel to draw.
 @param y The vertical position of the pixel to draw.
 @param color The color of the pixel to draw, represented as a 32-bit integer.
 @return 0 on success, 1 if the specified x coordinate is outside the screen, 2 if the specified y coordinate is outside the screen, or an error code from memcpy if it fails.
This function draws a single pixel with the specified color at the given position (x, y) in the screen buffer. It first checks if the coordinates are inside the screen dimensions, and then calculates the memory index to write the pixel color to. The function returns an error if memcpy fails to copy the color value to the screen buffer. The complexity of this function is O(1).
*/
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 @brief Clears the buffer by setting all pixels to white.
 @details The function uses memset to set all pixels in the frame buffer
to the white color value of 0xFFFFFF.
 @note The function assumes that the frame buffer has been initialized and
its size is stored in the global variable total_frame_buffer_size.
 @param None.
 @return None.
 @complexity The function has a constant time complexity of O(1) since it
performs a single operation regardless of the size of the frame buffer.
*/
void clearBuffer();

/**
 @brief Copies the contents of the back buffer (frame_buffer_copy) to the front buffer (frame_buffer), effectively swapping them.
This function simply uses the memcpy function to copy the contents of the back buffer to the front buffer. The front buffer is then displayed on the screen, showing the updated graphics.
 @return void
 @note This function assumes that the back buffer has already been updated with the desired graphics, and that clearBuffer() has been called to initialize the back buffer to the desired background color.
 @note This function does not clear the back buffer, so it is the responsibility of the programmer to ensure that the back buffer is properly cleared before drawing new graphics.
 @note This function has a time complexity of O(n), where n is the size of the frame buffer (in bytes).
*/
void swap_buffers();

/**
 @brief Draws a horizontal line with a given color on the screen.
 @param x The x coordinate of the starting point of the line.
 @param y The y coordinate of the starting point of the line.
 @param len The length of the line.
 @param color The color of the line.
 @return 0 upon success, non-zero otherwise.
This function draws a horizontal line on the screen starting at the point (x, y) with a given length and color.
If the x coordinate is greater than the screen's XResolution, the function returns 1.
If the y coordinate is greater than the screen's YResolution, the function returns 2.
The function uses the vg_draw_pixel() function to draw each pixel of the line.
The function returns 0 upon success, and non-zero otherwise.
 @note This function assumes that the video mode has been set using vg_init().
 @warning This function assumes that the frame buffer is mapped to the process' address space.
 @see vg_draw_pixel()
*/
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);


/**
 @brief Draws a vertical line on the screen.
This function draws a vertical line of a given length and color at a specified location on the screen.
It uses the vg_draw_pixel function to draw each pixel of the line.
 @param x The x-coordinate of the start of the line.
 @param y The y-coordinate of the start of the line.
 @param len The length of the line.
 @param color The color of the line.
 @return 0 if the operation is successful, 1 otherwise.
*/
int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 @brief Draws a rectangle with a specified color
 @param x The x coordinate of the top left corner of the rectangle
 @param y The y coordinate of the top left corner of the rectangle
 @param width The width of the rectangle
 @param height The height of the rectangle
 @param color The color of the rectangle
 @return int Returns 0 upon success, non-zero otherwise
This function draws a rectangle with a specified color on the screen using the VGA graphics mode.
It uses the vg_draw_hline() function to draw each horizontal line of the rectangle.
If the drawing of any horizontal line fails, the function returns a non-zero value and exits.
The complexity of this function is O(height * width).
*/
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 @brief Switches the VBE mode to the specified graphics mode.
This function switches the VBE mode to the specified graphics mode by invoking the BIOS interrupt with the appropriate parameters.
 @param graphics_mode The desired graphics mode to switch to.
 @return 0 indicating successful mode switch, non-zero value in case of an error.
*/
int switch_vbe_mode(uint16_t graphics_mode); 

/**
 @brief Initializes the memory for the specified graphics mode.
This function initializes the memory for the specified graphics mode by retrieving mode information, calculating the frame buffer size,
allocating physical and virtual memory, and mapping the physical memory to virtual memory.
 @param graphics_mode The graphics mode for which to initialize the memory.
 @return 0 indicating successful memory initialization, non-zero value in case of an error.
*/
int initialize_memory(uint16_t graphics_mode);

/**
 @brief Creates a copy of the frame buffer.
This function allocates memory for a duplicate frame buffer and initializes it with white pixels.
 @return A pointer to the duplicate frame buffer, or NULL in case of an allocation error.
*/
uint8_t* create_buffer_copy();

#endif /* __GRAPHICS_H */
