#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "cursor.h"

int (initialize_cursor)(struct cursor *cursor){
    cursor->x = 385, cursor->y = 237;
    xpm_image_t c_img;
    cursor->img = c_img;
    uint8_t *cursor_sprite = xpm_load(cursor_xpm,XPM_8_8_8,&cursor->img);
    cursor->sprite = cursor_sprite;
    cursor->LBpressed = false;
    cursor->RBpressed = false;
    return 0;
}
