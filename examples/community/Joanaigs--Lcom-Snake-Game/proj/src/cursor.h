#ifndef _CURSOR_H_
#define _CURSOR_H_

#include <stdbool.h>
#include <stdint.h>
#include <lcom/lcf.h>
/**
 * @brief struct that represents the cursor.It saves the position and the image of the cursor
 * 
 */
typedef struct{
    int x,y;
    xpm_image_t img;
}cursor;



#endif /*_CURSOR_H_ */
