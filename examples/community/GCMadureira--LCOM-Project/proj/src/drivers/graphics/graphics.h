/** 
 * @file graphics.h
 * @brief Header file for @ref graphics.c
 * 
 * This file declares all the functions to interact with the graphics and the vbe.
 */


#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <lcom/lcf.h>
#include <math.h>
#include "vbe.h"
#include "../../macros/colors.h"

/**
 * @brief Sets a new vbe mode for the graphics
 * 
 * Uses the reg86_t struct and calls sys_int86().
 * 
 * @param mode The new mode to set
 * @return 0 in case of success, 1 otherwise
 */
int (vg_set_VBE_mode)(uint16_t mode);

/**
 * @brief Initializes the graphics with a mode
 * 
 * Calls vg_map_video_memory() and vg_set_VBE_mode() with the mode as a parameter.
 * 
 * @param mode The new mode to initialize
 * @return 0 in case of success, 1 otherwise
 */
int (vg_graphics_init)(uint16_t mode);

/**
 * @brief Maps the video memory into the program's memory space
 * 
 * Gets the mode info to obtain the necessary memory to map, then calls vm_map_phys to map it. <br>
 * Also sets the @ref bytes_per_pixel, @ref buffer and @ref video_mem static variables.
 * 
 * @param mode The new mode to initialize
 * @return 0 in case of success, 1 otherwise
 */
int (vg_map_video_memory)(uint16_t mode);

/**
 * @brief Getter for the @ref video_mem static variable
 * 
 * @return Pointer to the video_mem
 */
void* (vg_get_video_mem)();

/**
 * @brief Getter for the @ref mode_info static variable
 * 
 * @return The mode_info variable
 */
vbe_mode_info_t (vg_get_current_mode_info)();

/**
 * @brief Getter for the horizontal resolution
 * 
 * @return The horizontal resolution from @ref mode_info
 */
unsigned int (vg_get_hres)();

/**
 * @brief Getter for the vertical resolution
 * 
 * @return The vertical resolution from @ref mode_info
 */
unsigned int (vg_get_vres)();

/**
 * @brief Getter for the bites per pixel
 * 
 * @return The bites per pixel from @ref mode_info
 */
unsigned int (vg_get_bits_per_pixel)();

/**
 * @brief Check if the current mode is a direct color one
 * 
 * @return true if the mode is direct color, false otherwise
 */
bool (vg_is_direct_color_model)();

/**
 * @brief Extracts the red color field of the provided color
 * 
 * Assumes the size of the fields as the ones from the current mode.
 * 
 * @param color The color to extract from
 * @return The red field of the color
 */
uint32_t (vg_get_red_color_field)(const uint32_t color);

/**
 * @brief Extracts the green color field of the provided color
 * 
 * Assumes the size of the fields as the ones from the current mode.
 * 
 * @param color The color to extract from
 * @return The green field of the color
 */
uint32_t (vg_get_green_color_field)(const uint32_t color);

/**
 * @brief Extracts the blue color field of the provided color
 * 
 * Assumes the size of the fields as the ones from the current mode.
 * 
 * @param color The color to extract from
 * @return The blue field of the color
 */
uint32_t (vg_get_blue_color_field)(const uint32_t color);

/**
 * @brief Draws a pixel on the secondary @ref buffer
 * 
 * @param x The x coordinate on the screen
 * @param y The y coordinate on the screen
 * @param color The color of the drawn pixel
 * @return 0 in case of success, 1 otherwise
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a rectangle on the secondary @ref buffer for a 32 bit color mode
 * 
 * @param x The x coordinate on the screen
 * @param y The y coordinate on the screen
 * @param width The width of the resulting rectangle
 * @param height The height of the resulting rectangle
 * @param color The color of the resulting rectangle
 * @return 0 in case of success, 1 otherwise
 */
int (vg_draw_rectangle32)(int32_t x, int32_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws an image on the secondary @ref buffer for a 32 bit color mode
 * 
 * @param x The x coordinate on the screen
 * @param y The y coordinate on the screen
 * @param img_info The information of the image to be drawn
 * @return 0 in case of success, 1 otherwise
 */
int (vg_draw_image32)(int32_t x, int32_t y, xpm_image_t* img_info);

/**
 * @brief Draws a section of an image on the secondary @ref buffer for a 32 bit color mode
 * 
 * @param x The x coordinate on the screen
 * @param y The y coordinate on the screen
 * @param img_info The information of the image to be drawn
 * @param x_section The x coordinate of the section of the image
 * @param y_section The y coordinate of the section of the image
 * @param width The width of the section
 * @param height The height of the section
 * @return 0 in case of success, 1 otherwise
 */
int (vg_draw_image_section32)(int32_t x, int32_t y, xpm_image_t* img_info, uint16_t x_section,  uint16_t y_section, uint16_t width, uint16_t height);

/**
 * @brief Draws the information on the @ref buffer to the @ref video_mem
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (vg_show_frame)();

#endif /*_GRAPHICS_H */
