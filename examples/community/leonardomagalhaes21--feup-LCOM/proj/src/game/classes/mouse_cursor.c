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
