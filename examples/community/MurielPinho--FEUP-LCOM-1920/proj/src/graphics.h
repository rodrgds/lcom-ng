#include <lcom/lcf.h>
#ifndef GRAPHICS_H
#define GRAPHICS_H


/** @defgroup Graphics Graphics
 * @{
 * Functions to control the project Graphics 
 */

/**
 * @brief Function to draw a rectangle in the screen
 * @param mode uint16_t for the graphical mode that we want the info
 * @param vmi_p vbe_mode_info_t* pointer to destination of the information
 * @return Returns 0 if successfull
 */
int vbeGetModeInfo(uint16_t mode, vbe_mode_info_t *vmi_p);
/**
 * @brief Function to set the graphical mode
 * @param mode uint16_t for the graphical mode to be set 
 * @return Returns 0 if successfull
 */
int vg_set_mode(uint16_t mode);
/**
 * @brief Function to draw a rectangle in the screen
 * @param x uint16_t for the initial x coordinate of the rectangle 
 * @param y uint16_t for the initial y coordinate of the rectangle 
 * @param width uint16_t for the width of the rectangle 
 * @param height uint16_t for the height of the rectangle
 * @param color uint32_t for the RGB color of the rectangle 
 * @return Returns 0 if successfull
 */
int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
/**
 * @brief Function to initialize the graphic mode
 * @param mode uint16_t for the graphical mode to be used 
 * @return Returns the video buffer address
 */
void *(vg_init)(uint16_t mode);
/**
 * @brief Function to flip the video memory buffer
 */
void flipBuffer();
/**
 * @brief Function to draw a line between two points
 * @param xi uint16_t for the initial x coordinate of the line 
 * @param yi uint16_t for the initial y coordinate of the line 
 * @param xf uint16_t for the final x coordinate of the line 
 * @param yf uint16_t for the final y coordinate of the line
 * @param color uint32_t for the RGB color of the line 
 */
void drawLine(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color);
/**
 * @brief Function to fill a screen pixel with a color
 * @param x int for the x coordinate for the pixel 
 * @param y int for the y coordinate for the pixel 
 * @param color uint32_t for the RGB color of the pixel 
 */
void fillPixel(int x, int y, uint32_t color);
/**
 * @brief Function to clear the graphics screen
 */
void clearScreen();
/**
 * @}
 */
#endif /*GRAPHICS_H*/
