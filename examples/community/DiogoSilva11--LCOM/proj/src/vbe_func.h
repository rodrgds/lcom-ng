#ifndef _LCOM_VBE_FUNC_H_
#define _LCOM_VBE_FUNC_H_

#include <stdint.h>

/** @defgroup vbe vbe
 * @{
 * 
 * Functions for the vbe.
 */

/**
 * @brief Map memory for video graphics
 *   
 * @param mode video mode selected
 * @return Return 0 upon success and non-zero otherwise
 */
int(map_memory)(uint16_t mode);

/**
 * @brief Get the mode's horizontal resolution  
 *
 * @return Return the horizontal value
 */
uint16_t(get_hres)();

/**
 * @brief Get the mode's vertical resolution  
 *
 * @return Return the vertical value
 */
uint16_t(get_vres)();

/**
 * @brief Get number of bytes per pixel for that mode   
 *
 * @return Return the bytes per pixel
 */
int(bytes_per_pixel)();

/**
 * @brief Generate a memory address corresponding to a pixel 
 *        with the color content inside the video buffer 
 *   
 * @param x pixel x position
 * @param y pixel y position
 * @param color pixel color
 * @return Return 0 upon success and non-zero otherwise
 */
int(vg_generate_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief  Reset video buffer content
 */
void(vg_clear_screen)();

/**
 * @brief Copies the video buffer content to the physical video memory
 */
void(vg_draw_content)();

/**
 * @brief Read the content of a pixmap  
 * 
 * @param pixmap sprite pixmap
 * @param width address of memory to be initialized with the sprite width
 * @param height address of memory to be initialized with the sprite height
 * @return Return a pointer to the sprite
 */
uint8_t*(vg_read_pixmap)(xpm_row_t *pixmap, uint16_t *width, uint16_t *height);

/**
 * @brief Generate pixels with the sprite's content  
 *
 * @param sprite sprite to be drawn
 * @param x starting x position
 * @param y starting y position
 * @param width sprite width
 * @param height sprite height
 * @return Return 0 upon success and non-zero otherwise
 */
int(vg_draw_sprite)(uint8_t *sprite, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Draw a specific pixmap in given positions   
 *
 * @param pixmap pixmap to be drawn
 * @param x starting x position
 * @param y starting y position
 * @return Return 0 upon success and non-zero otherwise
 */
int(vg_draw_element)(xpm_row_t *pixmap, uint16_t x, uint16_t y);

/**@}*/

#endif /* _LCOM_VBE_FUNC_H */
