/**
 * @file video.h
 *
 */

#ifndef _LCOM_VIDEO_H_
#define _LCOM_VIDEO_H_

#include <lcom/lcf.h>
#include <stdint.h>

#include "VBE.h"

#define RED(color) ((color & mode_info.red_mask) >> mode_info.red_begin)
#define GREEN(color) ((color & mode_info.green_mask) >> mode_info.green_begin)
#define BLUE(color) ((color & mode_info.blue_mask) >> mode_info.blue_begin)

typedef struct {
    // general info
    uint8_t mode;
    uint8_t memory_model;
    uint8_t bits_per_pixel, bytes_per_pixel;
    uint16_t x_res, y_res;

    // physical memory
    phys_bytes physical_base;
    uint32_t physical_size;

    // color
    uint32_t red_mask, green_mask, blue_mask;
    uint8_t red_begin, green_begin, blue_begin;
    uint8_t red_end, green_end, blue_end;
} video_mode_info;

/**
 * @brief calls the function get_mode_info, gets information about the given mode
 * 
 * @param mode video card mode
 * 
 */
int (video_get_mode_info)(uint16_t mode);

/**
 * @brief switches the video card mode to the given mode, allocating memory
 * 
 * @param mode video card mode
 * 
 */
int (video_start)(uint16_t mode);

/**
 * @brief switches the video mode back to text mode
 * 
 */
int (video_stop)();

/**
 * @brief copies the memory from the back buffer to the front buffer
 * 
 */
int (video_switch)();

/**
 * @brief draws pixel at given position with given color
 * 
 * @param x x position of the pixel
 * 
 * @param y y position of the pixel
 * 
 * @param color color with which to paint the pixel
 * 
 */
int (video_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
/**
 * @brief draws row of pixels starting at given position, with given lenth and given color
 * 
 * @param x x starting position
 * 
 * @param y y starting position
 * 
 * @param len lenth of the row
 * 
 * @param color color with which to paint the row
 * 
 */
int (video_draw_row)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**
 * @brief draws column of pixels starting at given position, with given lenth and given color
 * 
 * @param x x starting position
 * 
 * @param y y starting position
 * 
 * @param len lenth of the column
 * 
 * @param color color with which to paint the column
 * 
 */
int (video_draw_col)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**
 * @brief draws rectangle of pixels
 * 
 * @param x x starting position
 * 
 * @param y y starting position
 * 
 * @param width width of the rectangle
 * 
 * @param height height of the rectangle
 * 
 * @param color color with which to paint the rectangle
 * 
 */
int (video_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief draws a sprite
 * 
 * @param x horizontal position
 * @param y vertical position
 * @param width width of the sprite
 * @param height height of the sprite
 * @param colors pointer to the array of the sprite's colors
 * 
 */
int (video_draw_sprite)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t* colors);

#endif // _LCOM_VIDEO_H_
