#include "mouse_cursor.h"

MouseCursor *createMouseCursor(int x,int y, Sprite *sp) {
    MouseCursor *mc = (MouseCursor *) malloc ( sizeof(*mc));
    
    if( mc == NULL )
      return NULL;
    mc->x = x; 
    mc->y = y;
    mc->sprite = sp;
    return mc;
}


void moveMouseCursor(MouseCursor *mc, int dx, int dy, int width, int height) {
    int next_x = mc->x + dx;
    int next_y = mc->y - dy;
    if (next_x <= 0 || next_y <= 0 ||
        next_x > width - mc->sprite->width || next_y > height - mc->sprite->height)
      return;
    mc->x = next_x;
    mc->y = next_y;
}
