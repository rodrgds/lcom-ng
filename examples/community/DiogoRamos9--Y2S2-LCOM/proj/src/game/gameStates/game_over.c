#include <lcom/lcf.h>

int game_over_option = 0;

void draw_game_over() {
    if ((cursor->x > 265 && cursor->x < 515) && (cursor->y > 250 && cursor->y < 300)) {
        game_over_option = 0;
        is_mouse_over = true;
    }
    
    else if ((cursor->x > 265 && cursor->x < 515) && (cursor->y > 270 && cursor->y < 370)) {
        game_over_option = 1;
        is_mouse_over = true;
    }
    else {
        is_mouse_over = false;
    }

    switch (game_over_option) {
        case 0:
            draw_sprite(gameover_tryagain_selected, 0, 0);
            break;
        case 1:
            draw_sprite(gameover_exit_selected, 0, 0);
            break;
    }
}
