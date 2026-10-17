/**
 * @file video_gr.h
 * @author Bruno Pinto and Vitor Magalhaes
 * @brief Contains functions related to video graphics
 */

#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "vbe.h"

unsigned char *video_mem;			/* Process address to which VRAM is mapped */
unsigned char *buffer;

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Returns information on the input VBE mode, including screen dimensions, color depth and VRAM physical address
 *
 * Initializes unpacked vbe_mode__info_t structure passed as an address with
 *  the information of the input mode, by calling VBE function 0x01
 *  Return VBE Mode Information and unpacking the ModeInfoBlock struct
 *  returned by that function.
 *
 * @param mode mode whose information should be returned
 * @param vmi_p address of vbe_mode_info_t structure to be initialized
 * @return 0 on success, non-zero otherwise
 */
int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p);

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

 /**
 * @brief Returns horizontal resolution
 *
 * @return Size of horizontal resolution
 */
unsigned getHRes();

 /**
 * @brief Returns vertical resolution
 *
 * @return Size of vertical resolution
 */
unsigned getVRes();

 /**
 * @brief Returns number of bits per pixel
 *
 * @return Number of Bits per Pixel
 */
unsigned getBitsPerPixel();

/**
* @brief Returns a pointer to Buffer
*
* @return Pointer to Buffer
*/
char* getBuffer();

/**
 * @brief Copies buffer to video_mem
 */
void copy_to_video_mem();

/**
 * @brief Clears the buffer
 */
void clear_buffer();

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
