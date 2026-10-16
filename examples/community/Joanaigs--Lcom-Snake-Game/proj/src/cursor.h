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


static inline void cursor_move(cursor *c, int dx, int dy, int width, int height) {
    int max_x = width > c->img.width ? width - c->img.width : 0;
    int max_y = height > c->img.height ? height - c->img.height : 0;
    int next_x = c->x + dx;
    int next_y = c->y - dy;
    c->x = next_x < 0 ? 0 : next_x > max_x ? max_x : next_x;
    c->y = next_y < 0 ? 0 : next_y > max_y ? max_y : next_y;
}



#endif /*_CURSOR_H_ */
