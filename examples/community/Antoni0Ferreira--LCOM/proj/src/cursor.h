#ifndef __CURSOR_H
#define __CURSOR_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "video.h"

//sprite from http://pixelartmaker.com/art/8639fae9c1747fb
#include "xpm/cursor.xpm"

/**
 * @defgroup cursor cursor 
 * @{
 * 
 * Functions regarding the initialization of the mouse's cursor
 */

/**
 * @brief Struct type for storing and organizing information relative to the cursor
 * 
 */
struct cursor {
    int16_t x, y;
    xpm_image_t img;
    uint8_t *sprite;
    bool LBpressed, RBpressed;
};

/**
 * @brief Initializes the cursor's position, xpm image, sprite and state
 * of the left and right buttons of the mouse
 * 
 * @param cursor The mouse's cursor
 * @return Return 0 upon success and non-zero otherwise 
 */
int (initialize_cursor)(struct cursor *cursor);

/**
 * @}
 */

#endif /* __CURSOR_H */
