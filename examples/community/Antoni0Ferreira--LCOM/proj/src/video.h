#ifndef __VIDEO_H
#define __VIDEO_H
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @defgroup video video
 * @{
 * 
 * Module for the video graphics
 */

#define PROJ_VG_MODE 0x115 // Video mode used in the project
#define RIGHT_BORDER_LIMIT 800 // Width of the Screen
#define DOWN_BORDER_LIMIT 600 // Height of the Screen

/**
 * @brief Function responsible for getting the information related to the video mode
 * and stores them in the struct vmi_p
 * 
 * @param mode Video graphics mode
 * @param vmi_p Struct that will contain all the information related to the video mode selected
 * @return Return 0 upon success and non-zero otherwise 
 */
int (my_vbe_get_mode_info)(uint16_t mode, vbe_mode_info_t *vmi_p);

/**
 * @brief Writes color in memory correspondent to pixel (x,y)
 * 
 *
 * @param x Left upper corner of image (x coordinate)
 * @param y Left upper corner of image (y coordinate)
 * @param color Sequence of bytes that represent the color that will be stored in the vram
 * @return Return 0 upon success and non-zero otherwise
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws the image passed by parameters starting in coordinates (x,y)
 * 
 * @param map Array with all the rgb components for each pixel
 * @param x Left upper corner of image (x coordinate)
 * @param y Left upper corner of image (y coordinate)
 * @param img Contains the info from the sprite being drawn (height, width)
 * @return Return 0 upon success and non-zero otherwise 
 */
int (vg_draw_sprite)(uint8_t *map, uint16_t x, uint16_t y, xpm_image_t img);

/**
 * @brief Replaces the memory correspondent to the sprite being destroyed with the background colors
 * 
 * @param x Left upper corner of image (x coordinate)
 * @param y Left upper corner of image (y coordinate)
 * @param img Contains the info from the sprite being destroyed (height, width)
 * @param bgMap Background colors
 * @return Return 0 upon success and non-zero otherwise 
 */
int (vg_destroy_sprite)(uint16_t x, uint16_t y, xpm_image_t img, uint8_t *bgMap);

/**
 * @brief Copies the memory from the secondary buffer to vram, used to implement double-buffering
 * 
 */
void (copy_buffer)();

/**
 * @brief Draws the astros and checks for collisions with the player and shots
 * 
 * @param map Colors of the astro
 * @param x Left upper corner of image (x coordinate)
 * @param y Left upper corner of image (y coordinate)
 * @param img Contains the info from the astro being drawn (height, width)
 * @param bgMap Colors of the background
 * @return Return 0 upon success, return 1 if collision with player and return 2 with collision with shot
 */
int (vg_draw_astro)(uint8_t *map, uint16_t x, uint16_t y, xpm_image_t img, uint8_t *bgMap);


/**
 * @}
 */

#endif /* __VIDEO_H */

