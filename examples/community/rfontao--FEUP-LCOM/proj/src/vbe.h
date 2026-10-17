#ifndef _VBEMODULE_H_
#define _VBEMODULE_H_

#include <lcom/lcf.h>

#include "utils.h"
#include <machine/int86.h>
#include "vbe_macros.h"

/** @defgroup VBE VBE
 * @{
 *
 * Constants functions of the VBE
 */


#define XRES 2000       /**< @brief X resulotion of the game matrix*/
#define YRES 2000       /**< @brief Y resulotion of the game matrix*/
#define MODE 0x117      /**< @brief Video mode for the VBE*/

/**
 * @brief Allocates virtual memory for the video graphics and sets the mode 
 * 
 * @param mode          Desired video mode
 * @return              Returns a pointer to the allocated video buffer
 */
void * (vg_init)(uint16_t mode);

/**
 * @brief Fills a pixel at (x,y) with color color
 * 
 * @param x             X position to fill
 * @param y             Y position to fill
 * @param color         Color to fill
 */
void vg_fillPixel(uint16_t x, uint16_t y, uint8_t *color);

/**
 * @brief Get the mode info of the VBE
 * 
 * @param mode          The mode to get the info of
 * @param info          The return valye
 * @return              1 on failure
 */
int get_mode_info(uint16_t mode,vbe_mode_info_t * info);

/**
 * @brief Gets the VBE controller info
 * 
 * @return      1 on failure
 */
int vbe_get_contr_info();

/**
 * @brief Converts a far pointer to a normal pointer
 * 
 * @param far           Far pointer to convert
 * @param map           ?
 * @return              The value of the result pointer       
 */
void* farToPtr(uint32_t far,mmap_t map);

/**
 * @brief Displays the buffer in (x,y) in the screen
 * 
 * @param video_buffer          Pointer to the screen video buffer
 * @param video_mem             Painter to the video matrix
 * @param x                     Camera X
 * @param y                     Camera Y
 */
void displayBuffer(void* video_buffer,void* video_mem,int x,int y);

/**@}*/

#endif
