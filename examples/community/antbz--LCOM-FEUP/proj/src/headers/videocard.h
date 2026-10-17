#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "vbe_defs.h"
#include "math.h"
#include "line.h"

/**
 * @file videocard.h
 * @brief Functions to set the videocard and draw on the screen
 * 
 * File containing the functions to subscribe and handle the mouse's interruptions.
*/

/**
 * @brief Sets the VBE mode.
 *  
 * @param mode the mode to set VBE to.
 * @return 0 if the operation was successful, 1 otherwise
 */
int set_vbe_mode(uint16_t mode);

/**
 * @brief Auxiliary function to get the number of bytes by the number of bits.
 *  
 * Divides the number of bits by eight and returns the ceiling of the result.
 * 
 * @param bits number of bits
 * @return corresponding number of bytes
 */
int bitsToBytes(uint8_t bits);

/**
 * @brief Maps video memory for specified mode
 * 
 * @param mode the mode to map video memory to.
 */
void map_video_mem(uint16_t mode);

/**
 * @brief Refreshes the video card buffer
 * 
 */
void refresh_buffer();

/**
 * @brief Sets a pixel to a specified color
 * 
 * @param color the color to change the pixel to.
 * @param x x coordinate of the pixel
 * @param y y coordinate of the pixel
 * @return 0 if the operation was successful, 1 otherwise
 */
int set_pixel(uint32_t color, uint16_t x, uint16_t y);

/**
 * @brief Draws a horizontal line on the screen
 * 
 * @param color the color to draw the line in.
 * @param x x coordinate of the pixel where to start drawing the line
 * @param y y coordinate of the pixel where to start drawing the line
 * @param len lenght of the line
 * @return 0 if the operation was successful, 1 otherwise
 */
int draw_hline(uint32_t color, uint16_t x, uint16_t y, uint16_t len);

/**
 * @brief Draws a rectangle on the screen
 * 
 * Draws a rectangle by drawing several lines
 * 
 * @param color the color to draw the rectangle in.
 * @param x x coordinate of the pixel where to start drawing the rectangle
 * @param y y coordinate of the pixel where to start drawing the rectangle
 * @param width width of the rectangle
 * @param height height of the rectangle
 * @return 0 if the operation was successful, 1 otherwise
 */
int draw_rectangle(uint32_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height);


/**
 * @brief Draws a line on the screen
 * 
 * @param color the color to draw the line in.
 * @param line 
 * @return 0 if the operation was successful, 1 otherwise
 */
int draw_line(uint32_t color, line_t* line);


/**
 * @brief Draws a number on the screen.
 * 
 * @param num integer of the number to be drawn
 * @param x x coordinate of the pixel where to start drawing
 * @param y y coordinate of the pixel where to start drawing
 * @param right
 * @param digits xpm_image_t* with the xpms of the digits to be used
 * @return 0 if the operation was successful, 1 otherwise
 */
void draw_number(int num, uint16_t x, uint16_t y, bool right, xpm_image_t* digits);

/**
 * @brief Draws a xpm image on the screen
 * 
 * @param x x coordinate of the pixel where to start drawing
 * @param y y coordinate of the pixel where to start drawing
 * @param img xpm_image_t with the xpm to be drawn
 * @return 0 if the operation was successful, 1 otherwise
 */
int draw_xpm(uint16_t x, uint16_t y, xpm_image_t img);

/**
 * @brief Clears a xpm image from the screen
 * 
 * @param x x coordinate of the pixel where the image starts
 * @param y y coordinate of the pixel where the image starts
 * @param img xpm_image_t with the xpm to be cleared
 * @return 0 if the operation was successful, 1 otherwise
 */
int clear_xpm(uint16_t x, uint16_t y, xpm_image_t img);

/**
 * @brief Checks if the coordinates of the pixel are inside the screen
 * 
 * Checks if the x and y coordinates are larger than 0 and smaller than the screen's resolution
 * 
 * @param x x coordinate of the pixel to check
 * @param y y coordinate of the pixel to check
 * @return true if the pixel is inside the screen, false otherwise
 */
bool in_screen(uint16_t x, uint16_t y);

/**
 * @brief Clears the screen
 * 
 * Sets every pixel on the screen to black
 */
void clear_screen();

/**
 * @brief Clears the screen with a background
 * 
 * Draws the background on the screen, clearing every other image that was previously showing 
 * on the screen
 * 
 * @param background_img xpm_image_t with the xpm of the background
 */
void clear_screen_background(xpm_image_t background_img);

/**
 * @brief Gets the screen's y resolution
 * 
 * @return Value of the screen's y resolution
 */
unsigned get_y_res();

/**
 * @brief Gets the screen's x resolution
 * 
 * @return Value of the screen's x resolution
 */
unsigned get_x_res();
