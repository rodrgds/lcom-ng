#pragma once
#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>
#include "define.h"
#include "graphics_card_macros.h"

/**
 * @brief This funtion initiates graphics mode (GUI instead of CLI)
 * @param mode The VBE mode in which the graphics will be configured
 */
void *(vg_init)(uint16_t mode);

/**
 * @brief This funtion changes graphics mode
 * @param mode The VBE mode in which the graphics will be changed
 */
int(change_mode)(uint16_t mode);

/**
 * @brief This function allocates memomry necessary for the graphics and maps it in an array
 * @param mode The VBE mode in which the graphics will be configured
 * @return Return 0 in case of success, 1 if otherwise
 */
int(map_virtual_machine)(uint16_t mode);

/**
 * @brief This function prints a pixels to the display
 * @param x the x-axis position of the pixel's position
 * @param y the y-axis position of the pixel's position
 * @param color the color the pixel to be displayed will be
 */
void (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief This function is responsible for drawing the matrix that represents the board of the game
 * @param mode The VBE mode that determines the color format (direct or indexed) 
 * @param no_rectangles The number of rectangles in each row 
 * @param first Used to map the color
 * @param step Used to map the color
 * @return Return 0 in case of success, 1 if otherwise
*/
int vg_draw_matrix(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief This function returns the red component of a color in RGB
 * @param color The original color
 * @return The red component of the original color
 */
uint8_t (R)(uint32_t color);

/**
 * @brief This function returns the green component of a color in RGB
 * @param color The original color
 * @return The green component of the original color
 */
uint8_t (G)(uint32_t color);

/**
 * @brief This function returns the blue component of a color in RGB
 * @param color The original color
 * @return The blue component of the original color
 */
uint8_t (B)(uint32_t color);

/**
 * @brief This function returns the color formed by the red, green and blue componets
 * @return The final color 
*/
uint32_t (build_RGB)(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief This function returns the index of the color mapped in a Minix color palette
 * @return The index of the color
*/
uint8_t (get_indexed_color)(uint row, uint col, uint32_t first, uint8_t no_rectangles, uint8_t step);

/**
 * @brief This functions return a color formed by a set of bytes that represent each of the RGB components
 * @returns The final color
*/
uint32_t (get_direct_color)(uint row, uint col, uint32_t first, uint8_t no_rectangles, uint8_t step);

/**
 * @brief This function prints a xmp image to the display
 * @param img The xpm image to be loaded to the display
 * @param x the x-axis position of the top left pixel's position of the xpm
 * @param y the y-axis position of the top left pixel's position of the xpm
 */
void draw_xpm(xpm_image_t img, uint16_t x, uint16_t y);

/**
 * @brief This functions reads and stores all the necessary information in the struct vg_vbe_contr_info_t
 * @param info Object of type vg_vbe_contr_info_t struct
 * @return Return 0 in case of success, 1 if otherwise
*/
int (controller_info)(vg_vbe_contr_info_t * info);

/**
 * @brief This function draws the game board on the screen using the provided XPM image.
 * @param board The XPM image of the game board to be drawn.
 * @return EXIT_SUCCESS upon successful execution.
*/
int vg_draw_board(xpm_image_t board);

/**
 * @brief This function draws an individual cell of the game board on the screen using the provided XPM image.
 * @param img The XPM image representing the cell to be drawn.
 * @param x The x-coordinate of the cell.
 * @param y The y-coordinate of the cell.
*/
void draw_board_cell(xpm_image_t img, uint8_t x, uint8_t y);
