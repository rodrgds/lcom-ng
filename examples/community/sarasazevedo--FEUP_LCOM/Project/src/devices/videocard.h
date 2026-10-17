/**
 * @file videocard.h
 * 
 * @brief Variables to store information about the videocard's current graphics submode and associated frame-buffers and functions to handle that information, paint pixels and set the Graphics Mode and frame-buffers allocation
*/

#ifndef _VIDEOCARD_H_
#define _VIDEOCARD_H_

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "VBE.h"

/** @defgroup videocard videocard
 * @{
 *
 * @brief  videocard related functions
 * 
 */

/* ---------------- Functions to set the videocards' graphic submode, initialize global variables about the chosen Graphics Mode and to deal with the frame buffers -------------------------*/


/**
 * @brief Initializes the videocard's global variables with information about the chosen submode
 * 
 * @param mode The chosen submode
 * @return Return 0 upon success and non-zero otherwise
*/
int (set_submode_info)(uint16_t mode);



/**
 * @brief Initializes the videocard's frame_buffer global variable with the initial virtual address of a frame-buffer with the correct size for the chosen submode and allocates memory for the extra temporary frame buffer
 * 
 * @param mode The chosen submode
 * @return Return 0 upon success and non-zero otherwise
*/
int (map_video_memory)(uint16_t mode);

/**
 * @brief Set the display start during vertical retrace.
 *
 * This function sets the display start coordinates (x, y) during the vertical retrace interval.
 * This is useful to avoid visual artifacts such as tearing by ensuring the buffer swap happens
 * during the vertical blanking interval.
 *
 * @param x The x coordinate of the display start.
 * @param y The y coordinate of the display start.
 * 
 * @return 0 on success, 1 otherwise.
 */
int vbe_vertical_retrace(uint16_t x, uint16_t y);


/**
 * @brief Copies the temporary frame buffer to the main frame buffer
*/
void (swap_buffer)();


/**
 * @brief Frees the memory space previously allocated to the extra temporary frame buffer
*/
void(destroy_buffer)();



/**
 * @brief Configures the videocard to work in Graphics Mode in the chosen submode using VBE Function 0x4F02 to do that, prepares the videocard's variables with information about the chosen submode and maps memory for the frame-buffers
 * 
 * @param mode The chosen graphics submode
 * @return Return 0 upon success, non-zero otherwise
*/
int (vg_set_graphics_mode)(uint16_t mode);






/* ----------- Getter Functions to use global variables outside videocard file ------------------------ */

/**
 * @brief Get the videocard's hres
 * @return Return the screen's XResolution
*/
uint16_t (get_hres)();


/**
 * @brief Get the videocard's vres
 * @return Return the screen's YResolution
*/
uint16_t (get_vres)();


/**
 * @brief Get the videocard's BitsPerPixel
 * @return Return the number of used bits per pixel
*/
uint16_t (get_bitsPerPixel)();

//Related to color components, transforming the values to int to use with bitwise operations

/**
 * @brief Get the videocard's redMaskSize
 * @return Return the size of the color's red component
*/
int (get_redMaskSize)();

/**
 * @brief Get the videocard's greenMaskSize
 * @return Return the size of the color's green component
*/
int (get_greenMaskSize)();


/**
 * @brief Get the videocard's blueMaskSize
 * @return Return the size of the color's blue component
*/
int (get_blueMaskSize)();

/**
 * @brief Get the videocard's redFieldPosition
 * @return Return the position of the color's red componenet
*/
int (get_redFieldPosition)();

/**
 * @brief Get the videocard's greenFieldPosition
 * @return Return the position of the color's green componenet
*/
int (get_greenFieldPosition)();

/**
 * @brief Get the videocard's blueFieldPosition
 * @return Return the position of the color's blue componenet
*/
int (get_blueFieldPosition)();




/* ----------- Helper Functions that perform calculations ------------------------ */

/**
 * @brief Find how many bytes per pixel are used for the current videocard's submode, rounding up, and set the videocard's bytespPerPixel variable
*/
void (set_bytesPerPixel)();

/**
 * @brief Find how many bytes the videocard's frame-buffer has and set the videocard's frameBufferSize variable
*/
void (set_frameBufferSize)();



/* ----------- Helper Function to help paiting pixels ------------------------ */

/**
 * @brief Paints pixel (x,y) with the specified color by copying the color's bytes to the pixel's memory area
 * 
 * @param x The x coordinate of the pixel in the screen
 * @param y The y coordinate of the pixel in the screen
 * @param color The color to paint the pixel
 * 
 * @return Return 0 upon susccess, non-zero otherwise
*/
int (fill_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Loads a XPM image from a XPM pixmap, with the xpm_image_type being XPM_8_8_8_8 (direct color mode with 32 bits per pixel)
 * 
 * @param xpm_map The pixmap to load the image from
 * @return Return a struct with the information about a XPM image
*/
xpm_image_t my_load_xpm(xpm_map_t xpm_map);

/** @} end of videocard */


#endif /* __VIDEOCARD_H */

