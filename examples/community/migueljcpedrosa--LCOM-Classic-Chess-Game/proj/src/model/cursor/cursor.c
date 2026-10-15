#include "cursor.h"
#include "../../drivers/video_card/gpu.h"
#include <stdlib.h>

void (cursor_create)(int x, int y){

    cursor = (Cursor*) malloc(sizeof(Cursor));

    cursor->x = x;
    cursor->y = y;
    cursor->old_x = x;
    cursor->old_y = y;	
    cursor->width = 17;
    cursor->height = 26;
}

void (cursor_destroy)(){
    free(cursor);
}

void (cursor_move)(int x, int y){

    if (cursor->x + x >= 0 && cursor->x + x + cursor->width < (int) h_res)
        cursor->x += x;

    if (cursor->y + y >= 0 && cursor->y + y + cursor->height < (int) v_res)
        cursor->y += y;
}
