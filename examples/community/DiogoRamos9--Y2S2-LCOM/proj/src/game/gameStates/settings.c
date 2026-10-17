#include "lcom/lcf.h"

int settings_option = 0;
int resolution_option = 0;

void draw_settings() {
    if ((cursor->x > 265 && cursor->x < 515) && (cursor->y > 215 && cursor->y < 273)) {
        settings_option = 0;
        is_mouse_over = true;
    }
    
    else if ((cursor->x > 265 && cursor->x < 515) && (cursor->y > 273 && cursor->y < 327)) {
        settings_option = 1;
        is_mouse_over = true;
    }
    
    else if ((cursor->x > 265 && cursor->x < 515) && (cursor->y > 327 && cursor->y < 385)) {
        settings_option = 2;
        is_mouse_over = true;
    }
    else {
        is_mouse_over = false;
    }

    switch (settings_option) {
        case 0:
            switch (resolution_option) {
                case 0:
                    draw_sprite(settings_resolution_480_selected, 0, 0);
                    break;
                case 1:
                    draw_sprite(settings_resolution_600_selected, 0, 0);
                    break;
                case 2:
                    draw_sprite(settings_resolution_768_selected, 0, 0);
                    break;
                case 3:
                    draw_sprite(settings_resolution_864_selected, 0, 0);
                    break;
                case 4:
                    draw_sprite(settings_resolution_1024_selected, 0, 0);
                    break;
            }
            break;
        case 1:
            draw_sprite(settings_clear_selected, 0, 0);
            break;
        case 2:
            draw_sprite(settings_back_selected, 0, 0);
            break;
        default:
            break;
    }
}
