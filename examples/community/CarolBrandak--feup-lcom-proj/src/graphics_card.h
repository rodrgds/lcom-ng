#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Sets the graphics card to operate in the mode given in the argument, also stores the values of hRes and vRes as well as the number of bytes per pyxel for later use
 * 
 */
int (set_graphics_mode)(uint16_t mode);

/**
 * @brief Changes the color of one pyxel in the given position and to the color given in the argument and does so by memcpy to a buffer, not the memory
 * 
 */
int (changeColorPixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Copies the buffer to the virtual adress of memory
 * 
 */
void (vg_copy)();


