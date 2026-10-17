/**
 * @file video
 * 
 * @brief This file contains functions related to video handling and graphics operations. It includes functions to set the graphic mode, map the video memory, draw pixels, lines, and rectangles, 
 * as well as functions for buffer manipulation and video initialization.
 * 
 * @author João Pedro Sousa, Bernardo Pinto, Carlos Madaleno
 * 
 * @date 28/05/2023 
*/

#ifndef SRC_VIDEO_H
#define SRC_VIDEO_H

#include <lcom/lcf.h>
#include <math.h>
#include "VBE.h"

uint8_t* baseAddress;

/**
 * @brief Sets the graphics mode.
 * 
 * @param submode The specific submode of the graphics mode to be set.
 * 
 * @return 0 on success, non-zero if there was an error setting the graphics mode.
*/

int (set_graphic_mode)(uint16_t submode);

/**
 * @brief Sets the frame buffer for video mode.
 * 
 * @param mode The video mode to set.
 * 
 * @return 0 on success, non-zero if there was an error setting the frame buffer.
*/

int (set_frame_buffer)(uint16_t mode);

/**
 * @brief Draws a pixel on the screen.
 * 
 * @param x The x-coordinate of the pixel.
 * 
 * @param y The y-coordinate of the pixel.
 * 
 * @param color The color value of the pixel.
 * 
 * @return 0 on success, non-zero if there was an error drawing the pixel.
*/

int draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Clears the frame buffer using memset.
*/

void clearBuffer();

/**
 * @brief Allocates memory for the frame buffer and auxiliary buffer using memset.
*/

void allocateSpaceBuffer();

/**
 * @brief Sets the video buffer to the contents of the frame buffer using memcpy.
*/

void videoSet();

/**
 * @brief Copies the contents of the auxiliar buffer to the frame buffer using memcpy.
*/

void copyToAuxiliarBuffer();

#endif //SRC_VIDEO_H
