#include <lcom/lcf.h>

#include <stdint.h>
#include <minix/sysutil.h>


/** @defgroup video_gr
 * @{
 *
 * Functions that deal with video card configuration and manipulation.
 */

/**
 * @brief paints the pixel in that x and y position in the screen
 * 
 * @param x x position of the pixel
 * @param y y position of the pixel
 * @param color collor to paint the pixel
 */
void vg_paint_pixel(uint16_t x,uint16_t y,uint32_t color);

/**
 * @brief maps all buffers used to draw  the screen 
 * 
 * @param mode video mode 
 * @return void* pointer to the beginning of the allocated buffer
 */
void *(vg_map)(uint16_t mode);

/**
 * @brief gets all information related to that mode
 * 
 * @param mode video mode 
 * @param vmi struct that contai all the info returned by the function call
 * @return int 
 */
int vbe_get_mode_info_me(uint16_t mode,vbe_mode_info_t * vmi);

/**
 * @brief set main buffer memory to tranperency collor
 * 
 */
void (clean_screen)();

/**
 * @brief this function free all the memory alocated to the background buffer and second buffer
 * 
 */
void (free_buffer_mem)();

/**
 * @brief copy data of second buffer to main buffer
 * 
 */
void (copy_buffer)();

/**
 * @brief set all buffer conttent to tranparency collor
 * 
 */
void (clean_buffer)();

/**
 * @brief copy data of background buffer to second buffer
 * 
 */
void (copy_background_buffer)();

/**
 * @brief used to initialize background buffer
 * 
 */
void (copy_buffer_to_background_buffer)();

/**
 * @brief Get the max x of the buffer
 * 
 * @return int x max
 */
int get_x_max();

/**
 * @brief Get the max y of the buffer
 * 
 * @return int y max
 */
int get_y_max();

