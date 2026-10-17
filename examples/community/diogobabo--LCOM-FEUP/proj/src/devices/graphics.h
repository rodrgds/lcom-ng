#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

#include <lcom/lcf.h>
#include "vbeMacros.h"

/**
 * @brief sets the video graphics mode and maps the memory
 * @param mode
 * @return 0 if sucess
 */
int (video_set_graphics)(uint16_t mode);

/**
 * @brief draws the pallete for indexed mode
 * @param mode
 * @þaram no_rectangles
 * @param first
 * @param step
 * @return 0 if success, 1 otherwise
 */
int(vg_draw_matrix)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief Draws the XPM on the screen
 * @param x
 * @param y
 * @param map
 * @param img
 * @return 0 if success, 1 otherwise
 */
int (draw_pix_map)(uint16_t x, uint16_t y, uint8_t *map, xpm_image_t img);

/**
 * @brief Draws a letter/number from the XPM of the font used
 * @param x (coordenada x no ecrã)
 * @param y (coordenada y no ecrã)
 * @param map (coordenada y no ecrã)
 * @param img (coordenada y no ecrã)
 * @param startX (coordenada x no xpm)
 * @param startY (coordenada y no xpm)
 * @return 0 if success, 1 otherwise
 */
int (draw_letter_map)(uint16_t x, uint16_t y, uint8_t *map, xpm_image_t img, unsigned int startX, unsigned int startY);

/**
 * @brief Clears the XPM on the screen by paiting the background
 * @param x
 * @param y
 * @param img (background xpm IMG)
 * @return 0 if success, 1 otherwise
 */

int (clear_pix_map)(uint16_t x, uint16_t y, xpm_image_t img);

/**
 * @brief Used for the Double Buffering
 */
void switchBuffer();

/**
 * @brief Clears the XPM on the screen by paiting the background
 * @param x
 * @param y
 * @param width
 * @param height
 * @param img
 * @param map
 */
void cleanBG(unsigned int x, unsigned int y, int width, int height, xpm_image_t img, uint8_t* map);

/**
 * @brief Makes minix go back to TEXT Mode & frees the buffer
 */
void exit_graphics();
#endif
