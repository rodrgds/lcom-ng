/**
 * @file video.h
 * @brief file that has functions to allow configurating and map memory to the graphics card
 * 
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <machine/int86.h>

#define VIDEO_VALUE 0x10;
#define VBE_MODE 0x4F02;
#define LINEAR_BUFFER BIT(14);

vbe_mode_info_t m_info;

/**
 * @brief allocates memory for the mode and sets and configurates the desired video mode
 * @param mode video mode that we want to set
 * @return 0 if success otherwise 1
 */
int (video_init)(uint16_t mode);

/**
 * @brief allocates virtual and physical memory for the buffers depending on the mode that we pass to it
 * @param mode video mode that is setted
 * @param buffer where we want to allocate the memory
 */
int (map_phys_vir_mem)(uint16_t mode, uint8_t** buffer);

/**
 * @brief draws a pixel in the screen, this is mostly used in the function to draw a sprite on the screen
 * @param x vertical position of the pixel on the screen
 * @param y horizontal position of the pixel on the screen
 * @param color the color in hex that we want to draw the pixel
 * @param buffer in which buffer will the memory will be copied
 */
int (draw_pixel)(uint16_t x,  uint16_t y, uint32_t color, uint8_t* buffer);

#endif /* __GRAPHICS_H */
