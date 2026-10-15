#ifndef _LCOM_VG_H_
#define _LCOM_VG_H_

#include <lcom/lcf.h>
#include <lcom/vbe.h>

#include <stdint.h>
#include <stdio.h>

//BIOS
#define VBE_SET_MODE_FUNCTION 0x4F
#define VBE_SET_MODE_SUBFUNCTION 0x02
#define BIOS_INTERRUPT_GRAPHICS_CARD 0x10

//Graphics modes
#define MODE_640_480_D 0x110
#define MODE_800_600_D 0x115
#define MODE_1024_768_IC 0x105
#define MODE_1152_864_D 0x14C
#define MODE_1280_1024_D 0x11A

//Main buffer
uint8_t* videoMemory; 


vbe_mode_info_t vmi;



/**
 * @brief Calculates a frame size for selected mode
 * 
 * @return frame size
 */
unsigned int get_frame_size();



/**
 * @brief Sets the limits ot the frame according to the mode used
 *
 * @param mode graphics card mode
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int set_frame(uint16_t mode);


/**
 * @brief Sets the video mode
 *
 * @param mode graphics card mode
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int set_video_mode (uint16_t mode);



/**
 * @brief Draws a pixel of a color, in (x,y) position
 *
 * @param x position referencing to the x axis
 * @param y position referencing to the y axis
 * @param color pixel color
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_pixel(uint16_t x, uint16_t y, uint32_t color);



/**
 * @brief Draws a rectangle, with top left (x,y) anchor position
 *
 * @param x position referencing to the x axis
 * @param y position referencing to the y axis
 * @param width rectangle width
 * @param height rectangle height
 * @param color rectangle color
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_rectangle(int x, int y, int width, int height, uint32_t color);



/**
 * @brief Can only be used after a xpm_load call due to map and image attribute inavailability.
 * Draws xpm with top left (x,y) anchor position
 *
 * @param map holds the xpm map, after xpm_load
 * @param x position referencing to the x axis
 * @param y position referencing to the y axis
 * @param img holds the xpm image, after xpm_load
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_xpm(uint8_t *map, uint16_t x, uint16_t y, xpm_image_t img);


// Updates main buffer with double buffer information
void update_screen();

#endif
