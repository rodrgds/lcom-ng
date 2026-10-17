#include <lcom/lcf.h>
#include <stdint.h>
#include "cursor.h"


Cursor* createCursor(xpm_map_t sprite, uint16_t x, uint16_t y) {
    Cursor* cursor =  (Cursor*)malloc(sizeof(Cursor));
  
  cursor->x =x;
  cursor->y= y;
  cursor->curr_sprite = create_sprite(sprite,x,y,0,0);
  return cursor;
}
void drawCursor(Cursor* cursor) {
  draw_sprite(cursor->curr_sprite);
}
void updateCursor(Cursor* cursor, uint16_t x, uint16_t y) {

  //stops the cursor from moving out of the screen
  if(x+cursor->curr_sprite->width > get_hres() || x > get_hres() || y + cursor->curr_sprite->height > get_vres() || y > get_vres() ) {
    return;
  }
  cursor->x = x;
  cursor->y = y;
  cursor->curr_sprite->x=x;
  cursor->curr_sprite->y=y;
}

void destroyCursor(Cursor* cursor) {
  if(cursor == NULL) {
        return;
    }

    destroy_sprite(cursor->curr_sprite);
    free((cursor));
}





