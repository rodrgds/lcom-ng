/**
 * @file video.h
 * @author LCOM professors
 * @brief Video card handling
 * @version 0.1
 * @date 2021-01-05
 * 
 * Adapted by Bernardo Santos (bernardo.ots@fe.up.pt)
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __VIDEO_H
#define __VIDEO_H

#include <lcom/lcf.h>

/** @defgroup video Video
 * @{
 *
 * Video card handling
 */

/**
 * @brief Use the VBE function 02 to set the VBE mode.
 * 
 * @param mode VBE mode
 * @return 0 on success, 1 on failure 
 */
int(set_vbe_mode)(uint16_t mode);

/**
 * @brief Use the VBE function 01 to get the VBE Mode Info Block.
 * 
 * @param mode VBE mode
 * @param vmi_p VBE Mode Info Block
 * @return 0 on success, 1 on failure 
 */
int(get_vbe_mode_info)(uint16_t mode, vbe_mode_info_t *vmi_p);

/**
 * @brief Map physical memory to the process' (virtual) address space and get screen attributes.
 * 
 * @param mode VBE mode
 * @return 0 on success, 1 on failure 
 */
int(map_vram)(uint16_t mode);

/**
 * @brief Draw a pixmap on the screen.
 * 
 * @param pixmap the pixel map, an array of pixels
 * @param x starting x coordinate on the screen
 * @param y starting y coordinate on the screen
 * @param width pixmap width
 * @param height pixmap height
 * @return 0 on success, 1 on failure 
 */
int(draw_pixmap)(uint8_t *pixmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Erase a pixmap from the screen.
 * 
 * @param x starting x coordinate on the screen
 * @param y starting y coordinate on the screen
 * @param width pixmap width
 * @param height pixmap height
 * @return 0 on success, 1 on failure 
 */
int(erase_pixmap)(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Get the screen horizontal resolution.
 * 
 * @return horizontal resolution 
 */
unsigned int(get_hres)();

/**
 * @brief Get the screen vertical resolution.
 * 
 * @return vertical resolution 
 */
unsigned int(get_vres)();

/** @} end of video */

#endif
