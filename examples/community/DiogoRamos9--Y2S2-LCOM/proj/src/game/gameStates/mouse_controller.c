#include <lcom/lcf.h>

#include "mouse_controller.h"

void update_mouse() {
    if((pp.x_ov != 0) && (pp.delta_x + cursor->x < 0)) cursor->x = 0;
    else if((pp.x_ov != 0) && (pp.delta_x + cursor->x > 800)) cursor->x = 800;
    else cursor->x += pp.delta_x;

    if((pp.y_ov != 0) && (pp.delta_y + cursor->y < 0)) cursor->y = 0;
    else if((pp.y_ov != 0) && (pp.delta_y + cursor->y > 600)) cursor->y = 600;
    else cursor->y -= pp.delta_y;
}

void draw_mouse() {
    draw_sprite(cursor, cursor->x, cursor->y);
}
