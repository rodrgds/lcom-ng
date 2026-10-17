#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>

#define TRANSPARENT_COLOR 0x123123

vbe_mode_info_t vbe_info;

/// @brief sets the video graphics to the graphic mode
/// @param submode submode desired
/// @return 1 in case of success, and 0 if there´s any problem with the system calls
int (switch_to_graphic_mode)(uint16_t submode);

/// @brief sets the frame buffer according to the submode and the needed space
/// @param mode submode desired
/// @return 1 in case of success, and 0 if there´s any problem with the system call
int (set_frame_buffer)(uint16_t submode,uint8_t** frame_buffer);

/// @brief draws one pixel in the screen
/// @param x x coordinate
/// @param y y coordinate
/// @param color color thet the pixel will have
/// @param frame_buffer varible that contains the addres to the physical location in memory 
/// @return varible that will contain the addres to the physical location in memory 
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer);

#endif
