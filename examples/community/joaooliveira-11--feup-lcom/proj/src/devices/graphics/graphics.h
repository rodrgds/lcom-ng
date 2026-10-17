#ifndef _GRAPHICS_PROJECT
#define _GRAPHICS_PROJECT

#include <lcom/lcf.h>

vbe_mode_info_t vbe_mode_info;


///@brief Change to graphic mode with the specified submode.
///@note This function switches to graphic mode with the specified submode.
///@param submode The desired submode.
///@return 0 if the change is successful, otherwise returns 1.
int (changeTo_graphic_mode)(uint16_t submode);

///@brief Build the frame buffer in the specified mode.
///@note This function builds the frame buffer in the specified mode by allocating the necessary memory.
///@param mode The desired video mode.
///@param buffer The pointer to the frame buffer to be built.
///@return 0 if the build is successful, otherwise returns 1.
int (frame_buffer_build)(uint16_t mode, uint8_t** frame_buffer);

///@brief Color the pixel at the specified coordinates.
///@note This function colors the pixel at coordinates (x, y) with the specified color.
///@param x The x-coordinate of the pixel.
///@param y The y-coordinate of the pixel.
///@param color The color to be applied to the pixel.
///@param frameBuffer The frame buffer where the pixel should be colored.
///@return 0 if the coloring is successful, otherwise returns 1.
int (color_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t* frame_buffer);

#endif
