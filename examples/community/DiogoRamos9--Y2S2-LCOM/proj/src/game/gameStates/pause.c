#include <lcom/lcf.h>

int pause_option = 0;

void draw_pause() {
    if ((cursor->x > 265 && cursor->x < 515) && (cursor->y > 250 && cursor->y < 300)) {
        pause_option = 0;
        is_mouse_over = true;
    }
    
    else if ((cursor->x > 265 && cursor->x < 515) && (cursor->y > 320 && cursor->y < 370)) {
        pause_option = 1;
        is_mouse_over = true;
    }
    else {
        is_mouse_over = false;
    }
    switch (pause_option) {
        case 0:
            draw_sprite(pause_resume_selected, 0, 0);
            break;
        case 1:
            draw_sprite(pause_exit_selected, 0, 0);
            break;
    }
}
