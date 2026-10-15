#include "cursor_input.h"
#include <lcom/lcf.h>

CursorInput (read_cursor_input)(struct packet *packet){

    CursorInput CursorInput;

    int16_t move_x = (((int16_t) packet->x_ov) << 8) | packet->delta_x;
    int16_t move_y = (((int16_t) packet->y_ov) << 8) | packet->delta_y;
                            
    cursor_move(move_x, -move_y);

    CursorInput.leftClick = packet->lb;

    CursorInput.rightClick = packet->rb;

    CursorInput.middleClick = packet->mb;

    CursorInput.x = cursor->x;
    CursorInput.y = cursor->y;

    return CursorInput;
}
